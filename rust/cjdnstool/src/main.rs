mod cexec;
mod common;
mod peers;
mod session;
mod util;

use clap::{Parser, Subcommand};
use std::process::{ExitCode, Termination};

#[derive(Parser)]
#[command(author, version, about, long_about = None)]
struct Args {
    #[command(flatten)]
    common: common::args::CommonArgs,

    #[command(subcommand)]
    command: Command,
}

#[derive(Subcommand)]
enum Command {
    /// Call specified cjdns RPC, or list available RPCs if none specified.
    #[command(long_about = Some(cexec::LONG_ABOUT))]
    Cexec {
        /// Name of the specified RPC to call, or list RPCs if none.
        rpc: Option<String>,

        /// Arguments to the specified RPC, in the form --name=value.
        #[arg(allow_hyphen_values = true)]
        args: Vec<String>,
    },

    /// Perform operations with cjdns peers (show current peers by default).
    Peers {
        #[command(subcommand)]
        command: Option<peers::Command>,
    },

    /// Perform operations with cjdns sessions (show current sessions by default).
    Session {
        #[command(subcommand)]
        command: Option<session::Command>,
    },

    /// Locally perform utility functions over data (public and private keys).
    Util {
        #[command(subcommand)]
        command: util::Command,
    },
}

#[tokio::main]
async fn main() -> MainResult {
    use Command::*;
    match Args::try_parse() {
        Ok(args) => match args.command {
            Cexec {
                rpc,
                args: rpc_args,
            } => cexec::cexec(args.common, rpc, rpc_args).await.into(),
            Peers { command } => peers::peers(args.common, command.unwrap_or_default())
                .await
                .into(),
            Session { command } => session::session(args.common, command.unwrap_or_default())
                .await
                .into(),
            Util { command } => util::util(command).await.into(),
        },
        Err(err) => err.into(),
    }
}

enum MainResult {
    Success,
    ClapError(clap::error::Error),
    AnyhowError(anyhow::Error),
}

impl From<clap::error::Error> for MainResult {
    fn from(value: clap::error::Error) -> Self {
        MainResult::ClapError(value)
    }
}

impl From<anyhow::Error> for MainResult {
    fn from(value: anyhow::Error) -> Self {
        MainResult::AnyhowError(value)
    }
}

impl<T: Into<Self>> From<Result<(), T>> for MainResult {
    fn from(value: Result<(), T>) -> Self {
        match value {
            Ok(_) => MainResult::Success,
            Err(err) => err.into(),
        }
    }
}

impl Termination for MainResult {
    fn report(self) -> ExitCode {
        match self {
            Self::Success => ExitCode::SUCCESS,
            Self::ClapError(err) => {
                err.print().expect("Failed to print diagnostic message");
                ExitCode::from(err.exit_code() as u8)
            }
            Self::AnyhowError(err) => {
                let exe = common::utils::exe_name();
                eprintln!("{exe}: {err}");
                ExitCode::FAILURE
            }
        }
    }
}
