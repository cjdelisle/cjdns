<?php

function labelToBits($label) {
    $out = array();
    $sections = explode('.', $label);
    $sections = array_reverse($sections);
    foreach ($sections as $sec) {
        $n = hexdec("0x" . $sec);
        for ($i = 0; $i < 16; $i++) {
            array_unshift($out, ($n & 1) ? 1 : 0);
            $n >>= 1;
        }
    }
    return $out;
}

function bitsToChar(&$array) {
    $num = 0;
    for ($i = 0; $i < 4; $i++) {
        $num |= array_pop($array) << $i;
    }
    return dechex($num);
}

function bitsToLabel(&$array) {
    $chars = [];
    for ($i = 0; $i < 16; $i++) {
        if (($i % 4) === 0) {
            array_unshift($chars, '.');
        }
        array_unshift($chars, bitsToChar($array));
    }
    array_pop($chars);
    return implode('', $chars);
}

function randLabel () {
    $out = [];
    for ($i = 0; $i < 4; $i++) {
        $x = bin2hex(random_bytes(16));
        if (strlen($x) < 6) {
            $i--;
            continue;
        }
        array_push($out, substr($x,strlen($x)-4));
    }

    return implode('.', $out);
}

function test() {
    for ($i = 0; $i < 1000; $i++) {
        $x = randLabel();
        if (bitsToLabel(labelToBits($x)) !== $x) {
            throw new Exception($x);
        }
    }
}

test();

function errorArray () {
    $out = [];
    for ($i = 0; $i < 64; $i++) { array_push($out, 1); }
    return $out;
};

function splice(&$goHere, $viaHere) {
    while (array_shift($viaHere) === 0) ;
    $goHere = array_merge($goHere, $viaHere);
    while (array_shift($goHere) === 0) ;
    array_unshift($goHere, 1);
    if (count($goHere) >= 63) {
        return errorArray();
    }
    while (count($goHere) < 64) {
        array_unshift($goHere, 0);
    }
    return $goHere;
}


if (sizeof($argv) > 2) {
    $viaHereL = array_pop($argv);
    $goHereL = array_pop($argv);
    $result = splice(labelToBits($goHereL), labelToBits($viaHereL));
    echo bitsToLabel($result) . "\n";
}

