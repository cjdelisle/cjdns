# Cjdns Core Notes

## Introduction

A number of us have begun the task of documenting and reverse engineering the cjdns core. The "core" refers to the core functionality required to implement a cjdns-compatible router.

To speak with those involved, join **HypeIRC/#documentation**

## Status

ansuz has re-implemented the [xor metric](https://github.com/ansuz/cjdnsjs/tree/master/scripts/xor) and [key generation process](https://github.com/ansuz/cjdnsjs/tree/master/scripts/keys), along with the support functions involved, notably the implementation of Base32 used by cjdns.

[jph](https://hackworth.be/) is experimenting with writing a minimal, linux first implementation in golang, along with deconstructing everything involved in making that happen. He is also investigating TUN support in other languages (notably Python and ocaml).

**Note to contributors**: 

* If you think of additional core functions to document, please add them below  
* Please provide either pseudo-code or working code examples inline. Longer code examples should be linked to, and preferably hosted on gitboria

# TUN device support

## Overview

Cjdns uses [TUN devices](https://www.kernel.org/doc/Documentation/networking/tuntap.txt) to create a virtual network interface and enable routable peer to peer connections. Therefore, TUN support is a critical requirement for implement a cjdns router. 

At this time, the following languages have built-in or third-party TUN support:

* C
* Golang
* Python
* Ruby
* Ocaml

Please note that "TUN support" may not have the full functionality required by a cjdns router. As this section is expanded, we will describe what that functionality is.

In addition, be aware that TUN implementation varies across operating system implementations. Multiplatform router implementations need to keep this in mind.

## Creating a TUN device

TBC

## Configuring a TUN device

TBC

# Cryptography

Cryptography provides core functionality within cjdns. [Here](cryptography.md) we will describe some of the cryptographic functions performed within cjdns and implementation details.

## Public Keys

* Cryptographic algorithms required
* Creating a private/public key pair
* Encoding private/public keys into a string
* Decoding private/public keys from a string
* Format for sharing public keys (peering credentials)

# Utility Functions

Instead of relying on external libraries, some utility functions have been re-implemented within cjdns. We will describe those functions that differ from standard implementations to ensure compatibility with existing cjdns routers.

## JSON

cjdns config files are typically stored as what the community often refers to as [cjdson](https://github.com/cjdson). As [finn](https://github.com/thefinn93) has pointed out, [comments](https://commentjson.readthedocs.org/en/latest/) [in json](https://www.npmjs.com/package/strip-json-comments) are [not unheard of](https://www.npmjs.com/package/json-comments).

In any case, we're going to try to add some functionality to that repo that the others lack, such as _inserting fields without deleting comments_.

## Base32

[This Javascript library](https://github.com/ansuz/cjdnsjs/blob/master/scripts/keys/cjdb32.js) is a direct translation of [the cjdns base32 encoder written in C](https://github.com/cjdelisle/cjdns/blob/master/util/Base32.h#L109)
