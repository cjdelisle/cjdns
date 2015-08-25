Proxying
===========================

#Keep in mind that we'd rather you put cjdns on all your machines!

Sometimes it's just not possible to get cjdns working on a machine.

The easiest way to access Hyperboria if you can't get access on your machine is through a box that already has access.
This can be simply done using a SSH connection.

Creating SOCKSv5 connection
----------------------------

To create SOCKS SSH connection in Linux/OSX:

```bash
ssh -f -N -D 8080 [your_cjdns_capable_host]
```
This will connect to host under `[your_cjdns_capable_host]`.

This can be also achieved in PuTTY by creating a dynamic tunnel on port `8080`.

(The port `8080` can be any different port but you will have to edit PAC later to make it work)

Configuring your Web Browser
----------------------------

If proxying all of your traffic through the server is acceptable to you, then just select SOCKS5 proxy
and put `localhost` as hostname and `8080` as port.

If you want your normal traffic to travel directly and only cjdns traffic to connect though the proxy,
you have to use Proxy auto-config (PAC).

The following PAC file checks if the host you are connecting to is a cjdns IPV6 address, if not tries to resolve it. (If that fails, it usually means current DNS server is not IPv6 capable, so it redirects the traffic through the proxy)Finally, if resolving via DNS is successful (and the current host is IPv6 capable) it checks whether the address is in cjdns space.

```js
function FindProxyForURL(url, host) {
    // If we can't resolve this means that this host is IPv4 only. Trying cjdns won't hurt.
    // It also make IPv4 only hosts connect ot clearnet IPv6 hosts if proxy is IPv6 capable.
    // If we can resolve check if the host is insice cjdns network space.
    if (shExpMatch(host, "fc*:*") || !dnsResolve(host) || shExpMatch(dnsResolve(host), "fc*:*")) {
        return "SOCKS5 localhost:8080";
    }
    return "DIRECT";
}
```
Also available as a file in this repo: [cjdns.pac](/cjdns.pac)

This PAC file uses SOCKS5 connection with proxy server on `localhost` at port `8080`.
If you'd like to use a different port, you have to change it accordingly.

Use this PAC file for `Automatic Proxy Configuration` option which is available in most browsers and other proxy management solutions.
