<?php
require "base32.php";

function publicKey2ipv6($publicKey)
{
	$publicKey = preg_replace('#.k$#', '', $publicKey);
	$data = base32_decode($publicKey);
	$data = hash('sha512', $data, true);
	$data = hash('sha512', $data);
	$data = substr($data, 0, 32);
	return substr(preg_replace("#(.{4})#", "\$1:", $data), 0, -1);
}
