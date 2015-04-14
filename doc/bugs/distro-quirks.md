# OS/distribution-specific quirks

## OSX

### Failure to autopeer

```IRC
16:46 < ansuz> sounds like you guys are in srs mode, but that mac no-auto-peering thing only really has one line of documentation
16:46 < ansuz> at some point if someone could fill me in on the why, it would get more
16:47 <@cjd> ahh, it's because generating ethernet frames is different per operating system
16:47 <@cjd> there's no standardized socket(SO_LINK_LAYER, "eth0")
16:48 <@cjd> so ETHInterface_darwin.c just isn't written
16:49 <@cjd> now you can copy/paste that into a little txt file and when some poor bastard asks why, tell him and then shout at him to update the documentation
16:49 <@cjd> and if he doesn't, shitlist him so no more questions \:D/
16:49 <@Arceliar> or bug people who own a mac to write ETHInterface_darwin.c
```

There you have it, Macs don't autopeer via ethernet frames.
