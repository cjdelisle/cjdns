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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef UniqueName_H
#define UniqueName_H

#ifdef __INTELLISENSE__
#define UniqueName_get() UniqueName
#define UniqueName_last() UniqueName
#else
<?js file.Constant_JS = file.Constant_JS || require('util/Constant.js'); ?>

#define UniqueName_get() <?js return \
    file.UniqueName_last = 'UniqueName_' + \
        JSON.parse(file.Constant_JS.randHexString(20, file)); \
?>

#define UniqueName_last() <?js return file.UniqueName_last; ?>
#endif
#endif
