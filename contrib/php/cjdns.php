<?
/*
CJDNS admin interface for PHP. See index.php for sample usage
*/
require_once("bencode.php");
class cjdns {
    public $conn;
    private $password;
    public function connect($password, $ip="127.0.0.1", $port=11234)
        {
        $conn = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
        $this->password = $password;
        if(!socket_connect($conn, $ip, $port))
           {die("Failed to connect to admin interface: ".socket_strerror(socket_last_error()));}

        // Connection established. Just gotta make sure it's a CJDNS admin interface we're connected to:
        socket_write($conn, "d1:q4:pinge");
        if(socket_read($conn, 11) != "d1:q4:ponge")
            {die("Connected to non CJDNS port");}
        $this->conn = $conn;
        }

    public function functionlist()
        {
        $be = new BEncoded;
        $this->write("d1:q7:invalide");
        return $be->Decode($this->read());
        }
    public function call($function, $args)
        {
        $be = new BEncoded;
        $args['isDct'] = TRUE;

        // Get a cookie
        $this->write("d1:q6:cookiee");
        $cookie = $be->Decode($this->read());
        $cookie = $cookie['cookie'];

        // Build the request
        $req = array("q"=>"auth","aq"=>$function,"hash"=>hash("sha256",$this->password.$cookie), "cookie"=>$cookie,"args"=>$args,"isDct" => TRUE);

        // Do CJD's weird ass hashing thingy
        $hash = hash("sha256",$be->Encode($req));
        $req['hash'] = $hash;
        $this->write($be->Encode($req));
        return $be->Decode($this->read());
        }

    public function read()
        {
        $reply = socket_read($this->conn, 69632);
        return $reply;
        }

    public function write($data)
        {
        if(!socket_write($this->conn, $data))
            {die("Please connect before making function calls: ".socket_strerror(socket_last_error()));}
        }
    public function disconnect()
        {
        socket_close($this->conn);
        }
    }
?>
