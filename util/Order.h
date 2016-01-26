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
#ifndef Order_H
#define Order_H

#include "util/Linker.h"
Linker_require("util/Order.c");

#include <stddef.h>
typedef int (* Order_Comparator)(const void* a, const void* b);
void Order_qsort(void* base, size_t num, size_t size, Order_Comparator compare);
#endif

#ifdef Order_TYPE
#ifndef Order_NAME
    #error must give this ordering a name by defining Order_NAME
#endif

#ifndef Order_COMPARE
    #error must define a comparitor
#endif

#define Order_FUNCTION(name) Order_GLUE(Order_GLUE(Order_GLUE(Order_, Order_NAME),_), name)
#define Order_GLUE(x, y) Order_GLUE2(x, y)
#define Order_GLUE2(x, y) x ## y

static inline int Order_COMPARE(const Order_TYPE* one, const Order_TYPE* two);

static inline void Order_FUNCTION(qsort)(Order_TYPE* array, int size)
{
    Order_qsort(array, size, sizeof(Order_TYPE), (Order_Comparator) Order_COMPARE);
}

#undef Order_TYPE
#undef Order_NAME
#undef Order_COMPARE
#undef Order_FUNCTION
#undef Order_GLUE
#undef Order_GLUE2

#endif
