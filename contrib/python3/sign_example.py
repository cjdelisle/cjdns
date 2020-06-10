from cjdnsadmin.cjdnsadmin import connect
cjdns = connect("127.0.0.1", 11234, "NONE")
print(cjdns.Sign_checkSig(
    'test message',
    '0ytl2njc1hy86tlxtc2zc3449up47uqb0u04kcy233d7zrn2cwh1_y96duzwpvmslj8b7pnk2b32m0rhs738yujwtrtlcq81r0u114svygwn56phn9yncpyzhswpj3bd808lgd5bknlj8xwf7purl0r0hc30'))