randombytes(1) -- write secure random bytes to stdout
=============================================

## SYNOPSIS

`randombytes`

## DESCRIPTION

Writes random bytes to stdout forever using the cjdns secure random generator.
In practice, the output is piped to a program like dd(1) or base64(1).

## SEE ALSO

makekeys(1)
