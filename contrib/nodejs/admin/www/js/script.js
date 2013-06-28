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

            send(data, function (res) {
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
        output.find('.new-message').removeClass('new-message');
        output.prepend('<div class="new-message">' + text + '</div>');
    }

    function send (msg, callback) {
        $.ajax({
            url: '/api',
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