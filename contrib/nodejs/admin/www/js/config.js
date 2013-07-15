(function (window, $) {
    App.prototype.init = function showConfig () {
        var app = this,
            peerContainer = $('#peers'),
            configOutput = $('#configOutput');

        $('#save').click(function () {
            app.send('/config', {
                config: app.config
            }, function (msg) {
                configOutput.empty();
                app.updateOutput(msg, configOutput);
            });
        });

        app.send('/config', function (config) {
            var host,
                peers = config.UDPInterface[0].connectTo,
                html = '';

            app.config = config;

            for (host in peers) {
                if (peers.hasOwnProperty(host)) {
                    new Peer(peers, host, peerContainer);
                }
            }

            $('#add').click(function () {
                new Peer(app.config.UDPInterface[0].connectTo, '', peerContainer);
            });
        });
    };

    var app = new App(),
        peerTmpl = app.render([
            '<div class="peer">',
                '<div>',
                    '<strong class="peer-host-block">',
                        '<input class="peer-input input-medium peer-host" value="%host%" placeholder="xxx.xxx.xxx.xxx" />',
                    '</strong>',
                    '<span class="peer-delete btn btn-danger">delete</span>',
                    '<span class="btn-group">',
                        '<span class="peer-revert btn btn-success">undo</span>',
                        '<span class="peer-delete-forever btn btn-danger">delete forever</span>',
                    '</span>',
                '</div>',
                '<div class="peer-info">',
                    '<div><label>password = <input class="peer-input input-xxlarge peer-password" value="%password%" /></label></div>',
                    '<div><label>public key = <input class="peer-input input-xxlarge peer-key" value="%publicKey%" /></label></div>',
                '</div>',
            '</div>'
        ].join(''));

    var Peer = function (peers, host, container) {
        this.peers = peers;
        this.host = host;
        this.peer = peers[host] || {};
        this.makeHtml();

        container.append(this.node);

        this.bindEvents();
    };

    Peer.prototype.makeHtml = function () {
        this.node = $(peerTmpl({
            host: this.host,
            password: this.peer.password || '',
            publicKey: this.peer.publicKey || ''
        }));
    };

    Peer.prototype.bindEvents = function () {
        var peer = this,
            host,
            pass,
            publicKey;

        host = this.node.find('.peer-host').change(function () {
            var newHost = $(this).val();

            if (!peer.peers[newHost]) {
                delete peer.peers[peer.host];
                peer.host = newHost;
                peer.peers[newHost] = {
                    password: pass.val() || '',
                    publicKey: publicKey.val() || ''
                };

                peer.peer = peer.peers[newHost];
            }
        });

        pass = this.node.find('.peer-password').change(function () {
            peer.peer.password = $(this).val();
        });

        publicKey = this.node.find('.peer-key').change(function () {
            peer.peer.publicKey = $(this).val();
        });

        this.node.find('.peer-delete').click(function () {
            delete peer.peers[peer.host];

            peer.node.addClass('peer-removed');
        });

        this.node.find('.peer-delete-forever').click(function () {
            peer.node.remove();
        });

        this.node.find('.peer-revert').click(function () {
            host.trigger('change');

            peer.node.removeClass('peer-removed');
        });

        if (host.val() === '') {
            host.focus();
        }
    };
} (window, jQuery));