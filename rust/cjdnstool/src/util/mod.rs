mod key2ip6;
mod keygen;
mod priv2pub;

use eyre::Result;
use clap::Subcommand;

pub async fn util(command: Command) -> Result<()> {
    use Command::*;
    match command {
        Key2Ip6 { pubkeys } => key2ip6::key2ip6(pubkeys).await,
        Priv2Pub { privkeys } => priv2pub::priv2pub(privkeys).await,
        Keygen => keygen::keygen().await,
    }
}

#[derive(Subcommand)]
#[command(rename_all = "lower")]
pub enum Command {
    /// Calculate IPv6 address(es) to be assigned to peer(s) depending on their public key(s).
    Key2Ip6 {
        /// One or more public keys to calculate IPv6 addresses from.
        #[arg(id = "PUBKEY", required = true)]
        pubkeys: Vec<String>,
    },

    /// Calculate the public key(s) which correspond to the given private key(s).
    Priv2Pub {
        /// One or more private keys to calculate their corresponding public keys.
        #[arg(id = "PRIVKEY", required = true)]
        privkeys: Vec<String>,
    },

    /// Generate a private-public key pair, and display it along with its IPv6 address.
    Keygen,
}
