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
#ifndef AuthorizedPasswords_H
#define AuthorizedPasswords_H

#include "admin/Admin.h"
#include "crypto/CryptoAuth.h"
#include "memory/Allocator.h"
#include "util/Linker.h"
Linker_require("admin/AuthorizedPasswords.c");

/**
 * Init the AuthorizedPasswords admin function.
 * This function exports the following calls,
 *   AuthorizedPasswords_add() --- Add an authorized password routers to connect.
 *     - requires a user and a password.
 *     - inputs
 *         user: (String, mandatory)
 *             The password to authorize.
 *         password: (String, mandatory)
 *             The password to authorize.
 *         authType: (Integer, optional)
 *             The way in which nodes will be allowed to authenticate with
 *             the password.
 *
 *     - outputs
 *         error: (String)
 *             A message explaining what went wrong, if everything went ok,
 *             the error will be "none".
 *
 *   AuthorizedPasswords_remove() --- Remove an authorized password.
 *     - requires a user
 *     - inputs
 *         user: (String, mandatory)
 *             The password to authorize.
 *     - outputs
 *         error: (String)
 *             A message explaining what went wrong, if everything went ok,
 *             the error will be "none".
 *
 * @param admin the admin object.
 * @param ca the cryptoauth to add users to.
 * @param allocator a persistent memory allocator.
 */
void AuthorizedPasswords_init(struct Admin* admin,
                              struct CryptoAuth* ca,
                              struct Allocator* allocator);

#endif
