<?
require_once("cjdns.php");
$admin = new cjdns;
$admin->connect("supersecure_password");
print_r($admin->call("ping"));
$admin->disconnect();
?>
