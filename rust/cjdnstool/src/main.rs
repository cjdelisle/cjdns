mod common;
mod peers;
mod session;
mod util;
mod wire;

use clap::{Parser, Subcommand};
use std::{
    env,
    path::MAIN_SEPARATOR,
    process::{ExitCode, Termination},
};

#[derive(Parser)]
#[command(author, version, about, long_about = None)]
struct Args {
    #[command(flatten)]
    common: common::CommonArgs,

    #[command(subcommand)]
    command: Command,
}

#[derive(Subcommand)]
enum Command {
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
}

#[tokio::main]
async fn main() -> MainResult {
    use Command::*;
    match Args::try_parse() {
        Ok(args) => match args.command {
            Peers { command } => peers::peers(args.common, command.unwrap_or_default())
                .await
                .into(),
            Session { command } => session::session(args.common, command.unwrap_or_default())
                .await
                .into(),
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
                let arg0 = env::args().next();
                let exe = arg0
                    .as_ref()
                    .and_then(|path| path.rsplit(MAIN_SEPARATOR).next())
                    .unwrap_or("cjdnstool");
                eprintln!("{exe}: {err}");
                ExitCode::FAILURE
            }
        }
    }
}
