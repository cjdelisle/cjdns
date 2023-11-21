use cjdns_admin::Opts;
use clap::Args;

#[derive(Args)]
pub struct CommonArgs {
    /// Remote IP address (either IPv4 or IPv6).
    #[arg(short, long, value_name = "IP")]
    address: Option<String>,

    /// Remote UDP port.
    #[arg(short, long, value_name = "PORT")]
    port: Option<u16>,

    /// Path to config file (~/.cjdnsadmin used by default).
    #[arg(short, long, value_name = "PATH")]
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

    #[allow(dead_code)]
    pub fn with_password(self, password: Option<String>) -> Opts {
        Opts {
            addr: self.address,
            port: self.port,
            password,
            config_file_path: self.cjdnsadmin,
            anon: false,
        }
    }
}
