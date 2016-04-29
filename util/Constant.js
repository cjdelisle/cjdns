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

var TABLE = {
    '0000': '0',
    '0001': '1',
    '0010': '2',
    '0011': '3',

    '0100': '4',
    '0101': '5',
    '0110': '6',
    '0111': '7',

    '1000': '8',
    '1001': '9',
    '1010': 'a',
    '1011': 'b',

    '1100': 'c',
    '1101': 'd',
    '1110': 'e',
    '1111': 'f'
};

var base2ToHex = function (numStr) {
    while ((numStr.length / 4) % 1) { numStr = '0' + numStr; }
    var out = [];
    for (var i = 0; i < numStr.length; i += 4) {
        out.push(TABLE[numStr.substring(i, i+4)]);
    }
    return out.join('');
};

var testBase2 = function () {
    for (var i = 0; i < 10000; i++) {
        var num = Math.random() * 4294967296;
        var b2 = num.toString(2);
        var numB = Number('0x' + base2ToHex(b2));
        if (num !== numB) { throw new Error(num + " --> " + b2 + " --> " + numB); }
    }
};

var base2 = module.exports.base2 = function (numStr) {
    var type = "uint8_t";
    if (numStr.length > 8) { type = "uint16_t"; }
    if (numStr.length > 16) { type = "uint32_t"; }
    if (numStr.length > 32) { type = "uint64_t"; }
    if (numStr.length > 64) { throw new Error("cannot have more than 64 bits"); }
    return '((' + type + ') 0x' + base2ToHex(numStr) + ((type === 'uint64_t') ? 'ull' : 'ul') + ')';
};

var randomHex = function (bytes) {
    var hex = '';
    var len = Number(bytes * 2); // One byte is two hex digits
    while (hex.length < len) { hex += Math.random().toString(16).substring(2); }
    return hex.substring(0,len);
};

var rand64 = module.exports.rand64 = function () {
    return '((uint64_t) 0x' + randomHex(64 / 8) + 'ull)';
};

var rand32 = module.exports.rand32 = function () {
    return '((uint32_t) 0x' + randomHex(32 / 8) + 'ul)';
};

var randHexString = module.exports.randHexString = function (lenStr) {
    return '"' + randomHex(lenStr / 2) + '"';
};

var log2 = module.exports.log2 = function (val) {
    var x = 1;
    for (var i = 0; i < 31; i++) {
        if (x === val) {
            if ((1 << i) !== val) { throw new Error(); }
            return i;
        }
        x = x + x;
    }
    throw new Error("not an even power of 2");
};

var stringForHex = module.exports.stringForHex = function (hex) {
    var out = [];
    for (var i = 0; i < hex.length; i+=2) {
        out.push(hex[i] + hex[i+1]);
    }
    if (out.length < 2) { throw new Error(); }
    return '"\\x' + out.join('\\x') + '"';
};

if (!module.parent) {
    console.log("testing " + __filename);
    testBase2();
}
