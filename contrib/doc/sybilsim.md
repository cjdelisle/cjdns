sybilsim(8) -- Cjdns packet switch
=============================================

## SYNOPSIS

`/usr/libexec/cjdns/sybilsim < config.json`

## DESCRIPTION

Sybilsim reads a list of nodes and peers from stdin and simulates the
corresponding mesh without any actual networking using the same code
as cjdroute.  This is useful for testing and optimizing cjdroute.
Every node must have a valid cjdns private key. The makekeys(1) utility can be
useful for scripts that generate the config.  

##USAGE

Example config:

    {
      "nodes": {
	"alice": {
	  "privateKey":
	    "5e2295679394e5e1db67c238abbc10292ad9b127904394c52cc5fff39383e920",
	  "peers": []
	},
	"bob": {
	  "privateKey":
	    "6569bf3f0d168faa6dfb2912f8ee5ee9b938319e97618fdf06caed73b1aad1cc",
	  "peers": [ "alice" ]
	}
      }
    }

Example use:

    makekeys | head -20 >keys.txt
    node /usr/libexec/cjdns/tools/lib/makesim.js keys.txt |
    /usr/libexec/cjdns/sybilsim

## SEE ALSO

makekeys(1), cjdroute(1)
