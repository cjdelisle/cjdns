# Fedora Packaging

This is a place for packaging designed for the Fedora family of linux distros,
including EPEL. Fedora uses RPM packages, which are built from pristine source
after applying a set of patches. The pristine source for cjdns is pulled from
the github tagged versions for cjdelisle/cjdns. Patches that are generally
applicable will also be submitted as a pull request on github. Other patches,
such as changing genconf to use a unique chroot dir and unprivileged user, will
remain as patch files.  (Although cjd suggests adding cli options to cjdroute
to enable this generally.)

Note that patch files are applied against a specific tagged release of cjdns,
e.g. v16.3 or v17.1.  Patch files can often be used with a later version, but a
new patch for the later version is sometimes needed.  Some patches, like
setgid, are obsolete for current cjdns versions - but are kept for building
older releases. 
