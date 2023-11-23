mod show;

use crate::common::CommonArgs;
use anyhow::Result;
use clap::Subcommand;

#[derive(Subcommand)]
pub enum Command {
    /// Show current cjdns peers (default).
    Show,
}

impl Default for Command {
    fn default() -> Self {
        Command::Show
    }
}

pub async fn peers(common: CommonArgs, command: Command) -> Result<()> {
    use Command::*;
    match command {
        Show => show::show(common).await,
    }
}
