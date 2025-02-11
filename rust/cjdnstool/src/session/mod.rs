mod show;

use crate::common::args::CommonArgs;
use eyre::Result;
use clap::Subcommand;

pub async fn session(common: CommonArgs, command: Command) -> Result<()> {
    use Command::*;
    match command {
        Show { ip6 } => show::show(common, ip6).await,
    }
}

#[derive(Subcommand)]
pub enum Command {
    /// Show current cjdns sessions (default).
    Show {
        /// Print ipv6 of sessions rather than pubkeys.
        #[arg(short = '6', long)]
        ip6: bool,
    },
}

impl Default for Command {
    fn default() -> Self {
        Command::Show { ip6: false }
    }
}
