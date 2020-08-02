# Why is there a half a crypto library copy-pasted into cjdns?
> hysterical raisins

In the beginning cjdns didn't use signing at all, only encryption.
Then when the route server model was adopted, we needed to sign messages,
particularly we needed to know that a message was signed with a key
corrisponding to a particular cjdns IPv6 address.

Fortunately there exists a way to convert the same keys between the twisted
edwards curve (ed25519) used for signing and the this conversion is widely
supported and believed to be safe.

Unfortunately we needed to go the other way, from curve25519 keys to ed25519,
but since we have the private key at this stage, we can simply copy the
function which creates an ed25519 pubkey and everything is good. Except that
it isn't.

Ed25519 API is designed in such a way that it expects to be able to hash the
private key before usage, but if we're converting that key from a curve25519
key and expecting the public key to come out correct, we need to reimplement
the sign function to not hash the key before usage, so that's what we did.

My apologies to all of the security researchers out there who are now
gritting their teeth at the prospect of reviewing "home-made crypto", the
ugly is confined to
[Sign.c](https://github.com/cjdelisle/cjdns/blob/master/crypto/Sign.c) and
I've made efforts to explain what it's doing and why I believe it to be
equivilent to nacl/libsodium's `crypto_sign()`.