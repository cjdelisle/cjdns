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
#define AddressMapper_MAX_ENTRIES 128
struct AddressMapper
{
    uint64_t labels[AddressMapper_MAX_ENTRIES];
    uint8_t addresses[AddressMapper_MAX_ENTRIES][16];
    uint8_t accessNumber;
};

static inline struct AddressMapper* AddressMapper_new(struct MemAllocator* allocator)
{
    return allocator->calloc(sizeof(struct AddressMapper), 1, allocator);
}

/**
 * This is a very hot loop,
 * a large amount of code relies on this being fast so it is a good target for optimization.
 */
static inline int AddressMapper_indexOf(uint64_t label, struct AddressMapper* map)
{
    for (uint32_t i = 0; i < AddressMapper_MAX_ENTRIES; i++) {
        if (map->labels[i] == 0L) {
            break;
        } else if (map->labels[i] == label) {
            if (!(map->accessNumber % 8) && i > 0) {
                map->labels[i] = map->labels[i - 1];
                map->labels[i - 1] = label;
                uint8_t address[16];
                memcpy(address, map->addresses[i], 16);
                memcpy(map->addresses[i], map->addresses[i - 1], 16);
                memcpy(map->addresses[i - 1], address, 16);
                i--;
            }
            map->accessNumber++;
            return i;
        }
    }
    return -1;
}

/**
 * @param index the index of the element to remove.
 * @param map the map to remove from.
 * @return 0 if the entry is removed, -1 if it could not be found.
 */
static inline int AddressMapper_remove(int index, struct AddressMapper* map)
{
    if (index >= 0 && index < AddressMapper_MAX_ENTRIES) {
        for (int i = index + 1; i < AddressMapper_MAX_ENTRIES; i++) {
            if (map->labels[i] == 0L) {
                i--;
                memcpy(map->addresses[index], map->addresses[i], 16);
                map->labels[index] = map->labels[i];
                map->labels[i] = 0L;
                return 0;
            }
        }
        map->labels[index] = 0;
        return 0;
    }
    return -1;
}

static inline int AddressMapper_put(uint64_t label, uint8_t address[16], struct AddressMapper* map)
{
    int i;
    for (i = 0; i < AddressMapper_MAX_ENTRIES; i++) {
        if (map->labels[i] == 0L || map->labels[i] == label) {
            break;
        }
    }
    memcpy(map->addresses[i], address, 16);
    map->labels[i] = label;
    return i;
}
