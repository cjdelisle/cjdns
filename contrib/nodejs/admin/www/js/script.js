(function (window, $) {
    var output;

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
    }

    function updateOutput (msg) {
        var time = new Date(),
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

                map.html('<table>' + nodes.join('') + '</table>');
            }
        });
    }

    function connectToLogs () {
        var $logs = $('#logs'),
            ts = (new Date()).getTime() - 3600000;

        function getNewLogs() {
            send('/logs', {ts:ts}, function (resp) {
                var logs;

                if (resp.logs && resp.logs.length > 0) {
                    logs = resp.logs.map(function (log) {
                        return updateOutput(log, true);
                    });

                    $logs.append(logs.join(''));
                }

                if (resp.ts) {
                    ts = resp.ts;
                }

                window.setTimeout(getNewLogs, 1000);
            });
        }

        getNewLogs();
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