## Private Keys

* Private Keys are represented internally as 32 element Uint8 Arrays.
* Represented in text form as 64 digits of hexadecimal.
* cjdns does not currently require anything more than the private key at launch time. It derives the public key and ipv6 after loading the private key to avoid errors.

## Public Keys

* The public key is entirely dependent on the privateKey.
* Represented internally as another 32 element Uint8 Array.
* Represented in text form as a 52 character string of [cjdns' base32 encoding](https://github.com/ansuz/cjdnsjs/blob/master/scripts/keys/cjdb32.js).
* cjdns base32 consists of the following characters:
  + 0123456789bcdfghjklmnpqrstuvwxyz
* Any string matching the following regex is **invalid** cjdns base32
  + /[^0-9b-df-hj-np-z]/

## IPV6 Addresses

* An ipv6 is the hash of the hash of the public key, encoded in hexadecimal, and trimmed to the first 32 characters, with colons inserted between every two byte section.

## Passwords

* Password are (nearly) arbitrary text strings.
* Their length is variable.
* There are only a few banned characters:
  + newlines (\n)
  + quotation marks (")
  + backticks (`)
  + backslashes (\)
  + spaces ( )
  + tabs (\t)
  + curly braces ({,})
  + `<@cjd> be careful of non a-zA-Z0-9_`
* There may be more invalid characters, please let us know if you find any!
* They can be generated in bash with the following one-liner:

```Bash
cat /dev/urandom | strings | head -n 50 | tr -d '\n"`\\ \t'  | head -c 50 && echo OR tr -cd '[:alnum:]' < /dev/urandom | fold -w32 | head -n20`
```

* On OpenWrt, the following shorter one-liner is used:

```Bash
tr -cd 'A-Za-z0-9' 2> /dev/null < /dev/urandom | head -n 20
```
