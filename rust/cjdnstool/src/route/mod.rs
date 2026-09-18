mod get;

use clap::{Subcommand, ValueEnum};
use eyre::Result;

use crate::common::args::CommonArgs;

pub use self::get::resolve;

pub async fn route(common: CommonArgs, command: Command) -> Result<()> {
    use Command::*;
    match command {
        Get { dest, origin, from } => get::get(common, dest, origin, from).await,
    }
}

// TODO document CLI arguments

#[derive(Debug, Clone, PartialEq, ValueEnum)]
#[value(rename_all = "lower")]
pub enum ResolveFrom {
    Session,
    Snode,
}

#[derive(Subcommand)]
pub enum Command {
    /// Get the route to a given destination.
    Get {
        dest: String,
        origin: Option<String>,
        #[arg(long)]
        from: Option<ResolveFrom>,
    },
}
