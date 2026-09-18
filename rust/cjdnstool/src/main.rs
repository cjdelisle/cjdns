mod cexec;
mod common;
mod log;
mod peers;
mod ping;
mod route;
mod session;
mod util;

use std::{
    future::Future,
    io::{self, IsTerminal as _, stderr},
    process::{ExitCode, Termination},
};

use clap::{Parser, Subcommand};
use color_eyre::config::{HookBuilder, Theme};
use tokio::runtime::Runtime;

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

    /// Stream event logs from CJDNS instance to stdout.
    ///
    /// Once streaming, press Ctrl+C (or send SIGINT on Unix) to stop gracefully.
    Log {
        /// Log verbosity level.
        ///
        /// If present, instance event with deeper verbosity levels than specified will be filtered out.
        ///
        /// Note: For the log subcommand, the placement of the short form of this flag is important.
        ///
        /// In order for it to influence the logging of instance events, rather than client side events,
        /// it should be passed AFTER the log subcommand.
        #[arg(short = 'v', long, value_name = "LEVEL", ignore_case = true)]
        verbosity: Option<log::Verbosity>,

        /// CJDNS source code file name, e.g. "CryptoAuth.c".
        ///
        /// If present, only instance events fired from a function defined within the source file
        /// with the matching name will be displayed.
        #[arg(short = 'f', long, value_name = "NAME")]
        file: Option<String>,

        /// Line number in source code file.
        ///
        /// If present, only instance events fired from a line with the matching line number
        /// will be displayed.
        #[arg(short = 'l', long, value_name = "NUM")]
        line: Option<u64>,

        /// Output human-readable time (RFC 3339 / ISO 8601) instead of Unix timestamps.
        #[arg(short = 'H', long)]
        human_time: bool,
    },

    /// Perform operations with cjdns peers (show current peers by default).
    Peers {
        #[command(subcommand)]
        command: Option<peers::Command>,
    },

    /// Send a cjdns ping to a node.
    #[command(long_about = Some(ping::LONG_ABOUT))]
    Ping {
        /// Send this type of ping message, default: "router".
        #[arg(short = 't', long = "type", value_name = "TYPE")]
        typ: Option<ping::Type>,

        /// Resolve the path using this method, default: "default".
        #[arg(short = 'r', long)]
        resolve: Option<ping::Resolve>,

        /// Stop after <count> replies.
        #[arg(short = 'c', long)]
        count: Option<u32>,

        /// Number of data bytes to be sent, default: pattern.len() or zero.
        #[arg(short = 'l', long)]
        length: Option<u16>,

        /// Hex data pattern, default: --length random bytes, if length > pattern.len(), pattern is repeated.
        #[arg(short = 'p', long)]
        pattern: Option<String>,

        /// Display additional data, including the entire response in the case of router ping
        ///
        /// Note: For the ping subcommand in particular, the placement of this flag is important.
        ///
        /// In order for it to influence the router ping response, rather than the logging of client side events,
        /// it should be passed AFTER the ping subcommand.
        ///
        /// It is possible to apply the flag twice separately, both before and after,
        /// in which case both behaviors will be influenced.
        #[arg(short = 'v', long)]
        verbose: bool,

        /// Destination path, address, or IPv6.
        dest: String,
    },

    /// Get route to destination.
    Route {
        #[command(subcommand)]
        command: route::Command,
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

fn main() -> MainResult {
    // Realistically should always succeed, but
    if let Err(error) = install_eyre_hook() {
        eprintln!("Error installing eyre hook: {error}");
    }

    if let Err(error) = dotenvy::dotenv()
        && !error.not_found()
    {
        eprintln!("Error parsing .env file(s): {error}");
    }

    match Args::try_parse() {
        Ok(args) => {
            use Command::*;

            if let Err(error) = args.common.init_logger() {
                eprintln!("Error initializing logger: {error}");
            }

            match args.command {
                Cexec {
                    rpc,
                    args: rpc_args,
                } => with_tokio(cexec::cexec(args.common, rpc, rpc_args)).into(),

                Log {
                    verbosity,
                    file,
                    line,
                    human_time,
                } => with_tokio(log::log(args.common, verbosity, file, line, human_time)).into(),

                Peers { command } => {
                    with_tokio(peers::peers(args.common, command.unwrap_or_default())).into()
                }

                Ping {
                    typ,
                    resolve,
                    count,
                    length,
                    pattern,
                    dest,
                    verbose,
                } => with_tokio(ping::ping(
                    args.common,
                    typ,
                    resolve,
                    count,
                    length,
                    pattern,
                    verbose,
                    dest,
                ))
                .into(),

                Route { command } => with_tokio(route::route(args.common, command)).into(),

                Session { command } => {
                    with_tokio(session::session(args.common, command.unwrap_or_default())).into()
                }

                Util { command } => util::util(command).into(),
            }
        }
        Err(err) => err.into(),
    }
}

enum MainResult {
    Success,
    ArgParseError(clap::error::Error),
    RuntimeError(eyre::Error),
}

impl From<clap::error::Error> for MainResult {
    fn from(value: clap::error::Error) -> Self {
        MainResult::ArgParseError(value)
    }
}

impl From<eyre::Error> for MainResult {
    fn from(value: eyre::Error) -> Self {
        MainResult::RuntimeError(value)
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

            Self::ArgParseError(err) => {
                err.print().expect("Failed to print diagnostic message");
                ExitCode::from(err.exit_code() as u8)
            }

            Self::RuntimeError(err) => {
                let exe = common::utils::exe_name();
                eprintln!("{exe}: {err:?}"); // Assuming `install_eyre_hook` succeeds
                ExitCode::FAILURE
            }
        }
    }
}

fn install_eyre_hook() -> eyre::Result<()> {
    let mut builder = HookBuilder::new();
    if !stderr().is_terminal() {
        builder = builder.theme(Theme::new());
    }
    builder.install()
}

fn with_tokio<T, E, F>(future: F) -> Result<T, E>
where
    E: From<io::Error>,
    F: 'static + Future<Output = Result<T, E>> + Send + Sync,
{
    let rt = Runtime::new()?;
    rt.block_on(future)
}
