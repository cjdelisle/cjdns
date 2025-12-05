
pub const SNODE_SAYS: u32   = 0xff100000;
pub const PF_PEER: u32      = 0xff200000;
pub const SNODE: u32        = 0xff300000;
pub const DHT_PEER: u32     = 0xff400000;
pub const PING_REPLY: u32   = 0xff500000;
pub const DHT_INCOMING: u32 = 0xff510000;
pub const DHT: u32          = 0xff600000;
// pub const DHT_MASK: u32     = 0x000fffff;
pub const SM_INCOMING: u32  = 0xff700000;
pub const SM_SEND: u32      = 0xff710000;
pub const NO_INFO: u32      = 0xfffffffe;
pub const DEAD_LINK: u32    = 0xffffffff;
pub const IC_PEER: u32      = 0xffff0000;
// pub const IC_PEER_MASK: u32 = 0x0000ffff;

pub fn print_metric(metric: i64) -> String {
    if metric > 0xffffffff && metric < 0 {
        return "INVALID".to_string();
    }
    let metric = metric as u32;
    match metric {
        DEAD_LINK => "DEAD_LINK".to_string(),
        NO_INFO => "NO_INFO".to_string(),
        _ if metric >= IC_PEER =>       format!("IC_PEER+{}", metric - IC_PEER),

        // Won't see these in sessions
        _ if metric >= SM_SEND =>       format!("SM_SEND+{}", metric - SM_SEND),
        _ if metric >= SM_INCOMING =>   format!("SM_INCOMING+{}", metric - SM_INCOMING),
        _ if metric >= DHT =>           format!("DHT+{}", metric - DHT),
        _ if metric >= DHT_INCOMING =>  format!("DHT_INCOMING+{}", metric - DHT_INCOMING),
        _ if metric >= PING_REPLY =>    format!("PING_REPLY+{}", metric - PING_REPLY),
        _ if metric >= DHT_PEER =>      format!("DHT_PEER+{}", metric - DHT_PEER),
        _ if metric >= SNODE =>         format!("SNODE+{}", metric - SNODE),
        _ if metric >= PF_PEER =>       format!("PF_PEER+{}", metric - PF_PEER),
        _ if metric >= SNODE_SAYS =>    format!("SNODE_SAYS+{}", metric - SNODE_SAYS),
        _ =>                            format!("UNKNOWN+{}", metric),
    }
}