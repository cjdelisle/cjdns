use cjdns::bencode::bendy::value::Value;
use std::collections::BTreeMap;
use std::io::Read;
use std::borrow::Cow;
use cjdns::bytes::message::RWrite;
use std::io::Result as IoResult;

#[derive(Debug)]
pub enum ParseError {
    MalformedInput(String, usize), // Error with description and line number
}

struct JsonParser<R: Read> {
    reader: R,
    current_line: usize,
    lax_mode: bool,
    last_chr: Option<u8>,
}

fn format_chr(chr: u8) -> String {
    char::from_u32(chr as _).map(|x|format!("{x}")).unwrap_or_else(||format!("CODE[{chr}]"))
}

impl<R: Read> JsonParser<R> {
    fn new(reader: R, lax_mode: bool) -> Self {
        JsonParser {
            reader,
            current_line: 1,
            lax_mode,
            last_chr: None,
        }
    }

    fn parse(&mut self) -> Result<Value<'static>, ParseError> {
        loop {
            let byte = self.next_meaningful_char()?;
            match byte {
                b'{' => return self.parse_dict(),
                b'[' => return self.parse_list(),
                b'"' => return self.parse_string(),
                b'0'..=b'9' | b'-' => return self.parse_int(byte),
                x => {
                    return Err(ParseError::MalformedInput(
                        format!("Unexpected char {}", format_chr(x)),
                        self.current_line,
                    ));
                }
            }
        }
    }

    fn read_byte(&mut self) -> Result<Option<u8>, ParseError> {
        if let Some(lc) = self.last_chr.take() {
            return Ok(Some(lc));
        }
        let mut byte = [0; 1];
        match self.reader.read(&mut byte) {
            Ok(0) => Ok(None), // End of stream
            Ok(_) => Ok(Some(byte[0])),
            Err(_) => Err(ParseError::MalformedInput("Error reading input".into(), self.current_line)),
        }
    }

    fn next_meaningful_char(&mut self) -> Result<u8, ParseError> {
        while let Some(byte) = self.read_byte()? {
            match byte {
                b'/' => self.handle_comment()?,
                b' ' | b'\t' => {}
                b'\n' => { self.current_line += 1 }
                x => {
                    return Ok(x);
                }
            }
        }
        Err(ParseError::MalformedInput("Ran out of content while parsing".into(), self.current_line))
    }

    fn parse_dict(&mut self) -> Result<Value<'static>, ParseError> {
        let mut dict = BTreeMap::new();
        let mut need_comma = false;
        loop {
            let byte = self.next_meaningful_char()?;

            if need_comma && byte != b'}' && byte != b',' && !self.lax_mode {
                return Err(ParseError::MalformedInput(
                    format!("Dict: Expecting ',' but got '{}'", format_chr(byte)),
                    self.current_line)
                );
            }
            match byte {
                b'}' => {
                    if !dict.is_empty() && !need_comma && !self.lax_mode {
                        return Err(ParseError::MalformedInput(
                            format!("Dict: Unexpected trailing ','"),
                            self.current_line)
                        );
                    }
                    return Ok(Value::Dict(dict));
                }
                b'"' => {
                    let key = if let Value::Bytes(x) = self.parse_string()? {
                        x
                    } else {
                        unreachable!();
                    };
                    let mut need_colon = true;
                    let val = loop {
                        let byte = self.next_meaningful_char()?;
                        if need_colon && byte != b':' && !self.lax_mode {
                            return Err(ParseError::MalformedInput(
                                format!("Dict: Expecting ':' but got '{}'", format_chr(byte)),
                                self.current_line)
                            );
                        }
                        match byte {
                            b':' => {
                                if !need_colon && !self.lax_mode {
                                    return Err(ParseError::MalformedInput(
                                        "Dict: Expected value but got duplicate ':'".into(), self.current_line));
                                }
                                need_colon = false;
                            }
                            b'{' | b'[' | b'"' | b'0'..=b'9' | b'-' => {
                                self.last_chr = Some(byte);
                                break self.parse()?;
                            }
                            x => {
                                return Err(ParseError::MalformedInput(
                                    format!("Dict: Unexpected character '{}', exppecting value", format_chr(x)),
                                    self.current_line)
                                );
                            }
                        }
                    };
                    dict.insert(key, val);
                    need_comma = true;
                }
                b',' => {
                    if !need_comma && !self.lax_mode {
                        return Err(ParseError::MalformedInput("Dict: Unexpected comma".into(), self.current_line));
                    }
                    need_comma = false;
                }
                x => {
                    return Err(ParseError::MalformedInput(
                        format!("Dict: Unexpected character '{}'", format_chr(x)),
                        self.current_line)
                    );
                }
            }
        }
    }

    fn parse_list(&mut self) -> Result<Value<'static>, ParseError> {
        let mut list = vec![];
        let mut need_comma = false;
        loop {
            let byte = self.next_meaningful_char()?;
            if need_comma && byte != b']' && byte != b',' && !self.lax_mode {
                return Err(ParseError::MalformedInput(
                    format!("List: Expecting ',' but got '{}'", format_chr(byte)),
                    self.current_line)
                );
            }
            match byte {
                b']' => {
                    if !list.is_empty() && !need_comma && !self.lax_mode {
                        return Err(ParseError::MalformedInput(
                            format!("List: Unexpected trailing ','"),
                            self.current_line)
                        );
                    }
                    return Ok(Value::List(list));
                }
                b',' => {
                    if !need_comma && !self.lax_mode {
                        if !need_comma && !self.lax_mode {
                            return Err(ParseError::MalformedInput("List: Unexpected comma".into(), self.current_line));
                        }
                    }
                    need_comma = false;
                }
                b'{' | b'[' | b'"' | b'0'..=b'9' | b'-' => {
                    self.last_chr = Some(byte);
                    list.push(self.parse()?);
                    need_comma = true;
                }
                x => {
                    return Err(ParseError::MalformedInput(
                        format!("List: Unexpected character '{}'", format_chr(x)),
                        self.current_line)
                    );
                }
            }
        }
    }

    fn parse_string(&mut self) -> Result<Value<'static>, ParseError> {
        let mut result = vec![];
        while let Some(byte) = self.read_byte()? {
            match byte {
                b'"' => break,
                b'\\' => {
                    if let Some(escaped_byte) = self.read_byte()? {
                        match escaped_byte {
                            b'x' => {
                                let hex = self.read_two_hex_digits()?;
                                result.push(hex);
                            }
                            _ => {
                                return Err(ParseError::MalformedInput(
                                    format!("String: Invalid escape sequence: '\\{}'", format_chr(byte)),
                                    self.current_line,
                                ));
                            }
                        }
                    }
                }
                _ => result.push(byte),
            }
        }
        Ok(Value::Bytes(Cow::Owned(result)))
    }

    fn parse_int(&mut self, first_byte: u8) -> Result<Value<'static>, ParseError> {
        let mut num_str = String::new();
        num_str.push(first_byte as char);
        while let Some(byte) = self.read_byte()? {
            match byte {
                b'0'..=b'9' | b'-' => num_str.push(byte as char),
                x => {
                    self.last_chr = Some(x);
                    break;
                },
            }
        }
        match num_str.parse::<i64>() {
            Ok(num) => Ok(Value::Integer(num)),
            Err(_) => Err(ParseError::MalformedInput("Invalid integer".into(), self.current_line)),
        }
    }

    fn handle_comment(&mut self) -> Result<(), ParseError> {
        let next_byte = self.read_byte()?;
        match next_byte {
            Some(b'*') => self.consume_multiline_comment(),
            Some(b'/') => self.consume_singleline_comment(),
            _ => Ok(()), // Ignore invalid comment
        }
    }

    fn consume_singleline_comment(&mut self) -> Result<(), ParseError> {
        while let Some(byte) = self.read_byte()? {
            if byte == b'\n' {
                self.current_line += 1;
                break;
            }
        }
        Ok(())
    }

    fn consume_multiline_comment(&mut self) -> Result<(), ParseError> {
        while let Some(byte) = self.read_byte()? {
            if byte == b'*' {
                if let Some(b'/') = self.read_byte()? {
                    break;
                }
            }
        }
        Ok(())
    }

    fn read_two_hex_digits(&mut self) -> Result<u8, ParseError> {
        let mut hex = [0u8; 2];
        for i in 0..2 {
            if let Some(byte) = self.read_byte()? {
                hex[i] = byte;
            } else {
                return Err(ParseError::MalformedInput("Incomplete hex escape".into(), self.current_line));
            }
        }
        let hex_str = std::str::from_utf8(&hex).map_err(|_| ParseError::MalformedInput("Invalid hex escape".into(), self.current_line))?;
        u8::from_str_radix(hex_str, 16).map_err(|_| ParseError::MalformedInput("Invalid hex digits".into(), self.current_line))
    }
}

fn serialize_string<W: RWrite>(writer: &mut W, s: &[u8]) -> std::io::Result<()> {
    writer.write_all(b"\"")?;
    for &byte in s.iter().rev() {
        if byte < 126 && byte > 31 && byte != b'\\' && byte != b'"' {
            writer.write_all(&[byte])?;
        } else {
            let escaped = format!(r"\x{:02X}", byte);
            writer.write_all(escaped.as_bytes())?;
        }
    }
    writer.write_all(b"\"")?;
    Ok(())
}

fn serialize_reverse<'a, W: RWrite>(writer: &mut W, obj: &Value<'a>, indent_level: usize) -> std::io::Result<()> {
    let indent = "  ".repeat(indent_level);
    let indent2 = "  ".repeat(indent_level + 1);

    match obj {
        Value::Bytes(s) => serialize_string(writer, s)?,
        Value::Integer(i) => {
            let int_str = i.to_string();
            writer.write_all(&int_str.as_bytes())?;
        }
        Value::List(list) => {
            writer.write_all(b"]")?;
            if !list.is_empty() {
                writer.write_all(indent.as_bytes())?;
                writer.write_all(b"\n")?;
                for (i, item) in list.iter().rev().enumerate() {
                    if i > 0 {
                        writer.write_all(b",")?;
                    }
                    serialize_reverse(writer, item, indent_level + 1)?;
                    writer.write_all(indent2.as_bytes())?;
                    writer.write_all(b"\n")?;
                }
            }
            writer.write_all(b"[")?;
        }
        Value::Dict(dict) => {
            writer.write_all(b"}")?;
            if !dict.is_empty() {
                writer.write_all(indent.as_bytes())?;
                writer.write_all(b"\n")?;
                for (i, (key, value)) in dict.iter().rev().enumerate() {
                    if i > 0 {
                        writer.write_all(b",\n")?;
                    }
                    serialize_reverse(writer, value, indent_level + 1)?;
                    writer.write_all(b": ")?;
                    serialize_string(writer, key)?;
                    writer.write_all(indent2.as_bytes())?;
                }
                writer.write_all(b"{\n")?;
            } else {
                writer.write_all(b"{")?;
            }
        }
    }

    Ok(())
}

pub fn parse<R: Read>(reader: R, lax_mode: bool) -> Result<Value<'static>, ParseError> {
    JsonParser::new(reader, lax_mode).parse()
}

pub fn serialize<'a, W: RWrite>(writer: &mut W, obj: &Value<'a>) -> IoResult<()> {
    serialize_reverse(writer, obj, 0)
}

#[cfg(test)]
mod test {
    use cjdns::bencode::bendy::encoding::ToBencode;

    use crate::interface::wire::message::Message;

    use super::{parse, serialize};

    #[test]
    fn test() {
        let conf = r#"
{
    // Private key:
    // Your confidentiality and data integrity depend on this key, keep it secret!
    "privateKey": "0b4eed1835d708a79914a8925e8d5fff706a5258dfb4826f56d4c29eaec7abc5",

    // This key corresponds to the public key and ipv6 address:
    "publicKey": "f09rvtmr2s25942ygutztv8bf70ny8zd0kpffk8lmbzm40qty9k0.k",
    "ipv6": "fcea:4775:eacd:7c3b:6fbc:c02b:43d6:53e1",

    // Anyone connecting and offering these passwords on connection will be allowed.
    //
    // WARNING: If a "login" parameter is passed, someone sniffing on the wire can
    //          sniff the packet and crack to find it. If the "login" is not passed
    //          then the hash of the 'password' is effectively the login, therefore
    //          that can be cracked.
    //
    "authorizedPasswords": [
        // Password is a unique string which is known to the client and server.
        // User is an optional login name and will also be used to display the peer.
        { "password": "dq1nlc6g46k9v5pd6hkrclpsz7807nl", "user": "default-login" }

        // More passwords should look like this.
        // { "password": "86251h6k89u20rx9k0f4tlhwc6qkj5j", "user": "my-second-peer" },
        // { "password": "xzmnu69xqhff297pz1v3rnbhl034z1f", "user": "my-third-peer" },
        // { "password": "ksr4nftwznrsqw5z3t69dwn6v3rw6z4", "user": "my-fourth-peer" },

        // Below is an example of your connection credentials
        // that you can give to other people so they can connect
        // to you using your default password (from above).
        // The login field here yourself to your peer and the peerName field
        // is the name the peer which will be displayed in peerStats
        // Adding a unique password for each peer is advisable
        // so that leaks can be isolated.
        /*
         "your.external.ip.goes.here:65288": {
             "login": "default-login",
             "password": "dq1nlc6g46k9v5pd6hkrclpsz7807nl",
             "publicKey": "f09rvtmr2s25942ygutztv8bf70ny8zd0kpffk8lmbzm40qty9k0.k",
             "peerName": "your-name-goes-here"
         },
         */
    ],

    // Settings for administering and extracting information from your router.
    // This interface provides functions which can be called through a UDP socket.
    // See admin/Readme.md for more information about the API and try:
    // ./tools/cexec
    // For a list of functions which can be called.
    // For example: ./tools/cexec 'memory()'
    // will call a function which gets the core's current memory consumption.
    // ./tools/cjdnslog
    // is a tool which uses this admin interface to get logs from cjdns.
    "admin": {
        // Port to bind the admin RPC server to.
        "bind": "127.0.0.1:11234",

        // Password for admin RPC server.
        // This is a static password by default, so that tools like
        // ./tools/cexec can use the API without you creating a
        // config file at ~/.cjdnsadmin first. If you decide to
        // expose the admin API to the network, change the password!
        "password": "NONE"
    },

    // Interfaces to connect to the switch core.
    "interfaces": {
        // The interface which connects over UDP/IP based VPN tunnel.
        "UDPInterface": [
            {
                // Bind to this port.
                "bind": "0.0.0.0:65288",
                // Set the DSCP value for Qos. Default is 0.
                // "dscp": 46,

                // Automatically connect to other nodes on the same LAN
                // This works by binding a second port and sending beacons
                // containing the main data port.
                // beacon is a number between 0 and 2:
                //   0 -> do not beacon nor connect to other nodes who beacon
                //   1 -> quiet mode, accept beacons from other nodes only
                //   2 -> send and accept beacons
                // beaconDevices is a list which can contain names of devices such
                // as eth0, as well as broadcast addresses to send to, such as
                // 192.168.101.255, or the pseudo-name "all".
                // in order to auto-peer, all cjdns nodes must use the same
                // beaconPort.
                "beacon": 2,
                "beaconDevices": [ "all" ],
                "beaconPort": 64512,

                // Nodes to connect to (IPv4 only).
                "connectTo": {
                    // Add connection credentials here to join the network
                    // If you have several, don't forget the separating commas
                    // They should look like:
                    // "ipv4 address:port": {
                    //     "login": "(optional) name your peer has for you"
                    //     "password": "password to connect with",
                    //     "publicKey": "remote node key.k",
                    //     "peerName": "(optional) human-readable name for peer"
                    // },
                    // Ask somebody who is already connected.
                }
            },
            {
                // Bind to this port.
                "bind": "[::]:65288",
                // Set the DSCP value for Qos. Default is 0.
                // "dscp": 46,

                // Nodes to connect to (IPv6 only).
                "connectTo": {
                    // Add connection credentials here to join the network
                    // Ask somebody who is already connected.
                }
            }
        ],

        // The interface which allows peering using layer-2 ethernet frames
        "_disabled_ETHInterface": [
            // Alternatively bind to just one device and either beacon and/or
            // connect to a specified MAC address
            {
                // Bind to this device (interface name, not MAC)
                // "all" is a pseudo-name which will try to connect to all devices.
                "bind": "all",

                // Auto-connect to other cjdns nodes on the same network.
                // Options:
                //
                // 0 -- Disabled.
                //
                // 1 -- Accept beacons, this will cause cjdns to accept incoming
                //      beacon messages and try connecting to the sender.
                //
                // 2 -- Accept and send beacons, this will cause cjdns to broadcast
                //      messages on the local network which contain a randomly
                //      generated per-session password, other nodes which have this
                //      set to 1 or 2 will hear the beacon messages and connect
                //      automatically.
                //
                "beacon": 2,

                // Node(s) to connect to manually
                // Note: does not work with "all" pseudo-device-name
                "connectTo": {
                    // Credentials for connecting look similar to UDP credentials
                    // except they begin with the mac address, for example:
                    // "01:02:03:04:05:06":{"password":"a","publicKey":"b"}
                }
            }
        ]

    },

    // Configuration for the router.
    "router": {
        // DNS Seeds, these will be used to add peers automatically.
        // The first seed in the list is trusted to provide the snode.
        "dnsSeeds": [
            "seed.cjdns.fr"
        ],

        // supernodes, if none are specified they'll be taken from your peers
        "supernodes": [
            //"6743gf5tw80ExampleExampleExampleExamplevlyb23zfnuzv0.k",
        ],

        // The interface which is used for connecting to the cjdns network.
        "interface": {
            // The type of interface (only TUNInterface is supported for now)
            "type": "TUNInterface"

            // The name of a persistent TUN device to use.
            // This for starting cjdroute as its own user.
            // *MOST USERS DON'T NEED THIS*
            //"tunDevice": "tun0"
        },

        // As an alternative to the TUN interface, you can create a socket interface
        // which will create a UNIX socket which emits packets that would otherwise
        // be sent through the TUN device.
        // To enable this interface, change the name of the above TUN interface to
        // "_disabled_interface" and change the name of this interface to
        // simply "interface"
        "_disabled_interface": {
            "type": "SocketInterface",

            // The filesystem path to the socket to create or connect to.
            "socketFullPath": "/var/run/cjdns.sock"
        },

        // System for tunneling IPv4 and ICANN IPv6 through cjdns.
        // This is using the cjdns switch layer as a VPN carrier.
        "ipTunnel": {
            // Nodes allowed to connect to us.
            // When a node with the given public key connects, give them the
            // ip4 and/or ip6 addresses listed.
            "allowedConnections": [
                // Give the client an address on 192.168.1.0/24, and an address
                // it thinks has all of IPv6 behind it.
                // ip4Prefix is the set of addresses which are routable from the tun
                // for example, if you're advertizing a VPN into a company network
                // which exists in 10.123.45.0/24 space, ip4Prefix should be 24
                // default is 32 for ipv4 and 128 for ipv6
                // so by default it will not install a route
                // ip4Alloc is the block of addresses which are allocated to the
                // for example if you want to issue 4 addresses to the client, those
                // being 192.168.123.0 to 192.168.123.3, you would set this to 30
                // default is 32 for ipv4 and 128 for ipv6 (1 address)
                // {
                //     "publicKey": "f64hfl7c4uxt6krmhPutTheRealAddressOfANodeHere7kfm5m0.k",
                //     "ip4Address": "192.168.1.24",
                //     "ip4Prefix": 0,
                //     "ip4Alloc": 32,
                //     "ip6Address": "2001:123:ab::10",
                //     "ip6Prefix": 0
                //     "ip6Alloc": 64,
                // },

                // It's ok to only specify one address and prefix/alloc are optional.
                // {
                //     "publicKey": "ydq8csdk8p8ThisIsJustAnExampleAddresstxuyqdf27hvn2z0.k",
                //     "ip4Address": "192.168.1.25",
                //     "ip4Prefix": 0,
                // }
            ],

            "outgoingConnections": [
                // Connect to one or more machines and ask them for IP addresses.
                // "6743gf5tw80ExampleExampleExampleExamplevlyb23zfnuzv0.k",
                // "pw9tfmr8pcrExampleExampleExampleExample8rhg1pgwpwf80.k",
                // "g91lxyxhq0kExampleExampleExampleExample6t0mknuhw75l0.k"
            ]
        }
    },

    // Dropping permissions.
    // In the event of a serious security exploit in cjdns, leak of confidential
    // network traffic and/or keys is highly likely but the following rules are
    // designed to prevent the attack from spreading to the system on which cjdns
    // is running.
    // Counter-intuitively, cjdns is *more* secure if it is started as root because
    // non-root users do not have permission to use chroot or change usernames,
    // limiting the effectiveness of the mitigations herein.
    "security": [
        // Change the user id to sandbox the cjdns process after it starts.
        // If keepNetAdmin is set to 0, IPTunnel will be unable to set IP addresses
        // and ETHInterface will be unable to hot-add new interfaces
        // Use { "setuser": 0 } to disable.
        // Default: enabled with keepNetAdmin
        { "setuser": "nobody", "keepNetAdmin": 1 },

        // Chroot changes the filesystem root directory which cjdns sees, blocking it
        // from accessing files outside of the chroot sandbox, if the user does not
        // have permission to use chroot(), this will fail quietly.
        // Use { "chroot": 0 } to disable.
        // Default: enabled (using "/var/run")
        { "chroot": "/var/run/" },

        // Nofiles is a deprecated security feature which prevents cjdns from opening
        // any files at all, using this will block setting of IP addresses and
        // hot-adding ETHInterface devices but for users who do not need this, it
        // provides a formidable sandbox.
        // Default: disabled
        { "nofiles": 0 },

        // Noforks will prevent cjdns from spawning any new processes or threads,
        // this prevents many types of exploits from attacking the wider system.
        // Default: enabled
        { "noforks": 1 },

        // The client sets up the core using a sequence of RPC calls, the responses
        // to these calls are verified but in the event that the client crashes
        // setup of the core completes, it could leave the core in an insecure state
        // This call constitutes the client telling the core that the security rules
        // have been fully applied and the core may run. Without it, the core will
        // exit within a few seconds with return code 232.
        // Default: enabled
        { "setupComplete": 1 }
    ],

    // Logging
    "logging": {
        // Uncomment to have cjdns log to stdout rather than making logs available
        // via the admin socket.
        // "logTo": "stdout"
    },

    // If set to non-zero, cjdns will not fork to the background.
    // Recommended for use in conjunction with "logTo":"stdout".
    "noBackground": 0,

    // Path for admin control pipe:
    // If you pass only a filename then cjdns will guess the full path
    // On unix the default path is /tmp/
    // On windows: \\.\pipe\
    "pipe": "cjdroute.sock",

    // This is to make the configuration be parsed in strict mode, which allows
    // it to be edited externally using cjdnsconf.
    "version": 2
}
        "#;
        let mut msg = Message::new(20000);
        msg.push_bytes(conf.as_bytes()).unwrap();
        let res = parse(&mut msg, false).unwrap();
        let d = match res.clone() {
            cjdns::bencode::bendy::value::Value::Dict(d) => d,
            _ => panic!("Wrong type"),
        };
        let sec = d.get(&b"security"[..]).unwrap();
        let sec = match sec {
            cjdns::bencode::bendy::value::Value::List(d) => d,
            _ => panic!("Wrong type"),
        };
        let sec0 = sec.get(0).unwrap();
        let sec0 = match sec0 {
            cjdns::bencode::bendy::value::Value::Dict(d) => d,
            _ => panic!("Wrong type"),
        };
        // check that sec0 has "setuser" key
        let _ = sec0.get(&b"setuser"[..]).unwrap();

        msg.clear();
        serialize(&mut msg, &res).unwrap();
        println!("{}", String::from_utf8_lossy(msg.bytes()));

        let res2 = parse(&mut msg, false).unwrap();
        let bres = res.to_bencode().unwrap();
        let bres2 = res2.to_bencode().unwrap();
        assert_eq!(bres, bres2);
    }
}