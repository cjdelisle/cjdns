cjdns.inc.php
=============
A simple PHP API for the cjdns admin interface.

Usage
-------------
```php
<?php
// Include the two files
require "Bencode.php";
require "Cjdns.php";

// Make a new Cjdns object. The only required argument is the password, but it also accepts the IP and the port
$cjdns = new Cjdns("password", "127.0.0.1", 11234);

// Print a list of available functions and their arguments
print_r($cjdns->functions);

// Call one of them:
$ping = $cjdns->call("RouterModule_pingNode",array("path"=>"fc72:6c3b:8c74:68a7:d8c3:b4e0:6cbd:9588"));

// Display the result:
print_r($ping);
?>
```

Bugs
-------------
Please report any bugs you find to derp. IRC is fine, as is github.
