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

var Crypto = require('crypto');
var seed = process.env.SOURCE_DATE_EPOCH || Crypto.randomBytes(32).toString('hex');

if (process.env.SOURCE_DATE_EPOCH) { console.log('\n\n\nYES\n\n\n');}

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

var randomHex = module.exports.randomHex = function (bytes, file) {
    if (!file) { throw new Error('file unspecified'); }
    var nonce = file.Constant_JS_nonce = (file.Constant_JS_nonce || 0) + 1;
    var material = new Crypto.Hash('sha512').update(seed).update(file.name).update(String(nonce)).digest();
    if (bytes > 64) { throw new Error("meh, randomHex of over 64 bytes is unimplemented"); }
    return material.slice(0, bytes).toString('hex');
};

var rand64 = module.exports.rand64 = function (file) {
    return '((uint64_t) 0x' + randomHex(64 / 8, file) + 'ull)';
};

var rand32 = module.exports.rand32 = function (file) {
    return '((uint32_t) 0x' + randomHex(32 / 8, file) + 'ul)';
};

var randHexString = module.exports.randHexString = function (lenStr, file) {
    return '"' + randomHex(lenStr / 2, file) + '"';
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
