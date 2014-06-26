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

#include "util/Bits.h"
#include "exception/Except.h"
#include "exception/WinFail.h"
#include "memory/Allocator.h"
#include "interface/tuntap/windows/TAPDevice.h"

/*
 * Portions of this code are copied from QEMU project which is licensed
 * under GPLv2 or greater, further contributions are licensed under GPLv3
 * or greater.
 */

/*
 *  TAP-Win32 -- A kernel driver to provide virtual tap device functionality
 *               on win32.  Originally derived from the CIPE-Win32
 *               project by Damion K. Wilson, with extensive modifications by
 *               James Yonan.
 *
 *  All source code which derives from the CIPE-Win32 project is
 *  Copyright (C) Damion K. Wilson, 2003, and is released under the
 *  GPL version 2 (see below).
 *
 *  All other source code is Copyright (C) James Yonan, 2003-2004,
 *  and is released under the GPL version 2 (see below).
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program (see the file COPYING included with this
 *  distribution); if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <windows.h>

#define NETWORK_ADAPTER_GUID "{4D36E972-E325-11CE-BFC1-08002BE10318}"

#define ADAPTER_KEY \
    "SYSTEM\\CurrentControlSet\\Control\\Class\\" NETWORK_ADAPTER_GUID

#define NETWORK_CONNECTIONS_KEY \
    "SYSTEM\\CurrentControlSet\\Control\\Network\\" NETWORK_ADAPTER_GUID

#define USERMODEDEVICEDIR "\\\\.\\Global\\"
#define TAPSUFFIX         ".tap"


static int is_tap_win32_dev(const char *guid)
{
    HKEY netcard_key;
    LONG status;
    DWORD len;
    int i = 0;

    status = RegOpenKeyEx(
        HKEY_LOCAL_MACHINE,
        ADAPTER_KEY,
        0,
        KEY_READ,
        &netcard_key);

    if (status != ERROR_SUCCESS) {
        return FALSE;
    }

    for (;;) {
        char enum_name[256];
        char unit_string[256];
        HKEY unit_key;
        char component_id_string[] = "ComponentId";
        char component_id[256];
        char net_cfg_instance_id_string[] = "NetCfgInstanceId";
        char net_cfg_instance_id[256];
        DWORD data_type;

        len = sizeof (enum_name);
        status = RegEnumKeyEx(
            netcard_key,
            i,
            enum_name,
            &len,
            NULL,
            NULL,
            NULL,
            NULL);

        if (status == ERROR_NO_MORE_ITEMS) {
            break;
        } else if (status != ERROR_SUCCESS) {
            return FALSE;
        }

        snprintf (unit_string, sizeof(unit_string), "%s\\%s",
                  ADAPTER_KEY, enum_name);

        status = RegOpenKeyEx(
            HKEY_LOCAL_MACHINE,
            unit_string,
            0,
            KEY_READ,
            &unit_key);

        if (status != ERROR_SUCCESS) {
            return FALSE;
        } else {
            len = sizeof (component_id);
            status = RegQueryValueEx(
                unit_key,
                component_id_string,
                NULL,
                &data_type,
                (uint8_t*)component_id,
                &len);

            if (!(status != ERROR_SUCCESS || data_type != REG_SZ)) {
                len = sizeof (net_cfg_instance_id);
                status = RegQueryValueEx(
                    unit_key,
                    net_cfg_instance_id_string,
                    NULL,
                    &data_type,
                    (uint8_t*)net_cfg_instance_id,
                    &len);

                if (status == ERROR_SUCCESS && data_type == REG_SZ) {
                    if (!Bits_memcmp(component_id, "tap", CString_strlen("tap")) &&
                        !strcmp (net_cfg_instance_id, guid)) {
                        RegCloseKey (unit_key);
                        RegCloseKey (netcard_key);
                        return TRUE;
                    }
                }
            }
            RegCloseKey (unit_key);
        }
        ++i;
    }

    RegCloseKey (netcard_key);
    return FALSE;
}

static int get_device_guid(
    char *name,
    int name_size,
    char *actual_name,
    int actual_name_size,
    struct Except* eh)
{
    LONG status;
    HKEY control_net_key;
    DWORD len;

    WinFail_check(eh, (
        RegOpenKeyEx(HKEY_LOCAL_MACHINE, NETWORK_CONNECTIONS_KEY, 0, KEY_READ, &control_net_key)
    ));

    int stop = 0;
    for (int i = 0; !stop; i++) {
        char enum_name[256];
        char connection_string[256];
        HKEY connKey;
        char name_data[256];
        DWORD name_type;
        const char name_string[] = "Name";

        len = sizeof (enum_name);
        status = RegEnumKeyEx(control_net_key, i, enum_name, &len, NULL, NULL, NULL, NULL);

        if (status == ERROR_NO_MORE_ITEMS) {
            break;
        } else if (status != ERROR_SUCCESS) {
            WinFail_fail(eh, "RegEnumKeyEx() failed", status);
        }

        if (len != CString_strlen(NETWORK_ADAPTER_GUID)) {
            // extranious directory, eg: "Descriptions"
            continue;
        }

        snprintf(connection_string,
             sizeof(connection_string),
             "%s\\%s\\Connection",
             NETWORK_CONNECTIONS_KEY, enum_name);

        WinFail_check(eh, (
            RegOpenKeyEx(HKEY_LOCAL_MACHINE, connection_string, 0, KEY_READ, &connKey)
        ));

        len = sizeof (name_data);

        WinFail_check(eh, (
            RegQueryValueEx(connKey, name_string, NULL, &name_type, (uint8_t*)name_data, &len)
        ));

        if (name_type != REG_SZ) {
            WinFail_fail(eh, "RegQueryValueEx() name_type != REG_SZ", status);
        }

        if (is_tap_win32_dev(enum_name)) {
            snprintf(name, name_size, "%s", enum_name);
            if (actual_name) {
                if (CString_strcmp(actual_name, "") != 0) {
                    if (CString_strcmp(name_data, actual_name) != 0) {
                        RegCloseKey (connKey);
                        ++i;
                        continue;
                    }
                }
                else {
                    snprintf(actual_name, actual_name_size, "%s", name_data);
                }
            }
            stop = 1;
        }

        RegCloseKey(connKey);
    }

    RegCloseKey (control_net_key);

    if (stop == 0) {
        return -1;
    }

    return 0;
}

struct TAPDevice* TAPDevice_find(const char* preferredName,
                                 struct Except* eh,
                                 struct Allocator* alloc)
{
    #define BUFF_SZ 0x100
    char guid[BUFF_SZ] = {0};
    char buff[BUFF_SZ] = {0};
    if (preferredName != NULL) {
        snprintf(buff, sizeof(buff), "%s", preferredName);
    }

    if (get_device_guid(guid, sizeof(guid), buff, sizeof(buff), eh)) {
        return NULL;
    }

    struct TAPDevice* out = Allocator_malloc(alloc, sizeof(struct TAPDevice));
    out->name = Allocator_malloc(alloc, CString_strlen(buff)+1);
    Bits_memcpy(out->name, buff, CString_strlen(buff)+1);

    snprintf(buff, sizeof(buff), USERMODEDEVICEDIR "%s" TAPSUFFIX, guid);

    out->path = Allocator_malloc(alloc, CString_strlen(buff)+1);
    Bits_memcpy(out->path, buff, CString_strlen(buff)+1);
    return out;
}
