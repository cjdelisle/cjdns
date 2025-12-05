mod get;

pub use get::resolve;

use crate::common::args::CommonArgs;
use eyre::Result;
use clap::Subcommand;
use clap::ValueEnum;

pub async fn route(common: CommonArgs, command: Command) -> Result<()> {
    use Command::*;
    match command {
        Get { dest, origin, from } => get::get(common, dest, origin, from).await,
    }
}

#[derive(Debug, Clone, PartialEq, ValueEnum)]
#[value(rename_all = "lower")]
pub enum ResolveFrom {
    Session,
    Snode,
}

#[derive(Subcommand)]
pub enum Command {
    /// Get the route to a given destination
    Get {
        dest: String,
        origin: Option<String>,
        #[arg(long)]
        from: Option<ResolveFrom>,
    },
}