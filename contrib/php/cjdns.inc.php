<?php
/*
 CJDNS admin interface for PHP. See index.php for sample usage
 https://github.com/thefinn93/cjdns/ contrib/php/

 20120718 ircerr@EFNet
 Partially Moded for fscockopen vs socket_create for compatability

*/

require_once("bencode.inc.php");

class cjdns {
    public $conn;
    public function connect() {
        global $ip, $port;
        $conn = fsockopen($ip, $port, $errno, $errstr, 10);
        if (!$conn)
           {die("Failed to connect to admin interface: $errstr ($errno) ");}

        // Connection established. Just gotta make sure it's a CJDNS admin interface we're connected to:
        fwrite($conn, "d1:q4:pinge\r\n");
        if(fread($conn, 11) != "d1:q4:ponge") {
            die("Connected to non CJDNS port");
        }
        $this->conn = $conn;
    }

    public function functionlist() {
        $be = new BEncoded;
        $this->write("d1:q7:invalide");
        $list = $be->Decode($this->read());
        $list = $list["availableFunctions"];
        foreach($list as $function=>$info) {
            if($function != "isDct") {
                $args = array();
                foreach($info as $name=>$type) {
                    if($name != "isDct") {
                        $args[] = "\"".$name."\" => ".$type["type"];
                    }
                }
                print $function."(".implode(", ", $args).");\n";
            }
        }
    }

    public function call($function, $args) {
        global $password;
        $be = new BEncoded;
        $args['isDct'] = TRUE;

        // Get a cookie
        $this->write("d1:q6:cookiee");
        $cookie = $be->Decode($this->read());
        $cookie = $cookie['cookie'];

        // Build the request
        $req = array("q"=>"auth","aq"=>$function,"hash"=>hash("sha256",$password.$cookie), "cookie"=>$cookie,"args"=>$args,"isDct" => TRUE);

        // Do CJD's weird ass hashing thingy
        $hash = hash("sha256",$be->Encode($req));
        $req['hash'] = $hash;
        $this->write($be->Encode($req));
        return $be->Decode($this->read());
    }

    public function read() {
        $reply = fread($this->conn, 69632);
        return $reply;
    }

    public function write($data) {
        if(!fwrite($this->conn, "$data\r\n")) {
            die("Please connect before making function calls: $errstr ($errno) ");
        }

    }
    public function disconnect() {
          fclose($this->conn);
    }
}
?>
