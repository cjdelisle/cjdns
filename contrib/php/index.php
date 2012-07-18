<pre>
<?
require_once("cjdns.php");
$admin = new cjdns;
$admin->connect("supersecretpassword");
print_r($admin->call("RouterModule_pingNode",array("path"=>"fc72:6c3b:8c74:68a7:d8c3:b4e0:6cbd:9588")));
$admin->functionlist();
$admin->disconnect();
?>
</pre>
