# Admin panel for CJDNS
#### Does not support Internet Explorer 6, 7, or 8

### Usage
##### Node.js

You should install [Node.js](http://nodejs.org) first.

##### Install dependencies

    cd {cjdns-path}/contrib/nodejs/admin
    nmp install

##### Create ~/.cjdnsadmin

    echo '{"addr":"127.0.0.1","port":11234,"password":"your admin password from cjdroute.conf"}' > ~/.cjdnsadmin

##### Start service

    node admin.js

##### Whoa!
Open `http://localhost:8084` in your favorite browser. Hope, it's not IE < 9.