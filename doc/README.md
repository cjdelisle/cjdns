# Docs

*Documenting things related to the meshnet*

[cjdns](https://github.com/hyperboria/cjdns) addresses a large number of complex issues in an elegant way. While efforts have been made to provide a default configuration that matches the greatest number of use cases, it is impractical to expect software to replace understanding.

> Enter the documentation project...

The good citizens of Hyperboria decided to put together this collection of documentation to help make the inner workings of cjdns more transparent.

We want you to [get involved](http://www.roaming-initiative.com/blog/posts/wtfm)! Feel free to submit pull requests.

See [index.md](index.md) for current progress. We'd love more languages!

Also have a look at [Prose for Programmers](https://github.com/joshuacc/prose-for-programmers), a work-in-progress book aimed at helping software developers write better prose.


## The scope of this project

We are interested in documenting a range of topics:

1. cjdns internals: bugs and features
2. operator culture and best practices
3. meshlocal resources
4. introductory literature


## Reporting bugs

[cjd](https://github.com/cjdelisle) would rather write code than troubleshoot trivial issues which often turn out to have resulted from user error. To make this easier, a few members of the community have offered to investigate issues on his behalf.

If you think you've found a bug, report it on [our fork's issue tracker](https://github.com/hyperboria/cjdns/issues). We'll try to gather up documentation related to the issue, to better understand where it might be coming from. Once we can narrow it down, and research some of the background details to that particular piece of functionality, we may be able to submit patches ourselves.

Anything you can submit that gets us closer to understanding some bug or function within cjdns is welcome. By starting an inquiry, you make it just a little bit easier for someone else to continue it. With that in mind, please read [this note on reporting bugs](bugs/reporting.md) and [our more general contribution policy](bugs/policy.md). Get involved!

## License

Unless otherwise noted, the documents contained within this `/doc/` directory are licensed under a Creative Commons Attribution 4.0 Unported License. By contributing documentation you warrant that you provide it under this license and that you have the right to do so.

See <[creativecommons.org/licenses/by/4.0/](https://creativecommons.org/licenses/by/4.0/)>

### Authors requiring Attribution

If you wish to receive attribution for your work as you are entitled under the CC-BY-SA, please add the name you under which you wish have your work recognized to the following list, if you do not wish to receive specific attribution you may signify this when you contribute by leaving this list as it is.

* Cjdns Developer Community

## For committers

Please remember that the canonical repo is at [gitboria.com/projectmeshnet/documentation](http://gitboria.com/projectmeshnet/documentation) in Hyperboria.
Pull from there, and push there first.
If the canonical repo and mirror at [github.com/hyperboria/docs](https://github.com/hyperboria/docs) diverge,
the canonical repo's respective branch shall be force-pushed to the mirror.


## Documentation in cjdns.git/doc

This repository is semi-regularly merged into cjdns' `doc/` directory.
It's useful to ship documentation with the code.
We use the following commands.

```sh
$ cd cjdns/
$ git remote add hyperboria git@github.com:hyperboria/cjdns.git
$ git remote add docs git@gitboria.com:projectmeshnet/docs.git
```

To merge documentation changes into cjdns,
we squash all new commits since the last merge into one commit,
and merge that commit into master.

```sh
$ git checkout master
$ git pull hyperboria master
$ git subtree pull --squash -P doc/ docs/master
$ git push hyperboria master
```

To merge documentation changes in cjdns back upstream into this repository,
we create a merge commit, and push it.

```
$ git subtree split -P doc/
38e3bc6f899de49213aed754c74046b9ae4a85d2
$ git push docs 38e3bc6f:master
```
