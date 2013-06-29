(function (window, $) {
    var output,
        gLogs = [];

    function init () {
        var $command = $('#command'),
            $param = $('#param');

        output = $('#output');

        $('#clear').click(function () {
            output.empty();
        });

        $('#sendForm').submit(function (e) {
            var data = {},
                param = $param.val();

            e.preventDefault();
            e.stopPropagation();

            data.q = $command.val();
            if (param) {
                param = param.split('=');
                data.args = {};
                data.args[param[0]] = parseInt(param[1], 10);

                if (isNaN(data.args[param[0]]) || data.args[param[0]] != param[1]) {
                    data.args[param[0]] = param[1];
                }
            }

            send('/cjdns', data, function (res) {
                //$command.val('');
                //$param.val('');
            });
        });

        //Check if backend working
        send({lorem: 'ipsum'}, function (res) {
            if (!res.hello || res.hello !== 'world!') {
                alert('Check your node.js backend!');
            }
        });

        if (window.isMap) {
            createMap();
        }

        if (window.isLogs) {
            connectToLogs();
        }

        if (window.isConfig) {
            showConfig();
        }

    }

    function updateOutput (msg) {
        var time = msg && msg.time ? new Date(msg.time * 1000) : new Date(),
            text = [];

        function fillZero (num) {
            if (num < 10) {
                return '0' + num;
            }

            return num;
        }

        text.push(fillZero(time.getHours()));
        text.push(fillZero(time.getMinutes()));
        text.push(fillZero(time.getSeconds()));

        text = '<strong>[' + text.join(':') + ']</strong> ';
        text += JSON.stringify(msg);

        text = '<div class="new-message">' + text + '</div>';

        if (output) {
            output.find('.new-message').removeClass('new-message');
            output.prepend(text);
        }

        return text;
    }

    function createMap () {
        var map = $('#map');

        send('/map', {}, function (resp) {
            var nodes = resp.nodes;

            if (nodes) {
                nodes = nodes.map(function (node) {
                    var txt = '<tr>';

                    txt += '<td>' + node.ip + '</td>';
                    txt += '<td>' + node.link + '</td>';
                    txt += '<td>' + node.path + '</td>';
                    txt += '<td>' + node.version + '</td>';
                    txt += '</tr>';

                    return txt;
                });

                map.html(nodes.join(''));
            }
        });
    }

    function connectToLogs () {
        var $logs = $('#logs'),
            logTabs = {
                'ALL': $('#all'),
                'KEYS': $('#keys'),
                'DEBUG': $('#debug'),
                'INFO': $('#info'),
                'WARN': $('#warn'),
                'ERROR': $('#error'),
                'CRITICAL': $('#critical')
            },
            logCount = {
                'ALL': 0,
                'KEYS': 0,
                'DEBUG': 0,
                'INFO': 0,
                'WARN': 0,
                'ERROR': 0,
                'CRITICAL': 0
            },
            pause = $('#pause'),
            ts = (new Date()).getTime() - 60000;

        function makeHtml (msg) {
            var time = msg && msg.time ? new Date(msg.time * 1000) : new Date(),
            text = [];

            function fillZero (num) {
                if (num < 10) {
                    return '0' + num;
                }

                return num;
            }

            text.push(fillZero(time.getHours()));
            text.push(fillZero(time.getMinutes()));
            text.push(fillZero(time.getSeconds()));

            text = '<strong>[' + text.join(':') + ']</strong> ';
            text += msg.level;
            text += ' <i>' + msg.file + (msg.line ? ':' + msg.line : '') + '</i> ';
            text += msg.message;

            return '<div class="new-message">' + text + '</div>';
        }

        function getNewLogs() {
            if (pause.is('.active')) {
                window.setTimeout(getNewLogs, 1000);
                return;
            }

            send('/logs', {ts:ts}, function (resp) {
                var logs;

                if (resp.logs && resp.logs.length > 0) {
                    gLogs = gLogs.concat(resp.logs);

                    resp.logs.forEach(function (log) {
                        var logHtml = makeHtml(log);

                        logTabs[log.level].prepend(logHtml);
                        logTabs.ALL.prepend(logHtml);

                        logCount[log.level] += 1;
                        logCount.ALL += 1;
                    });

                    $('#countALL').html('(' + logCount.ALL + ')');
                    $('#countKEYS').html('(' + logCount.KEYS + ')');
                    $('#countDEBUG').html('(' + logCount.DEBUG + ')');
                    $('#countINFO').html('(' + logCount.INFO + ')');
                    $('#countWARN').html('(' + logCount.WARN + ')');
                    $('#countERROR').html('(' + logCount.ERROR + ')');
                    $('#countCRITICAL').html('(' + logCount.CRITICAL + ')');
                }

                if (resp.ts) {
                    ts = resp.ts;
                }

                window.setTimeout(getNewLogs, 1000);
            });
        }

        getNewLogs();
    }

    function showConfig () {
        function drawNode (data) {
            return ['<div class="peerNode">',
                        '<div><strong>' + data.host + '</strong> password = ' + (data.password || '<empty password>') + '</div>',
                        '<div>public key = ' + data.publicKey + '</div>',
                    '</div>'].join('');
        }

        send('/config', {}, function (config) {
            var host,
                peers = config.interfaces.UDPInterface[0].connectTo,
                html = '';

            for (host in peers) {
                if (peers.hasOwnProperty(host)) {
                    peers[host].host = host;

                    html += drawNode(peers[host]);
                }
            }

            $('#peers').html(html);
        });
    }

    function send (url, msg, callback) {
        if (typeof(url) === 'object') {
            callback = msg;
            msg = url;
            url = '';
        }

        $.ajax({
            url: '/api' + url,
            type: 'POST',
            contentType: 'application/json',
            dataType: 'json',
            data: JSON.stringify(msg),
            success: function (res) {
                updateOutput(res);

                if (callback) {
                    callback(res);
                }
            }
        });
    }

    $(init);
} (window, jQuery));