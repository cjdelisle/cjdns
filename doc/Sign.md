# Signing

Cjdns allows for signing and verification of messages using the RPC. To sign a message you need to send an authenticated RPC message to `Sign_sign()` RPC endpoint and to verify, you send a request (authentication not necessary) to `Sign_checkSig()`.

## Sign_sign()

`Sign_sign()` has 1 parameter:
* **msgHash**: `string`: is a short string (less than or equal to 64 bytes).

If you have a large message to sign, you should instead hash the message and sign the hash. `Sign_sign()` uses
the private key of the cjdns node to sign with.

Result:
* **error**: `string`: "none" or a string representing the error
* **signature**: `string` (optional): the signature in a printable format, missing if there is an error.

### Example of signing
```
user@underscore cjdns % ./tools/cexec 'Sign_sign("test message")'
{
  "error": "none",
  "signature": "0ytl2njc1hy86tlxtc2zc3449up47uqb0u04kcy233d7zrn2cwh1_y96duzwpvmslj8b7pnk2b32m0rhs738yujwtrtlcq81r0u114svygwn56phn9yncpyzhswpj3bd808lgd5bknlj8xwf7purl0r0hc30",
}
```

## Sign_checkSig()

`Sign_checkSig()` verifies a signature, it takes 2 parameters:
* **signature**: `string`: The signature produced by `Sign_sign()`
* **msgHash**: `string`: The original message hash that was signed.

Result:
* **error**: `string`: "none" or a string representing the error
* **ipv6**: `string` (optional): If the signature is valid, the cjdns IPv6 address of the signer.
* **pubkey**: `string` (optional): If the signature is valid, the pubkey of the signer.

### Example of signature verification
```
user@underscore cjdns % ./tools/cexec 'Sign_checkSig("0ytl2njc1hy86tlxtc2zc3449up47uqb0u04kcy233d7zrn2cwh1_y96duzwpvmslj8b7pnk2b32m0rhs738yujwtrtlcq81r0u114svygwn56phn9yncpyzhswpj3bd808lgd5bknlj8xwf7purl0
r0hc30", "test message")'
{
  "error": "none",
  "ipv6": "fca4:aa4c:3686:6a29:e301:89a5:942c:38d3",
  "pubkey": "hwnu9u7n8v9u7rjrflhsv45q16p103c1rfx9208hnzr2tq988z90.k",
  "txid": "575360523"
}
```

### Example with wrong message
```
user@underscore cjdns % ./tools/cexec 'Sign_checkSig("0ytl2njc1hy86tlxtc2zc3449up47uqb0u04kcy233d7zrn2cwh1_y96duzwpvmslj8b7pnk2b32m0rhs738yujwtrtlcq81r0u114svygwn56phn9yncpyzhswpj3bd808lgd5bknlj8xwf7purl0
r0hc30", "not the right message")'                                       
{                                                                                                     
  "error": "invalid signature",                                                      
  "txid": "828277386"
}
```

### Examples with corrupt signature
The last character of the signature was changed to an x
```
user@underscore cjdns % ./tools/cexec 'Sign_checkSig("0ytl2njc1hy86tlxtc2zc3449up47uqb0u04kcy233d7zrn2cwh1_y96duzwpvmslj8b7pnk2b32m0rhs738yujwtrtlcq81r0u114svygwn56phn9yncpyzhswpj3bd808lgd5bknlj8xwf7purl0
r0hc3x", "test message")'                                                
{                                                                        
  "error": "malformed signature, failed to decode signature",
  "txid": "1892499317"
}
```

The separator `_` was replaced with `+`
```
user@underscore cjdns % ./tools/cexec 'Sign_checkSig("0ytl2njc1hy86tlxtc2zc3449up47uqb0u04kcy233d7zrn2cwh1+y96duzwpvmslj8b7pnk2b32m0rhs738yujwtrtlcq81r0u114svygwn56phn9yncpyzhswpj3bd808lgd5bknlj8xwf7purl0
r0hc30", "test message")'                                                
{ 
  "error": "malformed signature, missing separator",
  "txid": "1193071367"
}
```

The first character of the signature was replaced with a `_`
```
user@underscore cjdns % ./tools/cexec 'Sign_checkSig("_ytl2njc1hy86tlxtc2zc3449up47uqb0u04kcy233d7zrn2cwh1_y96duzwpvmslj8b7pnk2b32m0rhs738yujwtrtlcq81r0u114svygwn56phn9yncpyzhswpj3bd808lgd5bknlj8xwf7purl0
r0hc30", "test message")'
{
  "error": "malformed signature, failed to decode pubkey",
  "txid": "2668454052"
}
```