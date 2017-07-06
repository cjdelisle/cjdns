# Written by Petru Paler
# see LICENSE.txt for license information
# http://cvs.degreez.net/viewcvs.cgi/*checkout*/bittornado/LICENSE.txt?rev=1.2
# "the MIT license"

import sys

if sys.version_info >= (3,):
    py23_ord = lambda c: ord(c)
else:
    py23_ord = lambda c: c

def decode_int(x, f):
    f += 1
    newf = x.index(py23_ord('e'), f)
    try:
        n = int(x[f:newf])
    except (OverflowError, ValueError):
        n = int(x[f:newf])
    if x[f] == py23_ord('-'):
        if x[f + 1] == py23_ord('0'):
            raise ValueError
    elif x[f] == py23_ord('0') and newf != f+1:
        raise ValueError
    return (n, newf+1)

def decode_string(x, f):
    colon = x.index(py23_ord(':'), f)
    try:
        n = int(x[f:colon])
    except (OverflowError, ValueError):
        n = int(x[f:colon])
# Leading zeros are FINE --cjd
#    if x[f] == b'0' and colon != f+1:
#        raise ValueError
    colon += 1
    return (x[colon:colon+n], colon+n)

def decode_list(x, f):
    r, f = [], f+1
    while x[f] != py23_ord('e'):
        v, f = decode_func[x[f]](x, f)
        r.append(v)
    return (r, f + 1)

def decode_dict(x, f):
    r, f = {}, f+1
    lastkey = None
    while x[f] != py23_ord('e'):
        k, f = decode_string(x, f)
        if sys.version_info >= (3,):
            k = k.decode()
        if lastkey is not None and lastkey >= k:
            raise ValueError
        lastkey = k
        r[k], f = decode_func[x[f]](x, f)
    return (r, f + 1)

decode_func = {}
decode_func['l'] = decode_list
decode_func['d'] = decode_dict
decode_func['i'] = decode_int
decode_func['0'] = decode_string
decode_func['1'] = decode_string
decode_func['2'] = decode_string
decode_func['3'] = decode_string
decode_func['4'] = decode_string
decode_func['5'] = decode_string
decode_func['6'] = decode_string
decode_func['7'] = decode_string
decode_func['8'] = decode_string
decode_func['9'] = decode_string

for (key, value) in list(decode_func.items()):
    decode_func[ord(key)] = value

def bdecode_stream(x):
    return decode_func[x[0]](x, 0);

def bdecode(x):
    if sys.version_info >= (3,) and not isinstance(x, bytes):
        raise TypeError('%r is not of type bytes' % x)
    try:
        r, l = bdecode_stream(x);
    except (IndexError, KeyError):
        raise ValueError
    if l != len(x):
        raise ValueError
    return r

def test_bdecode():
    try:
        bdecode(b'0:0:')
        assert 0
    except ValueError:
        pass
    try:
        bdecode(b'ie')
        assert 0
    except ValueError:
        pass
    try:
        bdecode(b'i341foo382e')
        assert 0
    except ValueError:
        pass
    assert bdecode(b'i4e') == 4
    assert bdecode(b'i0e') == 0
    assert bdecode(b'i123456789e') == 123456789
    assert bdecode(b'i-10e') == -10
#    cjd said leading zeros are fine.
#    try:
#        bdecode(b'i-0e')
#        assert 0
#    except ValueError:
#        pass
    try:
        bdecode(b'i123')
        assert 0
    except ValueError:
        pass
    try:
        bdecode(b'')
        assert 0
    except ValueError:
        pass
    try:
        bdecode(b'i6easd')
        assert 0
    except ValueError:
        pass
    try:
        bdecode(b'35208734823ljdahflajhdf')
        assert 0
    except ValueError:
        pass
    try:
        bdecode(b'2:abfdjslhfld')
        assert 0
    except ValueError:
        pass
    assert bdecode(b'0:') == b''
    assert bdecode(b'3:abc') == b'abc'
    assert bdecode(b'10:1234567890') == b'1234567890'
#    cjd said leading zeros are fine.
#    try:
#        bdecode(b'02:xy')
#        assert 0
#    except ValueError:
#        pass
    try:
        bdecode(b'l')
        assert 0
    except ValueError:
        pass
    assert bdecode(b'le') == []
    try:
        bdecode(b'leanfdldjfh')
        assert 0
    except ValueError:
        pass
    assert bdecode(b'l0:0:0:e') == [b'', b'', b'']
    try:
        bdecode(b'relwjhrlewjh')
        assert 0
    except ValueError:
        pass
    assert bdecode(b'li1ei2ei3ee') == [1, 2, 3]
    assert bdecode(b'l3:asd2:xye') == [b'asd', b'xy']
    assert bdecode(b'll5:Alice3:Bobeli2ei3eee') == [[b'Alice', b'Bob'], [2, 3]]
    try:
        bdecode(b'd')
        assert 0
    except ValueError:
        pass
    try:
        bdecode(b'defoobar')
        assert 0
    except ValueError:
        pass
    assert bdecode(b'de') == {}
    assert bdecode(b'd3:agei25e4:eyes4:bluee') == {'age': 25, 'eyes': b'blue'}, bdecode(b'd3:agei25e4:eyes4:bluee')
    assert bdecode(b'd8:spam.mp3d6:author5:Alice6:lengthi100000eee') == {'spam.mp3': {'author': b'Alice', 'length': 100000}}
    try:
        bdecode(b'd3:fooe')
        assert 0
    except ValueError:
        pass
    try:
        bdecode(b'di1e0:e')
        assert 0
    except ValueError:
        pass
    try:
        bdecode(b'd1:b0:1:a0:e')
        assert 0
    except ValueError:
        pass
    try:
        bdecode(b'd1:a0:1:a0:e')
        assert 0
    except ValueError:
        pass
    try:
        bdecode(b'i03e')
        assert 0
    except ValueError:
        pass
#    cjd said leading zeros are fine.
#    try:
#        bdecode(b'l01:ae')
#        assert 0
#    except ValueError:
#        pass
    try:
        bdecode(b'9999:x')
        assert 0
    except ValueError:
        pass
    try:
        bdecode(b'l0:')
        assert 0
    except ValueError:
        pass
    try:
        bdecode(b'd0:0:')
        assert 0
    except ValueError:
        pass
    try:
        bdecode(b'd0:')
        assert 0
    except ValueError:
        pass
#    cjd said leading zeros are fine.
#    try:
#        bdecode(b'00:')
#        assert 0
#    except ValueError:
#        pass
    try:
        bdecode(b'l-3:e')
        assert 0
    except ValueError:
        pass
    try:
        bdecode(b'i-03e')
        assert 0
    except ValueError:
        pass
    bdecode(b'd0:i3ee')

class Bencached(object):
    __slots__ = ['bencoded']

    def __init__(self, s):
        self.bencoded = s

def encode_bencached(x,r):
    r.append(x.bencoded)

def encode_int(x, r):
    r.extend((b'i', str(x).encode(), b'e'))

def encode_string(x, r):
    # Make sure x is encoded before running len() on it, so we get its
    # size and not its number of characters.
    if not isinstance(x, str):
        raise TypeError('%r is not a string' % x)
    x = x.encode('utf8')
    r.extend((str(len(x)).encode(), b':', x))

def encode_bytes(x, r):
    r.extend((str(len(x)).encode(), b':', x))

def encode_list(x, r):
    r.append(b'l')
    for i in x:
        encode_func[type(i)](i, r)
    r.append(b'e')

def encode_dict(x,r):
    r.append(b'd')
    ilist = list(x.items())
    ilist.sort()
    for k, v in ilist:
        encode_string(k, r)
        encode_func[type(v)](v, r)
    r.append(b'e')

encode_func = {}
encode_func[type(Bencached(0))] = encode_bencached
encode_func[int] = encode_int
if sys.version_info < (3,):
    encode_func[long] = encode_int
encode_func[str] = encode_string
if sys.version_info >= (3,):
    encode_func[bytes] = encode_bytes
encode_func[list] = encode_list
encode_func[tuple] = encode_list
encode_func[dict] = encode_dict

encode_func[bool] = encode_int

def bencode(x):
    r = []
    encode_func[type(x)](x, r)
    return b''.join(r)

def test_bencode():
    assert bencode(4) == b'i4e'
    assert bencode(0) == b'i0e'
    assert bencode(-10) == b'i-10e'
    assert bencode(12345678901234567890) == b'i12345678901234567890e'
    assert bencode('') == b'0:'
    assert bencode('abc') == b'3:abc'
    assert bencode('1234567890') == b'10:1234567890'
    assert bencode([]) == b'le'
    assert bencode([1, 2, 3]) == b'li1ei2ei3ee'
    assert bencode([['Alice', 'Bob'], [2, 3]]) == b'll5:Alice3:Bobeli2ei3eee'
    assert bencode({}) == b'de'
    assert bencode({'age': 25, 'eyes': 'blue'}) == b'd3:agei25e4:eyes4:bluee'
    assert bencode({'spam.mp3': {'author': 'Alice', 'length': 100000}}) == b'd8:spam.mp3d6:author5:Alice6:lengthi100000eee'
    assert bencode(Bencached(bencode(3))) == b'i3e'
    try:
        bencode({1: 'foo'})
    except TypeError:
        return
    assert 0

try:
    import psyco
    psyco.bind(bdecode)
    psyco.bind(bencode)
except ImportError:
    pass
