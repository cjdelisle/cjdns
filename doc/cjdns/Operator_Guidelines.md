# Operator Guidelines

When you become part of the network, you have become an ISP. This gives you certain responsibilities to the rest of the community.

---

### Lock Down Your Box

When you start up cjdns, you become an IPv6 host. Many programs, such as http, ssh, or smtp servers, will automatically bind to this new address. If you do not want these services to be available over your cjdns interface, you should either modify your `ip6tables` rules, or edit the configuration file of the service in question (a simple Google search can help with this for most programs).

To scan your box for running services, first get your cjdns address by running:

`$ ip addr | grep "inet6 fc"`

and then port scan that address with:

`$ nmap -6 <CJDNS ADDRESS>`

---

### Peer Efficiently

Murphy's Law applies to cjdns routing: if a packet can take a weird or inefficient route, it probably will. When you choose bad peers, it degrades the quality of the entire network. When you are peering over the internet, you should be peering with those who are in relative geographical proximity to you.

You should only be peering with people whom you (moderately) trust. Peering with total strangers ruins the friend-of-a-friend nature of the network.

---

### Keep Peering Credentials Safe

On the same lines as peering efficiently, be careful that nobody steals your peering credentials.

The passwords should not be passwords, they should be long strings of gibberish that are practically impossible for an attacker to guess. To generate a strong password, you can run this bash command:

```
$ cat /dev/urandom | strings | head -n 20 | tr -d '\n"`\ \t{}' | head -c 40 && echo

```

DO NOT transfer peering credentials over any sort of plaintext medium, such as IRC or unencrypted email. It's probably also a good idea to use a service like [EZCrypt](https://ezcrypt.it) (also available [on Hyperboria](http://cjdns.ezcrypt.it)) or send your peering details via encrypted email with PGP.

---

### Get on IRC!

As a service provider, you need to be available in case someone needs you.
