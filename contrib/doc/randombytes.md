randombytes(1) -- write random bytes generated via libnacl to stdout
=============================================

## SYNOPSIS

`randombytes`

## DESCRIPTION

Writes random bytes to stdout forever using the cryptographically secure
random generator in libnacl.  In practice, the output is piped
to a program like dd(1) or base64(1).

## SEE ALSO

makekeys(1)
