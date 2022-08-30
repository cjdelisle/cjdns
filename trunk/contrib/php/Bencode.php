<?php

/**
 * Rych Bencode Component
 *
 * @package Rych\Bencode
 * @author Ryan Chouinard <rchouinard@gmail.com>
 * @copyright Copyright (c) 2014, Ryan Chouinard
 * @license MIT License - http://www.opensource.org/licenses/mit-license.php
 */
class Bencode
{

    const TYPE_ARRAY = 'array';
    const TYPE_OBJECT = 'object'; // NOT IMPLEMENTED

    /**
     * Decodes a bencoded string
     *
     * @param string $string The bencoded string to decode.
     * @param string $decodeType Flag used to indicate whether the decoded
     *     value should be returned as an object or an array.
     * @return mixed Returns the appropriate data type for the bencoded data.
     */
    public static function decode($string, $decodeType = self::TYPE_ARRAY)
    {
        return Decoder::decode($string, $decodeType);
    }

    /**
     * Encodes a value into a bencoded string
     *
     * @param mixed $value The value to bencode.
     * @return string Returns a bencoded string.
     */
    public static function encode($value)
    {
        return Encoder::encode($value);
    }

}
class Decoder
{

    /**
     * @var string
     */
    private $_source;

    /**
     * @var string
     */
    private $_decodeType;

    /**
     * @var integer
     */
    private $_sourceLength;

    /**
     * @var integer
     */
    private $_offset = 0;

    /**
     * Class constructor
     *
     * @param string $source The bencode string to be decoded.
     * @param string $decodeType currently unused.
     * @return void
     */
    private function __construct($source, $decodeType)
    {
        $this->_source = $source;
        $this->_sourceLength = strlen($this->_source);
        if ($decodeType != Bencode::TYPE_ARRAY && $decodeType != Bencode::TYPE_OBJECT) {
            $decodeType = Bencode::TYPE_ARRAY;
        }
        $this->_decodeType = $decodeType;
    }

    /**
     * Decode a bencode entity into a value
     *
     * @param string $source The bencode string to be decoded.
     * @param string $decodeType currently unused.
     * @return mixed Returns the decoded value.
     * @throws Rych\Bencode\Exception\RuntimeException
     */
    static public function decode($source, $decodeType = Bencode::TYPE_ARRAY)
    {
        if (!is_string($source)) {
            throw new RuntimeException('Argument expected to be a string; Got ' . gettype($source));
        }

        $decoder = new self($source, $decodeType);
        $decoded = $decoder->_decode();

        if ($decoder->_offset != $decoder->_sourceLength) {
            throw new RuntimeException(
                'Found multiple entities outside list or dict definitions'
            );
        }

        return $decoded;
    }

    /**
     * Decode a bencode entity into a value
     *
     * @return mixed Returns the decoded value.
     * @throws Rych\Bencode\Exception\RuntimeException
     */
    private function _decode()
    {
        switch ($this->_getChar()) {

            case 'i':
                ++$this->_offset;
                return $this->_decodeInteger();
                break;

            case 'l':
                ++$this->_offset;
                return $this->_decodeList();
                break;

            case 'd':
                ++$this->_offset;
                return $this->_decodeDict();
                break;

            default:
                if (ctype_digit($this->_getChar())) {
                    return $this->_decodeString();
                }

        }

        throw new RuntimeException('Unknown entity found at offset ' . $this->_offset);
    }

    /**
     * Decode a bencode integer into an integer
     *
     * @return integer Returns the decoded integer.
     * @throws Rych\Bencode\Exception\RuntimeException
     */
    private function _decodeInteger()
    {
        $offsetOfE = strpos($this->_source, 'e', $this->_offset);
        if (false === $offsetOfE) {
            throw new RuntimeException('Unterminated integer entity at offset ' . $this->_offset);
        }

        $currentOffset = $this->_offset;
        if ('-' == $this->_getChar($currentOffset)) {
            ++$currentOffset;
        }

        /* if ('-' == $this->_getChar($currentOffset) && '0' == $this->_getChar($currentOffset + 1)) {
            throw new RuntimeException('Illegal zero-padding found in integer entity at offset ' . $this->_offset);
        }*/

        if ($offsetOfE === $currentOffset) {
            throw new RuntimeException('Empty integer entity at offset ' . $this->_offset);
        }

        while ($currentOffset < $offsetOfE) {
            if (!ctype_digit($this->_getChar($currentOffset))) {
                throw new RuntimeException('Non-numeric character found in integer entity at offset ' . $this->_offset);
            }
            ++$currentOffset;
        }

        $value = substr($this->_source, $this->_offset, $offsetOfE - $this->_offset);

        // Cjdns pads zeros which is a CLEAR violation of the bencode standard,
        // so we have to adjust the validation.
        
        // $absoluteValue = (string) abs($value);
        //if (1 < strlen($absoluteValue) && '0' == $value[0]) {
        if (1 < strlen($value) && '0' == $value[0]) {
            // TODO: Could probably just trigger a warning here
            throw new RuntimeException('Illegal zero-padding found in integer entity at offset ' . $this->_offset);
        }

        $this->_offset = $offsetOfE + 1;

        // The +0 auto-casts the chunk to either an integer or a float(in cases
        // where an integer would overrun the max limits of integer types)
        return $value + 0;
    }

    /**
     * Decode a bencode string into a string
     *
     * @return string Returns the decoded string.
     * @throws Rych\Bencode\Exception\RuntimeException
     */
    private function _decodeString()
    {
/*        if ('0' === $this->_getChar() && ':' != $this->_getChar($this->_offset + 1)) {
            // TODO: Trigger a warning instead?
            throw new RuntimeException('Illegal zero-padding in string entity length declaration at offset ' . $this->_offset);
        }*/

        $offsetOfColon = strpos($this->_source, ':', $this->_offset);
        if (false === $offsetOfColon) {
            throw new RuntimeException('Unterminated string entity at offset ' . $this->_offset);
        }

        $contentLength = (int) substr($this->_source, $this->_offset, $offsetOfColon);
        if (($contentLength + $offsetOfColon + 1) > $this->_sourceLength) {
            throw new RuntimeException('Unexpected end of string entity at offset ' . $this->_offset);
        }

        $value = substr($this->_source, $offsetOfColon + 1, $contentLength);
        $this->_offset = $offsetOfColon + $contentLength + 1;

        return $value;
    }

    /**
     * Decode a bencode list into a numeric array
     *
     * @return array Returns the decoded array.
     * @throws Rych\Bencode\Exception\RuntimeException
     */
    private function _decodeList()
    {
        $list = array ();
        $terminated = false;
        $listOffset = $this->_offset;

        while (false !== $this->_getChar()) {
            if ('e' == $this->_getChar()) {
                $terminated = true;
                break;
            }

            $list[] = $this->_decode();
        }

        if (!$terminated && false === $this->_getChar()) {
            throw new RuntimeException('Unterminated list definition at offset ' . $listOffset);
        }

        ++$this->_offset;

        return $list;
    }

    /**
     * Decode a bencode dictionary into an associative array
     *
     * @return array Returns the decoded array.
     * @throws Rych\Bencode\Exception\RuntimeException
     */
    private function _decodeDict()
    {
        $dict = array ();
        $terminated = false;
        $dictOffset = $this->_offset;

        while (false !== $this->_getChar()) {
            if ('e' == $this->_getChar()) {
                $terminated = true;
                break;
            }

            $keyOffset = $this->_offset;
            if (!ctype_digit($this->_getChar())) {
                throw new RuntimeException('Invalid dictionary key at offset ' . $keyOffset);
            }

            $key = $this->_decodeString();
            if (isset ($dict[$key])) {
                // TODO: This could probably just trigger a warning...
                throw new RuntimeException('Duplicate dictionary key at offset ' . $keyOffset);
            }

            $dict[$key] = $this->_decode();
        }

        if (!$terminated && false === $this->_getChar()) {
            throw new RuntimeException('Unterminated dictionary definition at offset ' . $dictOffset);
        }

        ++$this->_offset;

        return $dict;
    }

    /**
     * Fetch the character at the specified source offset
     *
     * If not offset is provided, the current offset is used.
     *
     * @param integer $offset the offset to retrieve from the source string.
     * @return string Returns the character found at the specified offset. If
     *     the specified offset is out of range, false is returned.
     */
    private function _getChar($offset = null)
    {
        if (null === $offset) {
            $offset = $this->_offset;
        }

        if (empty ($this->_source) || $this->_offset >= $this->_sourceLength) {
            return false;
        }

        return $this->_source[$offset];
    }

}

class Encoder
{

    /**
     * @var mixed Entity to be encoded.
     */
    private $_data;

    /**
     * Class constructor
     *
     * @param mixed $data Entity to be encoded.
     * @return void
     */
    private function __construct($data)
    {
        $this->_data = $data;
    }

    /**
     * Encode a value into a bencode entity
     *
     * @param mixed $data The value to be encoded.
     * @return string Returns the bencoded entity.
     */
    static public function encode($data)
    {
        if (is_object($data)) {
            if (method_exists($data, 'toArray')) {
                $data = $data->toArray();
            } else {
                $data = (array) $data;
            }
        }

        $encoder = new self($data);
        return $encoder->_encode();
    }

    /**
     * Encode a value into a bencode entity
     *
     * @param mixed $data The value to be encoded.
     * @return string Returns the bencoded entity.
     */
    private function _encode($data = null)
    {
        $data = is_null($data) ? $this->_data : $data;

        if (is_array($data) && (isset ($data[0]) || empty ($data))) {
            return $this->_encodeList($data);
        } else if (is_array($data)) {
            return $this->_encodeDict($data);
        } else if (is_integer($data) || is_float($data)) {
            $data = sprintf('%.0f', round($data, 0));
            return $this->_encodeInteger($data);
        } else {
            return $this->_encodeString($data);
        }
    }

    /**
     * Encode an integer into a bencode integer
     *
     * @param integer $data The integer to be encoded.
     * @return string Returns the bencoded integer.
     */
    private function _encodeInteger($data = null)
    {
        $data = is_null($data) ? $this->_data : $data;
        return sprintf('i%.0fe', $data);
    }

    /**
     * Encode a string into a bencode string
     *
     * @param string $data The string to be encoded.
     * @return string Returns the bencoded string.
     */
    private function _encodeString($data = null)
    {
        $data = is_null($data) ? $this->_data : $data;
        return sprintf('%d:%s', strlen($data), $data);
    }

    /**
     * Encode a numeric array into a bencode list
     *
     * @param array $data The numerically indexed array to be encoded.
     * @return string Returns the bencoded list.
     */
    private function _encodeList(array $data = null)
    {
        $data = is_null($data) ? $this->_data : $data;

        $list = '';
        foreach ($data as $value) {
            $list .= $this->_encode($value);
        }

        return "l{$list}e";
    }

    /**
     * Encode an associative array into a bencode dictionary
     *
     * @param array $data The associative array to be encoded.
     * @return string Returns the bencoded dictionary.
     */
    private function _encodeDict(array $data = null)
    {
        $data = is_null($data) ? $this->_data : $data;
        ksort($data); // bencode spec requires dicts to be sorted alphabetically

        $dict = '';
        foreach ($data as $key => $value) {
            $dict .= $this->_encodeString($key) . $this->_encode($value);
        }

        return "d{$dict}e";
    }

}