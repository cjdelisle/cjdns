%if 0%{?rhel} == 6
%define __restorecon /sbin/restorecon
%endif

Name:           cjdns
# major version is cjdns protocol version:
Version:        17.1
Release:        2%{?dist}
Summary:        The privacy-friendly network without borders
Group:          System Environment/Base
License:        GPL3
URL:            http://hyperboria.net/
Source0: https://github.com/cjdelisle/cjdns/archive/%{name}-v%{version}.tar.gz
# Add targeted selinux policy
Patch0: cjdns.selinux.patch
# Allow python2.6 for build.  Python is not used during the build
# process.  The python tools allegedly depend on python2.7, but that can
# be in Requires for the subpackage.
Patch1: cjdns.el6.patch
# Fix RLIMIT_NPROC - setuid() bug.   In its low priv process, cjdroute calls 
#
#   setrlimit(RLIMIT_NPROC, &(struct rlimit){ 0, 0 })
#
# which on recent kernels prevents fork() or exec() after the following
# setuid().  This is due to changes discussed here:
#
# https://lwn.net/Articles/451985/
# 
# On the 2.6.32 kernel used by EL6, the above causes setuid() to fail.
# This patch sets RLIMIT_NPROC to { 1, 1 } instead, which prevents
# fork(), but not exec.
Patch2:  cjdns.nprocs.patch
# Add setgid to Security_setUser().
Patch3:  cjdns.setgid.patch
# Change defaults generated by cjdroute --genconf
Patch4:  cjdns.genconf.patch
# Patch contributed init scripts to put cjdroute in /usr/sbin
Patch5:  cjdns.sbin.patch
# Patch make.js to use dynamic nacl library
Patch6:  cjdns.dyn.patch

# FIXME: selinux prevents cjdroute from writing to /etc/cjdroute.conf
# at first startup when run by the supplied init service scripts.  This is
# probably correct.  But the initial config needs to be generated another way.

BuildRequires:  nodejs, make

%if 0%{?rhel} != 6
# x86_64 and ARM libnacl are not compiled with -fPIC before f24, and cannot be
# used, but we require nacl-devel anyway, since it will be used in f24, and
# possibly f23.
BuildRequires:  nacl-devel
%endif
Requires(pre): shadow-utils

%description
Cjdns implements an encrypted IPv6 network using public-key cryptography for
address allocation and a distributed hash table for routing. This provides
near-zero-configuration networking, and prevents many of the security and
scalability issues that plague existing networks.

%package selinux
Summary: Targeted SELinux policy module for cjdns
Group: System Environment/Base
BuildRequires: policycoreutils, checkpolicy, selinux-policy-devel
Requires: policycoreutils, selinux-policy, %{name}

%description selinux
Targeted SELinux policy module for cjdns.

# FIXME: keep C tools separate?
%package tools
Summary: Command line tools for cjdns
Group: System Environment/Base
Requires: nodejs

%description tools
C language and nodejs tools for cjdns.

%package python
Summary: Python tools for cjdns
Group: System Environment/Base
Requires: python

%description python
Python tools for cjdns.

%package graph
Summary: Python tools for cjdns
Group: System Environment/Base
Requires: cjdns-python, python-networkx

%description graph
Python graphing tools for cjdns.

%prep
%setup -qn cjdns-%{name}-v%{version}
%patch0 -b .selinux
%if 0%{?rhel} == 6
%patch1 -b .el6
%patch2 -b .nprocs
%endif
#patch3 -b .setgid
%patch4 -b .genconf
%patch5 -b .sbin

# use system nacl library if provided.  
# nacl-devel on x86_64 and ARM fails to provide PIC object, so we have to use
# embedded nacl.  
if test -x %{_libdir}/libnacl.so; then
%patch6 -b .dyn
%ifarch i686
elif test -d %{_includedir}/nacl && test -r %{_libdir}/libnacl.a; then
  cd node_build/dependencies
  mv cnacl cnacl.embedded
  mkdir -p cnacl/jsbuild
  ln -s %{_libdir}/libnacl.a cnacl/jsbuild
  ln -s %{_includedir}/nacl cnacl/jsbuild/include
%endif
fi

# FIXME: grep Version_CURRENT_PROTOCOL util/version/Version.h and
# check that it matches major %%{version}

%build
cd contrib/selinux
ln -s /usr/share/selinux/devel/Makefile .
make 
cd -
# nodejs based build system
./do

# FIXME: use system libuv on compatible systems
# bundled libuv is 0.11.4 with changes:
# https://github.com/cjdelisle/cjdns/commits/master/node_build/dependencies/libuv

# FIXME: use system nacl, which is the same version (20110221)
# does bundled cnacl include any patches?

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT%{_sbindir}
cp -p cjdroute $RPM_BUILD_ROOT%{_sbindir}

# init support
# FIXME: move cjdroute to /usr/sbin or /usr/lib/cjdns and patch init configs
%if 0%{?rhel} == 6
mkdir -p $RPM_BUILD_ROOT/%{_sysconfdir}/init
cp -p contrib/upstart/cjdns.conf $RPM_BUILD_ROOT/%{_sysconfdir}/init
%else
mkdir -p $RPM_BUILD_ROOT%{_unitdir}
cp -p contrib/systemd/cjdns.service $RPM_BUILD_ROOT%{_unitdir}
%endif

# chroot 
mkdir -p %{buildroot}/var/empty/cjdns

# install selinux modules
mkdir -p %{buildroot}%{_datadir}/selinux/targeted
cp -p contrib/selinux/cjdns.pp %{buildroot}%{_datadir}/selinux/targeted
ln -f contrib/selinux/cjdns.{te,fc} .  # for doc dir

# install c and nodejs tools
mkdir -p %{buildroot}%{_libdir}/cjdns/{node_build,contrib}
cp -p publictoip6 privatetopublic makekeys randombytes sybilsim \
        $RPM_BUILD_ROOT%{_libdir}/cjdns
cp -pr tools $RPM_BUILD_ROOT%{_libdir}/cjdns
cp -p node_build/Semaphore.js $RPM_BUILD_ROOT%{_libdir}/cjdns/node_build
cp -pr contrib/nodejs $RPM_BUILD_ROOT%{_libdir}/cjdns/contrib

mkdir -p %{buildroot}%{_bindir}
# symlinks for selected nodejs tools
for t in peerStats sessionStats cjdnslog search dumpLinks dumptable \
         dumpRumorMill pathfinderTree pingAll; do
  ln -sf %{_libdir}/cjdns/tools/$t %{buildroot}%{_bindir}
done

# symlinks for selected C tools
for t in publictoip6; do
  ln -sf %{_libdir}/cjdns/$t %{buildroot}%{_bindir}
done


# install python tools that pull in networkx for graphing
cp -pr contrib/python $RPM_BUILD_ROOT%{_libdir}/cjdns
rm $RPM_BUILD_ROOT%{_libdir}/cjdns/python/README.md
rm $RPM_BUILD_ROOT%{_libdir}/cjdns/python/cjdns-dynamic.conf

# symlink python tools w/o conflict with nodejs tools or needing networkx
for t in pingAll.py trashroutes \
         getLinks ip6topk pktoip6 cjdnsa searches findnodes; do
  ln -sf %{_libdir}/cjdns/python/$t %{buildroot}%{_bindir}
done

# symlink python tools that pull in networkx for graphing
for t in drawgraph dumpgraph graphStats; do
  ln -sf %{_libdir}/cjdns/python/$t %{buildroot}%{_bindir}
done

%files
%defattr(-,root,root,-)
%{!?_licensedir:%global license %%doc}
%license LICENSE
%doc README.md README_RU.md HACKING.md 
%attr(0100,root,root) /var/empty/cjdns
%{_sbindir}/cjdroute
%dir %{_libdir}/cjdns
%if 0%{?rhel} == 6
%{_sysconfdir}/init/*
%else
%{_unitdir}/*
%endif

%pre
getent group cjdns > /dev/null || groupadd -r cjdns
getent passwd cjdns > /dev/null || /usr/sbin/useradd -g cjdns \
	-c "End to end encrypted IPv6 mesh" \
        -r -d %{_libdir}/cjdns -s /sbin/nologin cjdns
exit 0

# FIXME: how do you get upstart on EL6 to reload the service?
%post
%if 0%{?rhel} != 6
%systemd_post cjdns.service
%endif

%preun
%if 0%{?rhel} == 6
if [ "$1" -eq 0 ]; then
  /sbin/initctl stop cjdns
fi
%else
%systemd_preun cjdns.service
%endif

%postun
%if 0%{?rhel} == 6
if [ "$1" -ge 1 ]; then
  /sbin/initctl restart cjdns
fi
%else
%systemd_postun_with_restart cjdns.service
%endif

%files selinux
%defattr(-,root,root,-)
%doc cjdns.te cjdns.fc 
%{_datadir}/selinux/targeted/*

%post selinux
/usr/sbin/semodule -s targeted -i %{_datadir}/selinux/targeted/cjdns.pp \
        &>/dev/null || :
%{__restorecon} %{_sbindir}/cjdroute

%postun selinux
if [ $1 -eq 0 ] ; then
/usr/sbin/semodule -s targeted -r cjdns &> /dev/null || :
fi

%files tools
%defattr(-,root,root,-)
%dir %{_libdir}/cjdns
%{_libdir}/cjdns/contrib
%{_libdir}/cjdns/tools
%{_libdir}/cjdns/node_build
%{_libdir}/cjdns/publictoip6
%{_libdir}/cjdns/makekeys
%{_libdir}/cjdns/randombytes
%{_libdir}/cjdns/sybilsim
%{_libdir}/cjdns/privatetopublic
%{_bindir}/peerStats
%{_bindir}/sessionStats
%{_bindir}/cjdnslog
%{_bindir}/dumpRumorMill
%{_bindir}/pathfinderTree
%{_bindir}/dumptable
%{_bindir}/pingAll
%{_bindir}/search
%{_bindir}/publictoip6

%files python
%doc contrib/python/README.md contrib/python/cjdns-dynamic.conf
%defattr(-,root,root,-)
%dir %{_libdir}/cjdns/python
%{_libdir}/cjdns/python/cexec
%{_libdir}/cjdns/python/cjdnsadminmaker.py*
%{_libdir}/cjdns/python/cjdnslog
%{_libdir}/cjdns/python/dumptable
%{_libdir}/cjdns/python/dynamicEndpoints.py*
%{_libdir}/cjdns/python/peerStats
%{_libdir}/cjdns/python/sessionStats
%{_libdir}/cjdns/python/cjdnsadmin
%{_libdir}/cjdns/python/pingAll.py*
%{_libdir}/cjdns/python/trashroutes
%{_libdir}/cjdns/python/getLinks
%{_libdir}/cjdns/python/ip6topk
%{_libdir}/cjdns/python/pktoip6
%{_libdir}/cjdns/python/cjdnsa
%{_libdir}/cjdns/python/searches
%{_libdir}/cjdns/python/findnodes
%{_bindir}/dumpLinks
%{_bindir}/pingAll.py
%{_bindir}/trashroutes
%{_bindir}/getLinks
%{_bindir}/ip6topk
%{_bindir}/pktoip6
%{_bindir}/cjdnsa
%{_bindir}/searches
%{_bindir}/findnodes

%files graph
%defattr(-,root,root,-)
%{_libdir}/cjdns/python/drawgraph
%{_libdir}/cjdns/python/dumpgraph
%{_libdir}/cjdns/python/graphStats
%{_bindir}/drawgraph
%{_bindir}/dumpgraph
%{_bindir}/graphStats

%changelog
* Wed Nov 04 2015 Stuart D. Gathman <stuart@gathman.org> 17.1-2
- use dynamic nacl library backported from rawhide
- TODO: generate default config at install time, not first start

* Tue Nov 03 2015 Stuart D. Gathman <stuart@gathman.org> 17.1-1
- update to new protocol version

* Tue Oct 27 2015 Stuart D. Gathman <stuart@gathman.org> 16.3-2
- move graphing tools to graph subpackage: networkx has a lot of dependencies.
- use embedded nacl only for i686 (which compiles it with -fPIC)

* Fri Oct 16 2015 Stuart D. Gathman <stuart@gathman.org> 16.3-1
- Allow hostname lookup in selinux policy
- python tools subpackage

* Sun Oct  4 2015 Stuart D. Gathman <stuart@gathman.org> 16.0-6
- restorecon after selinux install to initialize cjdroute context
- remove module_request (to load tun driver) from selinux policy
- make init scripts load tun driver

* Sun Sep 27 2015 Stuart D. Gathman <stuart@gathman.org> 16.0-5
- Restart cjdroute on update, stop on uninstall
- symlink selected tools to bin
- use /var/empty/cjdns for chroot
- patch genconf to change chroot and setuser

* Wed Sep 23 2015 Stuart D. Gathman <stuart@gathman.org> 16.0-4
- Remove doc subpackage - only a meg of docs, and protocol is experimental.
- Fix for RLIMIT_NPROC - setuid bug.
- Add setgid to Security.c
- add contrib/nodejs so tools work

* Wed Sep 23 2015 Stuart D. Gathman <stuart@gathman.org> 16.0-3
- Add selinux, doc and tools subpackages
- Support EL6

* Mon Sep 21 2015 Stuart D. Gathman <stuart@gathman.org> 16.0-2
- nodejs not a runtime dependency of main package
- move binaries to /usr/bin (good idea?)

* Mon Sep 21 2015 Stuart D. Gathman <stuart@gathman.org> 16.0-1
- Initial RPM
