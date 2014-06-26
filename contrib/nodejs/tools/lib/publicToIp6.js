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

var Crypto = require('crypto');

var numForAscii = [
    99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9,99,99,99,99,99,99,
    99,99,10,11,12,99,13,14,15,99,16,17,18,19,20,99,
    21,22,23,24,25,26,27,28,29,30,31,99,99,99,99,99,
    99,99,10,11,12,99,13,14,15,99,16,17,18,19,20,99,
    21,22,23,24,25,26,27,28,29,30,31,99,99,99,99,99,
];

// see util/Base32.h
var Base32_decode = function (input) {
    var output = [];
    var outputIndex = 0;
    var inputIndex = 0;
    var nextByte = 0;
    var bits = 0;

    while (inputIndex < input.length) {
        var o = input.charCodeAt(inputIndex);
        if (o & 0x80) { throw new Error(); }
        var b = numForAscii[o];
        inputIndex++;
        if (b > 31) { throw new Error("bad character " + input[inputIndex] + " in " + input); }

        nextByte |= (b << bits);
        bits += 5;

        if (bits >= 8) {
            output[outputIndex] = nextByte & 0xff;
            outputIndex++;
            bits -= 8;
            nextByte >>= 8;
        }
    }

    if (bits >= 5 || nextByte) {
        throw new Error("bits is " + bits + " and nextByte is " + nextByte);
    }

    return new Buffer(output);
};

var convert = module.exports.convert = function (pubKey) {
    if (pubKey.substring(pubKey.length-2) !== ".k") { throw new Error("key does not end with .k"); }
    keyBytes = Base32_decode(pubKey.substring(0, pubKey.length-2));
    var hashOneBuff = new Buffer(Crypto.createHash('sha512').update(keyBytes).digest('hex'), 'hex');
    var hashTwo = Crypto.createHash('sha512').update(hashOneBuff).digest('hex');
    var first16 = hashTwo.substring(0,32);
    var out = [];
    for (var i = 0; i < 8; i++) {
        out.push(first16.substring(i*4, i*4+4));
    }
    return out.join(':');
};

//console.log(convert('rjndc8rvg194ddf2j5v679cfjcpmsmhv8p022q3lvpym21cqwyh0.k'));
