//! Replay attack protector

#[derive(Default)]
pub struct ReplayProtector {
    /// Internal bitfield.
    bitfield: u64,

    /// Internal offset.
    base_offset: u32,

    /// Number of definite duplicate packets.
    duplicates: u32,

    /// Number of lost packets.
    lost_packets: u32,

    /// Number of packets which could not be verified because they were out of range.
    /// Growing `lost_packets` and `received_out_of_range` together indicate severe packet reordering issues.
    /// Just `received_out_of_range` growing along indicates duplicate packets.
    received_out_of_range: u32,
}

#[derive(Clone, Default, PartialEq, Eq, Debug)]
pub struct ReplayProtectorStats {
    pub received_packets: u32,
    pub lost_packets: u32,
    pub received_unexpected: u32,
    pub duplicate_packets: u32,
}

impl ReplayProtector {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn reset(&mut self) {
        *self = Self::default();
    }

    pub fn init(&mut self, first_nonce: u32) {
        self.base_offset = first_nonce;
        self.bitfield = 0;
    }

    pub fn stats(&self) -> ReplayProtectorStats {
        ReplayProtectorStats {
            received_packets: self.base_offset + self.bitfield.count_ones(),
            lost_packets: self.lost_packets,
            received_unexpected: self.received_out_of_range,
            duplicate_packets: self.duplicates,
        }
    }

    /// Check a nonce and file it as being seen.
    ///
    /// Don't call this until the packet has been authenticated
    /// or else forged packets will make legit ones appear to be duplicates.
    ///
    /// Arg `nonce` is the number to check, this should be a counter nonce
    /// as numbers less than 32 minus the highest seen nonce will be dropped
    /// erroneously.
    ///
    /// Returns `true` if the packet is provably not a replay, otherwise `false`.
    pub fn check_nonce(&mut self, nonce: u32) -> bool {
        if nonce < self.base_offset {
            self.received_out_of_range += 1;
            return false;
        }

        let mut offset = nonce - self.base_offset;
        while offset > 63 {
            if self.bitfield & 0xFFFF_FFFF == 0xFFFF_FFFF {
                // Happy path, low 32 bits are checked in, rotate and continue.
                offset = self.do_shift(offset, 32);
            } else {
                // We are going to have to accept some losses, take offset - 47 to mitigate that
                // as much as possible.
                debug_assert!(offset >= 47);
                self.lost_packets += self.lost_in_shift(offset - 47);
                offset = self.do_shift(offset, offset - 47);
            }
        }

        let mask = 1_u64 << offset;

        if self.bitfield & mask != 0 {
            self.duplicates += 1;
            return false;
        }

        self.bitfield |= mask;
        return true;
    }

    #[inline]
    fn do_shift(&mut self, offset: u32, bits: u32) -> u32 {
        self.base_offset += bits;
        if bits > 63 {
            self.bitfield = 0;
        } else {
            self.bitfield >>= bits;
        }
        offset - bits
    }

    #[inline]
    fn lost_in_shift(&self, shift_amount: u32) -> u32 {
        if shift_amount == 0 {
            return 0;
        }
        if shift_amount > 63 {
            return shift_amount - self.bitfield.count_ones();
        }
        return shift_amount - (self.bitfield << (64 - shift_amount)).count_ones();
    }
}
