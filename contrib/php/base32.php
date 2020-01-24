<?php

define('BASE32_ALPHABET', '0123456789bcdfghjklmnpqrstuvwxyz');
define('BASE32_NBITS',    5);

function base32_encode($input)
{
	$output   = '';

	$buffer = ['size'=>BASE32_NBITS, 'callback'=> function($x) use(&$output) { $output .= BASE32_ALPHABET[$x]; }];
	$length = strlen($input);
	for ($i = 0; $i < $length; $i++)
	{
		$char = ord($input[$i]);
		base32_buffer_add($buffer, $char, 8);
	}

	if ($buffer) { base32_buffer_add($buffer, 0, 8); $length--; }
	$output .= str_repeat('=', $length % 3);

	return $output;
}

function base32_decode($input)
{
	$input  = preg_replace('#\s#', '', $input);
	$output = '';

	$buffer = ['size'=>8, 'callback'=> function($x) use(&$output) { $output .= chr($x); }];
	for ($i = 0; $i < strlen($input); $i++)
	if ($input[$i] != '=')
	{
		$char = strpos(BASE32_ALPHABET, $input[$i]);
		base32_buffer_add($buffer, $char, BASE32_NBITS);
	}

	return $output;
}

function base32_buffer_add(&$buffer, $char, $char_size)
{
	@$buffer['data']      |= $char << @$buffer['data_size'];
	@$buffer['data_size'] += $char_size;
	while ($buffer['data_size'] > $buffer['size'])
	{
		$mask = pow(2, $buffer['size']) - 1;
		$char = $buffer['data'] & $mask;
		$buffer['callback']($char);
		$buffer['data'] >>= $buffer['size'];
		$buffer['data_size'] -= $buffer['size'];
	}
}
