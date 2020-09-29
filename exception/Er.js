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

const trim = (x) => {
    if (x[0] !== ' ' || x[x.length-1] !== ' ') {
        throw new Error("INTERNAL: Er input must begin " +
            "with and end with a space, unrecognized [" + x + "]");
    }
    return x.trim();
};

const defun = (ctx, spec) => {
    trim(spec);
    if (spec.lastIndexOf(')') !== spec.length - 2) {
        throw new Error("Er function spec must end " +
            "with a ), unrecognized [" + spec + "]");
    }
    let c = 1;
    let i = spec.length - 3;
    for (; c && i >= 0; i--) {
        c += spec[i] === ')';
        c -= spec[i] === '(';
    }
    const args = spec.slice(i + 2, spec.length - 2);
    const rettAndFunc = spec.slice(0, i + 1).trim();
    const func = rettAndFunc.replace(/^.*\s+([a-zA-Z_][a-zA-Z0-9_]*)$/, (all, a) => a);
    //console.log('defun: ' + rettAndFunc + ' - ' + args);
    if (func === rettAndFunc) {
        throw new Error("Could not parse function [" + spec + "]");
    }
    const rett = rettAndFunc.replace(/\s+[a-zA-Z_][a-zA-Z0-9_]*$/, '').trim();
    ctx.activeFunction = ctx.functions[func] = { rett: rett };
    if (rett === 'void') {
        return 'struct Er_Ret* ' + func + '(' + args + ')';
    } else {
        return 'struct Er_Ret* ' + func + '(' + rett + ' *Er_returnValP, ' + args + ')';
    }
};

const ret = (ctx, val) => {
    val = trim(val);
    if (ctx.activeFunction.rett === 'void') {
        return 'return (struct Er_Ret*)0';
    } else {
        return '*Er_returnValP = ' + val + '; return (struct Er_Ret*)0';
    }
};

const er = (ctx, assert, errOut, expr, file, line) => {
    expr = trim(expr);
    if (!/[a-zA-Z_][a-zA-Z0-9_]*\(.*\)$/.test(expr)) {
        throw new Error("Er() expr must be in the form Er(funcName(arg1, arg2, ...)) " +
            "in [" + expr + "]");
    }
    const funcName = expr.slice(0, expr.indexOf('('));
    const f = ctx.functions[funcName];
    if (!f) {
        throw new Error("Er() not a defined function [" + funcName + "] in [" + expr + "]");
    }
    let ifret = `if (Er_ret) { return Er_unwind("${file}", ${line}, Er_ret); }`;
    if (assert) {
        ifret = `
            if (Er_ret) {
                struct Er_Ret** Er_errOut = ${errOut ? errOut : '(struct Er_Ret**)0'};
                if (Er_errOut) {
                    *Er_errOut = Er_unwind("${file}", ${line}, Er_ret);
                } else {
                    Er__assertFail(Er_unwind("${file}", ${line}, Er_ret));
                }
            }
        `;
    }
    if (f.rett === 'void') {
        return `do {
            struct Er_Ret* Er_ret = ${expr};
            ${ifret}
        } while (0)`;
    } else {
        const args = expr.slice(expr.indexOf('(') + 1);
        return `(__extension__({
            ${f.rett} Er_returnVal;
            __builtin_memset(&Er_returnVal, 0, sizeof(Er_returnVal));
            struct Er_Ret* Er_ret = ${funcName}(&Er_returnVal, ${args};
            ${ifret}
            Er_returnVal;
        }))`;
    }
};

module.exports.create = () => {
    const ctx = {
        activeFunction: undefined,
        functions: {},
    };
    return {
        defun: (spec) => defun(ctx, spec).replace(/\n/g, ' '),
        ret: (val) => ret(ctx, val).replace(/\n/g, ' '),
        er: (expr, file, line) => er(ctx, false, null, expr, file, line).replace(/\n/g, ' '),
        assert: (expr, file, line) => er(ctx, true, null, expr, file, line).replace(/\n/g, ' '),
        check: (out, expr, file, line) => er(ctx, true, out, expr, file, line).replace(/\n/g, ' '),
    };
};
