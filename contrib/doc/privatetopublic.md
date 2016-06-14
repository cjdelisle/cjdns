privatetopublic(8) -- convert cjdns private keys to public keys and IP6
=============================================

## SYNOPSIS

`/usr/libexec/cjdns/privatetopublic`

## DESCRIPTION

As private keys are very sensitive, /usr/libexec/cjdns/privatetopublic reads
them from stdin.  If your shell, terminal, or other program keeps history,
please avoid `echo 'key' | /usr/libexec/cjdns/privatetopublic`, or similar
constructs.  A heredoc is suitable for use in scripts, and will avoid
unwanted revealing of the key in process lists, like so:

    /usr/libexec/cjdns/privatetopublic <<EOF
    key
    EOF

Key should be in hex form, maximum 64 characters.  Extra characters will be
silently ignored.

## SEE ALSO

publictoip6(1)
