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

#include "util/log/Log.h"

#ifdef Illumos
    #define _XPG4_2
#endif
#include <strings.h>

#ifdef WIN32
    #define strcasecmp strcmp
#endif

char* Log_nameForLevel(enum Log_Level logLevel)
{
    switch (logLevel) {
        case Log_Level_KEYS:     return "KEYS";
        case Log_Level_DEBUG:    return "DEBUG";
        case Log_Level_INFO:     return "INFO";
        case Log_Level_WARN:     return "WARN";
        case Log_Level_ERROR:    return "ERROR";
        case Log_Level_CRITICAL: return "CRITICAL";
        default:                 return "INVALID";
    }
}

enum Log_Level Log_levelForName(char* name)
{
    for (enum Log_Level logLevel = Log_Level_KEYS; logLevel <= Log_Level_CRITICAL; logLevel++) {
        if (!strcasecmp(name, Log_nameForLevel(logLevel))) {
            return logLevel;
        }
    }
    return Log_Level_INVALID;
}
