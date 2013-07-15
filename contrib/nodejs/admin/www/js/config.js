(function (window, $) {
    App.prototype.init = function showConfig () {
        var nodeTmpl = app.render([
                '<div class="peer">',
                    '<div><strong>%host%</strong></div>',
                    '<div class="peer-info">',
                        '<div>password = %password%</div>',
                        '<div>public key = %publicKey%</div>',
                    '</div>',
                '</div>'
            ].join(''));

        app.send('/config', function (config) {
            var host,
                peers = config.interfaces.UDPInterface[0].connectTo,
                html = '';

            for (host in peers) {
                if (peers.hasOwnProperty(host)) {
                    peers[host].host = host;

                    html += nodeTmpl(peers[host]);
                }
            }

            $('#peers').html(html);
        });
    };

    var app = new App();
} (window, jQuery));