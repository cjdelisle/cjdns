## Upcoming

If anybody wants to get a leg up on an upcoming feature:

check out the new [Iface.h](https://github.com/cjdelisle/cjdns/blob/ducttape2/interface/Iface.h) which will replace the current `Interface.h`.

It features a manual tail call optimization which will likely be reused throughout the codebase, so we should probably make a big deal of it.

See [the flag used to turn on the optimization](https://github.com/cjdelisle/cjdns/blob/ducttape2/interface/Iface.h#L65).

