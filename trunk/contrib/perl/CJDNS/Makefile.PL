use 5.014002;
use ExtUtils::MakeMaker;
# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.
WriteMakefile(
    NAME              => 'CJDNS',
    VERSION_FROM      => 'lib/CJDNS.pm', # finds $VERSION
    PREREQ_PM         => {Bencode => 1.4, 'Digest::SHA2' => 1.1, JSON => 2.53}, # e.g., Module::Name => 1.1
    ($] >= 5.005 ?     ## Add these new keywords supported since 5.005
      (ABSTRACT_FROM  => 'lib/CJDNS.pm', # retrieve abstract from module
       AUTHOR         => 'Michael Gregorowicz <mike@mg2.org>') : ()),
);
