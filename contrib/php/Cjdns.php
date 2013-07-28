<?

class Cjdns {
    public $buffersize = 69632;
    public $keepalive = 2;
    public $functions;
    private $socket;
    private $password;
    private $responses = array();

    public static function randStr() {
        $output = "";
        $chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
        for($i = 0; $i < 10; $i++) {
            $output .= $chars[rand(0, strlen($chars)-1)];
        }
        return $output;
    }
    
    function receive($txid) {
        while(!isset($this->responses[$txid])) {
            $data = fread($this->socket, $this->buffersize);
            if($data != "") {
                try {
                    $decoded = bdecode($data);
                }
                catch(Exception $e) {
                    die("Failed to decode: ".$data);
                }
            }
            $this->responses[$decoded['txid']] = $decoded;
        }
        $response = $this->responses[$txid];
        unset($this->response[$txid]);
        return $response;
    }

    public function send_raw($message) {
        $txid = $this->randStr();
        if(!isset($message['txid'])) {
            $message['txid'] = $txid;
        } else {
            $txid = $message['txid'];
        }
        fwrite($this->socket, bencode($message));
        return $txid;
    }
    
    private function getCookie() {
        $data = $this->receive($this->send_raw(array("q" => "cookie")));
        return $data['cookie'];
    }

    public function call($function, $args) {
        $cookie = $this->getCookie();
        $txid = $this->randStr();
        $request = array("q" => "auth",
            "aq" => $function,
            "hash" => hash("sha256", $this->password.$cookie),
            "cookie" => $cookie,
            "args" => $args,
            "txid" => $txid
            );
        $requestBencoded = bencode($request);
        $request['hash'] = hash("sha256", $requestBencoded);
        $this->send_raw($request);
        return $this->receive($txid);
    }

    function __construct($password, $host="127.0.0.1", $port=11234) {
        $this->socket = stream_socket_client("udp://".$host.":".$port, $errorno, $errorstr);
        if(!$this->socket) {
            die("Failed to connect, Error #$errorno: $errorstr");
        }
        fwrite($this->socket, bencode(array("q"=>"ping")));   // Try to ping it
        $returndata = fread($this->socket, $this->buffersize);
        if($returndata != "d1:q4:ponge") {
            die("Looks like $host:$port isn't a cjdns admin port");
        }
        $this->password = $password;
        $page = 0;
        while(True) {
            $request = array("q" => "Admin_availableFunctions",
                "args" => array("page" => $page));
            fwrite($this->socket, bencode($request));
            $result = bdecode(fread($this->socket, $this->buffersize));
            foreach($result['availableFunctions'] as $function => $description) {
                $this->functions[$function] = $description;
            }
            if(isset($result['more'])) {
                $page++;
            } else {
                break;
            }
        }
    }
    
    function __destructor() {
        socket_close($this->socket);
    }
}
