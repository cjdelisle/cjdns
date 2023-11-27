use cjdns_admin::Opts;
use clap::Args;

#[derive(Args)]
pub struct CommonArgs {
    /// Remote IP address (either IPv4 or IPv6).
    #[arg(short = 'a', long, value_name = "IP")]
    address: Option<String>,

    /// Remote UDP port.
    #[arg(short = 'p', long, value_name = "PORT")]
    port: Option<u16>,

    /// Connection password for cjdns instance.
    #[arg(short = 'P', long, value_name = "PASSWORD")]
    password: Option<String>,

    /// Path to config file (~/.cjdnsadmin used by default).
    #[arg(short = 'c', long, value_name = "PATH")]
    cjdnsadmin: Option<String>,
}

impl CommonArgs {
    pub fn as_anon(self) -> Opts {
        Opts {
            addr: self.address,
            port: self.port,
            password: None,
            config_file_path: self.cjdnsadmin,
            anon: true,
        }
    }

    pub fn with_auth(self) -> Opts {
        Opts {
            addr: self.address,
            port: self.port,
            password: Some(self.password.unwrap_or_else(|| "NONE".to_owned())),
            config_file_path: self.cjdnsadmin,
            anon: false,
        }
    }
}
