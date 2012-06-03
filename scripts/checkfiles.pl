#!/usr/bin/perl
# You may redistribute this program and/or modify it under the terms of
# the GNU General Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

use strict;
use warnings;

my $lineInfo;
my $ignore;

my $header = <<EOF;
/* vim: set expandtab ts=4 sw=4: */
/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
EOF

my @headerArray = split("\n", $header);

sub error {
    if (!$ignore) {
        print $lineInfo . '  ' . $_[0] . "\n";
    }
};

undef $/;
my $files = <>;
foreach my $fileName (split("\n", $files))
{
    $fileName =~ /^.*\/(.*)\..*$/ or die;
    my $name = $1;
    open FILE, "$fileName" or die $!;
    my $lineNum = 0;
    my $parenthCount = 0;
    my $functionParenthCount = 0;
    my $expectBracket = 0;

    foreach my $line (split("\n", <FILE>)) {
        $lineNum++;
        $lineInfo = "$fileName:" . ($lineNum);

        if ($lineNum < scalar(@headerArray) + 1) {
            my $expectedLine = $headerArray[$lineNum - 1];
            if (index($line, $expectedLine) == -1) {
                error("missing header\n$expectedLine\n$line");
            }
        } elsif ($fileName =~ /\.h$/ && $lineNum < scalar(@headerArray) + 2) {
            if ($line !~ /^#ifndef ${name}_H$/) {
                error("expected #ifndef ${name}_H found ${line}");
            }
        } elsif ($fileName =~ /\.h$/ && $lineNum < scalar(@headerArray) + 3) {
            if ($line !~ /^#define ${name}_H$/) {
                error("expected #define ${name}_H found ${line}");
            }
        }

        $ignore = ($line =~ /CHECKFILES_IGNORE/);

        if ($expectBracket == 1) {
            $expectBracket = 0;
            if ($line !~ /^[\s]*{/) {
                error("expecting a { bracket " . $line);
            }
        }

        if (!($fileName =~ /_test/)) {
            my $n = $name;
            if ($name =~ /^W32(.*)$/) {
                $n = $1;
            }
            if ($line =~ /^\w+\s.*\(/) {
                if (!($line =~ /^int main\(/
                    || $line =~ / ${n}/
                    || $line =~ /^[ ]?static /))
                {
                    error("all globally visible functions must begin with the name of the file.");
                }
            }
        }

        if ($functionParenthCount > 0 || $line =~ /^\w+\s.*(\(.*)$/) {
            my $txt = ($functionParenthCount > 0) ? $line : $1;
            $functionParenthCount += (($txt =~ tr/(//) - ($txt =~ tr/)//));
            if ($functionParenthCount == 0) {
                $txt = substr($txt, rindex($txt, ')') + 1);
                if ($txt =~ /{/) {
                    error("please put the opening bracket on the next line.");
                }
            }
        }

        if ($line =~ /[\w]*int[\w]*\s+\*+\w/ || $line =~ /[\w]*struct\s+[\w]+\s+\*+\w/) {
            error("int* blah; means int pointer named blah, int *blah; means int names splatblah");
        }

        if (length($line) > 100) {
            error("cjd's editor window is only 100 characters wide");
        }

        if ($fileName =~ /\.h$/) {

            my $n = $name;
            # If the name is CryptoAuth_struct.h, it's ok to make a structure called CryptoAuth
            if ($name =~ /^(.*)_pvt$/) {
                $n = $1;
            }

            if ($line =~ /^struct / && !($line =~ /^struct ${n}/) && !($line =~ /\(/)) {
                error("all structures must begin with the name of the file.");
            }

            if ($line =~ /#define / && $line !~ /#define $n/) {
                error("all defines must begin with the name of the file.");
            }
        }
        if ($line =~ /\t/) {
            error("tabs are not allowed, use 4 spaces.");
        }

        if ($line =~ /\s$/) {
            error("trailing whitespace.");
        }

        if ($line =~ /(if|for|while)\(/) {
            error("If/for/while statements must be followed by whitespace.");
        }
        if ($parenthCount > 0 || $line =~ /[^\w#](if|for|while) (\(.*$)/) {
            my $txt = ($parenthCount > 0) ? $line : $2;
            $parenthCount += (($txt =~ tr/(//) - ($txt =~ tr/)//));
            if ($parenthCount == 0) {
                $txt = substr($txt, rindex($txt, ')') + 1);
                # for (x; y; z) ;
                # is not an unbracketed block.
                if ($txt !~ /^[\s]*;$/ && $txt !~ /^[\s]+{$/ && $txt !~ /^[\s]+{[\s]*\\$/) {
                    if ($txt =~ /[\s]*$/) {
                        $expectBracket = 1;
                    } else {
                        error($parenthCount . '  ' . $line);
                    }
                }
            }
        }
    }
}
