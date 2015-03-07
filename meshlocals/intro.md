# Mesh Local Intro

## What is a Mesh Local?

A 'Mesh Local' is a mesh network situated in a particular geographic locale, focused on providing a community-centric alternative (or complement) to the internet.

## How local is local?

Project Meshnet's mesh networking protocol (cjdns) was designed to be agnostic of the medium over which it communicates. That means that even if the closest node is beyond the functional range of radio signals, you can still peer with it over the internet.

While this is useful for connecting with others who are interested, and for forming a seed from which a local network can grow, it relies on another network to exist. The ultimate goal is to create a net which will not fail if the internet does.

<div class="mermaid">
    graph LR;
        A(Local node)--\>|wireless lan cable|B(Other local node);
        B--\>A;
</div>

**NOTE**: If you're viewing this on a web page which does not include [mermaid.js](https://github.com/knsv/mermaid), you'll just see some weird text. For an example of how it **should** look, see it in [its original context](https://docs.meshwith.me/en/meshlocals/intro.html).

## What if physical peering is out of the question?

If you're located somewhere extremely remote, you don't have to count yourself out of the picture. Try to gather some resources for a meshlocal anyway, as you might be pleasantly surprised to find someone nearby despite what you may have thought. That means letting people know that you're working on it (at least for yourself).

Read as much documentation as you can, and familiarize yourself with the software so that if someone does contact you, you have some knowledge to share. If you find any documentation that's unclear or misleading, get in touch with us and help to [write the fine manual](http://roaming-initiative.com/blog/posts/wtfm).

Finally, ask around for someone willing to peer with you despite the distance, but be patient. As time goes by and you find closer peers, disable the connections to those who are further away.

## About Wifi tuning

Antenna directionality, Signal to Noise ratio, beamforming: http://apenwarr.ca/log/?m=201408#01

A guide to tuning your wifi for adverse conditions: http://kmkeen.com/linux-wifi/
