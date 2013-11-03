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
#include <arm_neon.h>

/**
 * Very basic test which exercises an ARM NEON instruction and will fail if NEON is unavailable.
 */
int main(int argc, char** argv)
{
    uint32x2_t xx = vld1_u32((uint32_t*)&argc);
    uint32_t output = ~0;
    vst1_u32(&output, xx);
    return (output == ~0) ? 100 : 0;
}
