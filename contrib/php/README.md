cjdns.inc.php
=============
A simple PHP API for the CJDNS admin interface.

Installing
-------------
Make a `secrets.inc.php` file (see `secrets.example.inc.php`) and include it in
your page, in addition to `cjdns.inc.php`. It is recommended that you keep
`secrets.inc.php` out of your http directory to prevent accidental password
leakage.

Usage
-------------
```php
<?
// Include the two files
require("/etc/cjdns/secrets.inc.php");
require("cjdns.inc.php");

// Make a new cjdns object
$cjdns = new cjdns;

// Connect to the admin interface
$cjdns->connect();

// Print a list of available functions and their arguments
print_r($cjdns->functionlist());

// Call one of them:
$ping = $cjdns->call("RouterModule_pingNode",array("path"=>"fc72:6c3b:8c74:68a7:d8c3:b4e0:6cbd:9588")));

// Display the result:
print_r($ping);

// Close the connection:
$cjdns->disconnect();
?>
```

Bugs
-------------
Please report any bugs you find to thefinn93. IRC is fine, as is github.
