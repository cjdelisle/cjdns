<?php
/*
  Bencoding OOP wrapper for PHP | by Proger_XP | In public domain
    http://proger.i-forge.net/BEncoded/7Tn

  Based on lightenc.php functions from
    http://wiki.theory.org/Decoding_encoding_bencoded_data_with_PHP
*/

/* BEncoded classes by Proger_XP */
class BEncoded {
  public $nodes;
  public $tempChar;

  static function Decode($str) {
    $res = bdecode($str);
    if ($res === null) {
      throw new EBEncode(null, 'Cannot decode bencoded data.', 'string: '.$str);
    } else {
      return $res;
    }
  }

  static function Encode($data, $tempChar = null) {
    $res = bencode($data, $tempChar);
    if ($res === null) {
      throw new EBEncode(null, 'Cannot encode bencoded data of type '.gettype($data).'.');
    }
    return $res;
  }

  static function TypeOf($value) {
    if (is_scalar($value) or $value === null) {
      return ((is_int($value) or is_float($value)) ? 'int' : 'str');
    } else {
      return empty($value['isDct']) ? 'list' : 'dict';
    }
  }

  static function HashOf($nodes, $raw = false) {
    return strtoupper(sha1(self::Encode($nodes), $raw));
  }

  /* Instance methods */

  function __construct($str = null) {
    if ($str !== null) {
      $this->FromString($str);
    }
  }

  function FromString($str) {
    $nodes = self::Decode($str);

    if (!is_array($nodes)) {
      throw new EBEncode($this, 'Cannot load bencoded string - it decodes to'.
                                ' a non-array ('.gettype($nodes).').');
    }

    $this->nodes = &$nodes;
  }

    function FromFile($file) {
      $str = file_get_contents($file);
      if (!is_string($str)) {
        throw new EBEncode($this, 'File to load bencoded file from doesn\'t exist.', 'file: '.$file);
      }
      $this->FromString($str);
    }

  function ToString() {
    return self::Encode($this->nodes, $this->tempChar);
  }

    function ToFile($file) {
      $bytes = file_put_contents($file, $this->ToString, LOCK_EX);
      if (!is_int($bytes)) {
        throw new EBEncode($this, 'Cannot save bencoded file.', 'dest file: '.$file);
      }
      return $bytes;
    }

  // returns a shallow copy of root; to operate directly $this->nodes can be used.
  function Root() { return $this->nodes; }

  // returns null if node doesn't exist. $name = "/" or "" returns (sets/deletes) root.
  function ValueOf($name) { return $this->Alter($name); }
  // alias to ValueOf():
  function Get($name) { return $this->ValueOf($name); }

  function Set($name, $value) { return $this->Alter($name, 'set', $value); }
  function Copy($src, $dest) { return $this->Set($dest, $this->ValueOf($src)); }

  function Exchange($node_1, $node_2) {
    $temp = $this->ValueOf($node_2);
    $this->Set($node_2, $this->ValueOf($node_1));
    $this->Set($node_1, $temp);
  }

  function Delete($name) { return $this->Alter($name, 'delete'); }

    // $op: (g)et / (s)et, returns new value / (d)elete.
    protected function Alter($name, $op = 'get', $arg = null) {
      $lastSlash = strpbrk(mb_substr($name, -1), '\\/');

      $name = trim( strtr($name, '\\', '/'), '/' );
      $path = $name === '' ? array() : explode('/', $name);

      $parent = &$this->nodes;
      while ($path and is_array($parent)) {
        $value = &$parent[array_shift($path)];

        if ($op[0] === 'd') {
          if (!$path and $lastSlash == is_array($value)) {
            $value = null;
          }
        } elseif ($op[0] === 's') {
          if ($value === null and $path) {
            $value = array();
            if (( (string) $path[0] ) !== '0') {
              $value['isDct'] = true;
            }
          }
        }

        $parent = &$value;
      }

      if ($op[0] === 's') {
        $parent = $arg;
      } elseif ($op[0] === 'd' and !$name) {
        $parent = array();
      }

      return $parent;
    }

  function Export($name = '') { return $this->Dump( $this->ValueOf($name) ); }

    function Dump($value, $prefix = '') {
      $type = self::TypeOf($value);

      if ($type === 'int') {
        return is_float($value) ? sprintf('%1.1f', $value) : $value;
      } elseif ($type === 'str') {
        return var_export($value, true);
      } else {
        $res = '';

          $isDict = $type === 'dict';
          foreach ($value as $key => &$item) {
            if (!bskip($key, $item, $this->tempChar)) {
              $res .= $prefix;
              $res .= $isDict ? "$key:" : "#$key";
              $res .= is_array($item) ? "\n" : '  ';
              $res .= $this->Dump($item, "$prefix  ")."\n";
            }
          }

        return substr($res, 0, -1);
      }
    }

  // type: int|str|list|dict; other type throws exception.
  function NewNode($type, $name) {
    switch ($type = strtolower($type)) {
    case 'int':   return $this->Set($name, 0);
    case 'str':   return $this->Set($name, '');
    case 'list':  return $this->Set($name, array());
    case 'dict':  return $this->Set($name, array('isDct' => true));
    default:      throw new EBEncode($this, 'Cannot create bencoded node because type '.$type.' is unknown.');
    }
  }

  function SetEmpty($name) {
    $value = $this->ValueOf($name);

    if (is_int($value) or is_float($value)) {
      $value = 0;
    } elseif (is_string($value) or $value === null) {
      $value = '';
    } elseif (empty($value['isDct'])) {
      $value = array();
    } else {
      $value = array('isDct' => true);
    }

    return $this->Set($name, $value);
  }

  function Cast($name, $asType, $onlyIfNum = false) {
    $value = $this->ValueOf($name);
    if ($value === null) {
      throw new EBEncode($this, 'Cannot cast node '.$name.' into '.$asType.' because node doesn\'t exist.');
    }

    $asType = strtolower($asType);
    if (!in_array($asType, array('int', 'str', 'list', 'dict'))) {
      throw new EBEncode($this, 'Cannot cast node "'.$name.'" because new type ('.$asType.') is invalid.');
    }

    $type = self::TypeOf($value);
    if ($type !== $asType) {
      if ($type === 'int' or $type === 'str') {
        switch ($asType) {
        // str -> int:
        case 'int':
          if (!is_numeric($value)) {
            if (!$onlyIfNum) {
              throw new EBEncode($this, 'Cannot cast string "'.$value.' to integer because it\'s not a number.');
            }
          } else {
            $value = (float) $value;
          }

          break;

        // int -> str:
        case 'str':   $value = (string) $value; break;
        case 'list':  $value = array(0 => $value); break;
        case 'dict':  $value = array('isDct' => true, 0 => $value); break;
        }
      } elseif ($asType === 'int' or $asType === 'str') {
        throw new EBException($this, 'Casting list/dict node "'.$name.'" into int/str isn\'t allowed.');
      } elseif ($asType === 'dict') {   // list -> dict
        $value['isDct'] = true;
      } else {                          // dict -> list
        unset($value['isDct']);
      }

      $this->Set($name, $value);
    }

    return $value;
  }

  function TempChar($new = null) {
    $new === null or $this->tempChar = $new === '' ? null : $new;
    return $this->tempChar;
  }

  function InfoHash($raw = false) {
    $info = &$this->nodes['info'];
    if (empty($info)) {
      throw new EBEncode($this, 'Cannot calculate info hash because there is no \'info\' dictionary.');
    } else {
      return self::HashOf($info, $raw);
    }
  }
}

  class EBEncode extends Exception {
    public $obj;

    function __construct($bencObj, $msg, $details = '', $previous = null) {
      $this->obj = $bencObj;
      parent::__construct(rtrim($msg, '.').": $details", $previous);
    }
  }

/* lightenc.php */
function bdecode($s, &$pos=0) {
  if($pos>=strlen($s)) {
    return null;
  }
  switch($s[$pos]){
  case 'd':
    $pos++;
    $retval=array();
    while ($s[$pos]!='e'){
      $key=bdecode($s, $pos);
      $val=bdecode($s, $pos);
      if ($key===null || $val===null)
        break;
      $retval[$key]=$val;
    }
    $retval["isDct"]=true;
    $pos++;
    return $retval;

  case 'l':
    $pos++;
    $retval=array();
    while ($s[$pos]!='e'){
      $val=bdecode($s, $pos);
      if ($val===null)
        break;
      $retval[]=$val;
    }
    $pos++;
    return $retval;

  case 'i':
    $pos++;
    $digits=strpos($s, 'e', $pos)-$pos;
    // Proger_XP: changed (int) -> (float) to avoid trimming of values exceeding
    //            signed int's max value (2147483647).
    $val=(float)substr($s, $pos, $digits);
    $pos+=$digits+1;
    return $val;

//  case "0": case "1": case "2": case "3": case "4":
//  case "5": case "6": case "7": case "8": case "9":
  default:
    $digits=strpos($s, ':', $pos)-$pos;
    if ($digits<0 || $digits >20)
      return null;
    $len=(float)substr($s, $pos, $digits);
    $pos+=$digits+1;
    $str=substr($s, $pos, $len);
    $pos+=$len;
    //echo "pos: $pos str: [$str] len: $len digits: $digits\n";
    return (string)$str;
  }
  return null;
}

// Proger_XP: added added skipping for null values and $tempChar prefix for list/dicts.
function bencode(&$d, $tempChar = null){
  if(is_array($d)){
    $ret="l";
    $isDict=!empty($d["isDct"]);
    if($isDict){
      $ret="d";
      // this is required by the specs, and BitTornado actualy chokes on unsorted dictionaries
      ksort($d, SORT_STRING);
    }
    foreach($d as $key=>$value) {
      if($isDict){
        // skip the isDct element, only if it's set by us
        if (!bskip($key, $value, $tempChar)) {
          $ret .= strlen($key).":$key";
        }
      } elseif (!is_int($key) and !is_float($key) and trim($key, '0..9') !== '') {
                // Proger_XP: added exception raising for non-numeric list keys.
                throw new EBEncode(null, 'Cannot bencode() a list - it contains a non-numeric key "'.$key.'".');
            }

      if (is_string($value)) {
        $ret.=strlen($value).":".$value;
      } elseif (is_int($value) or is_float($value)){
        $ret.="i${value}e";
      } else {
        $ret.=bencode ($value);
      }
    }
    return $ret."e";
  } elseif (is_string($d)) { // fallback if we're given a single bencoded string or int
    return strlen($d).":".$d;
  } elseif (is_int($d) or is_float($d)) {
    return "i${d}e";
  } else {
    return null;
  }
}

// bskip by Proger_XP.
function bskip($key, &$value, $tempChar = null) {
  return ($key === 'isDct' and $value) or $value === null or strpos($key, $tempChar) === 0;
}
