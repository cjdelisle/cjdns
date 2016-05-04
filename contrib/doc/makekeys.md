makekeys(1) -- write cjdns keys generated via libnacl to stdout
=============================================

## SYNOPSIS

`makekeys`

## DESCRIPTION

Generates cjdns private keys and writes them to stdout forever with
corresponding IP6 address and public key.  Sample:

    9b5520e11b4e1aac4e54cf8382ca219b6942f75519dbe7cc71fc6c4bb636a2db fca1:43dc:9e20:68dc:df3f:6bf1:fbf9:37ab x70u0x6sfh0yuushlq7wvcdxj8rgb8lvss9t6f42zp60ftq71cm0.k
    579db68cfdeaeda1ff7f365c73e620bedb0f68cb443a0cd7ef6bae16d0e1e12e fc68:b272:3aa3:cb4a:a686:dab0:7df5:2fb5 fmurp2qrb0yqmcjmbjtuhuxuf6v3rgmdn9hgsfh47k20h68y2lz0.k

##USAGE

Example use:

    makekeys | head -20 >keys.txt

## SEE ALSO

publictoip6(1)
