function FindProxyForURL(url, host) {
    // If we can't resolve this means that this host is IPv4 only. Trying cjdns won't hurt.
    // It also make IPv4 only hosts connect ot clearnet IPv6 hosts if proxy is IPv6 capable.
    // If we can resolve check if the host is insice cjdns network space.
    if (shExpMatch(host, "fc*:*") || !dnsResolve(host) || shExpMatch(dnsResolve(host), "fc*:*")) {
        return "SOCKS5 localhost:8080";
    }
    return "DIRECT";
}