# Admin panel for cjdns
#### Does not support Internet Explorer 6, 7, or 8

### Usage
##### Node.js

You should install [Node.js](http://nodejs.org) first.

##### Install dependencies

    cd {cjdns-path}/contrib/nodejs/admin
    npm install

##### Create ~/.cjdnsadmin

You should create ~/.cjdnsadmin file with path to your cjdns config:

    echo '{"config":"path to your config file"}' > ~/.cjdnsadmin

##### Start service

    node admin.js

##### Whoa!
Open `http://localhost:8084` in your favorite browser. Hope, it's not IE < 9.
