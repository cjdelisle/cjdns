/* Copyright (c) 2009 Anton Ekblad

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software. */


/*self.addEventListener('message', function(e) {
  self.postMessage(bdecode(e.data));
}, false);*/

// bencode an object
function bencode(obj) {
    switch(btypeof(obj)) {
        case "string":     return bstring(obj);
        case "number":     return bint(obj);
        case "list":       return blist(obj);
        case "dictionary": return bdict(obj);
        default:           return null;
    }
}

// decode a bencoded string into a javascript object
function bdecode(str) {
    var dec = bparse(str);
    if(dec !== null && dec[1] === "")
        return dec[0];
    return null;
}

// parse a bencoded string; bdecode is really just a wrapper for this one.
// all bparse* functions return an array in the form
// [parsed object, remaining string to parse]
function bparse(str) {
    switch(str.charAt(0)) {
        case "d": return bparseDict(str.substr(1));
        case "l": return bparseList(str.substr(1));
        case "i": return bparseInt(str.substr(1));
        default:  return bparseString(str);
    }
}

// parse a bencoded string
function bparseString(str) {
    str2 = str.split(":", 1)[0];
    if(isNum(str2)) {
        len = parseInt(str2);
        return [str.substr(str2.length+1, len),
                str.substr(str2.length+1+len)];
    }
    return null;
}

// parse a bencoded integer
function bparseInt(str) {
    var str2 = str.split("e", 1)[0];
    if(!isNum(str2)) {
        return null;
    }
    return [str2, str.substr(str2.length+1)];
}

// parse a bencoded list
function bparseList(str) {
    var p, list = [];
    while(str.charAt(0) != "e" && str.length > 0) {
        p = bparse(str);
        if(null === p)
            return null;
        list.push(p[0]);
        str = p[1];
    }
    if(str.length <= 0)
        return null;
    return [list, str.substr(1)];
}

// parse a bencoded dictionary
function bparseDict(str) {
    var key, val, dict = {};
    while(str.charAt(0) != "e" && str.length > 0) {
        key = bparseString(str);
        if(null === key)
            return;

        val = bparse(key[1]);
        if(null === val)
            return null;

        dict[key[0]] = val[0];
        str = val[1];
    }
    if(str.length <= 0)
        return null;
    return [dict, str.substr(1)];
}

// is the given string numeric?
function isNum(str) {
    var i, c;
    str = str.toString();
    if(str.charAt(0) == '-')
        i = 1;
    else
        i = 0;

    for(; i < str.length; i++) {
        c = str.charCodeAt(i);
        if(c < 48 || c > 57) {
            return false;
        }
    }
    return true;
}

// returns the bencoding type of the given object
function btypeof(obj) {
    var type = typeof obj;
    if(type == "object") {
        if(typeof obj.length == "undefined")
            return "dictionary";
        return "list";
    }
    return type;
}

// bencode a string
function bstring(str) {
    return (str.length + ":" + str);
}

// bencode an integer
function bint(num) {
    return "i" + num + "e";
}

// bencode a list
function blist(list) {
    var str, enclist;
    enclist = [];
    for(var key in list) {
        enclist.push(bencode(list[key]));
    }
    enclist.sort();

    str = "l";
    for(var enckey in enclist) {
        str += enclist[enckey];
    }
    return str + "e";
}

// bencode a dictionary
function bdict(dict) {
    var str, enclist;
    enclist = [];
    for(var key in dict) {
        enclist.push(bstring(key) + bencode(dict[key]));
    }
    enclist.sort();

    str = "d";
    for(var enckey in enclist) {
        str += enclist[enckey];
    }
    return str + "e";
}

module.exports.encode = bencode;
module.exports.decode = bdecode;
