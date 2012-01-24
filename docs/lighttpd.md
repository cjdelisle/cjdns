# Configuring Lighttpd with CJDNS IPv6

_Download and install lighttpd dependencies:_

	# sudo apt-get install libpcre3-dev

_Download, unpack, configure and build lighttpd:_

	# wget http://download.lighttpd.net/lighttpd/releases-1.4.x/lighttpd-1.4.30.tar.xz
	# tar xJvf lighttpd-1.4.30.tar.xz
	# cd lighttpd-1.4.30
	# configure
	# make

_Get the IPv6 address for your node (assuming its tunnel `tun0`):_

	# ifconfig tun0

_Write a configuration file for `lighttpd`:_

	# Default Server Configuration
	server.use-ipv6 = "enable"
	server.bind = "fc6d:3c44:c420:cefc:432f:6f6b:2aa:d2b7"
	server.port = 8080

	server.document-root = "html6"
	# MIME Types
	mimetype.assign   = ( ".png"  => "image/png",
	                      ".jpg"  => "image/jpeg",
	                      ".jpeg" => "image/jpeg",
	                      ".html" => "text/html",
	                      ".txt"  => "text/plain" )
	                      
	# Index Files
	index-file.names = ( "index.html", "index.txt" )

	# Access Logging
	server.modules += ( "mod_accesslog" )
	accesslog.filename = "ipv6.access.log"

_Launch `lighttpd`:_

	# lighttpd -f lighttpd.conf
