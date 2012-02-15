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

my $lineInfo;
my $ignore;

my $header = <<EOF;
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
    my $lineNum = 1;

    foreach my $line (split("\n", <FILE>)) {
        $lineNum++;

        if ($lineNum < @headerArray.length) {
            my $expectedLine = @headerArray[$lineNum - 2];
            if (index($line, $expectedLine) == -1) {
                error("missing header\n$expectedLine\n$line");
            }
        }

        $ignore = ($line =~ /CHECKFILES_IGNORE/);

        if (!($fileName =~ /_test/)) {
            if ($line =~ /^\w+\s.*\(/) {
                if (!($line =~ /^int main\(/
                    || $line =~ / ${name}_/
                    || $line =~ /^[ ]?static /))
                {
                    error("all globally visible functions must begin with the name of the file.");
                }
            }
        }

        $lineInfo = "$fileName:$lineNum";
        if ($fileName =~ /\.h$/) {

            if ($line =~ /^struct / && !($line =~ /^struct ${name}/) && !($line =~ /\(/)) {
                error("all structures must begin with the name of the file.");
            }

            if ($line =~ /#define / && $line !~ /#define $name/) {
                error("all defines must begin with the name of the file.");
            }
        }
        if ($line =~ /\t/) {
            error("tabs are not allowed, use 4 spaces.");
        }

        if ($line =~ /\s$/) {
            error("trailing whitespace.");
        }
    }
}
