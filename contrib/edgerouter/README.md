# cjdns for Ubiquiti EdgeOS

This is a cjdns distributable package for Ubiquiti EdgeOS. It supports configuration through the standard configuration editor and, despite being in early stages, integrates quite well with the EdgeOS CLI.

It is maintained in a separate repository, please see [neilalexander/vyatta-cjdns](https://github.com/neilalexander/vyatta-cjdns) on GitHub. 

Currently tested models:

|                       | Architecture | Compatible |                      Notes                           |
|-----------------------|:------------:|:----------:|:----------------------------------------------------:|
|    EdgeRouter X (ERX) |    mipsel    |     Yes    | Builds on Debian Jessie with crossbuild-essential    |
| EdgeRouter Lite (ERL) |    mips64    |     Yes    | Builds on Debian Jessie with Codescape SDK as mips32 |

In addition to supporting Ubiquiti EdgeOS, the package should be very easily able to support VyOS and other Vyatta-based distributions with some minor changes, although that is not within the scope of this particular project.
