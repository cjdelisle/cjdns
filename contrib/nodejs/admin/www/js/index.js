(function (window, $) {
    var $command = $('#command'),
        $param = $('#param');

    App.prototype.init = function showConfig () {
        var app = this;

        $command = $('#command');
        $param = $('#param');

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

            app.send('/cjdns', data, function (res) {
                //$command.val('');
                //$param.val('');
            });
        });

        this.getMethods();
    };

    App.prototype.getMethods = function getMethods () {
        var app = this,
            $methods = $('#methods'),
            methodTmpl = app.render([
                '<td>',
                    '<a href="#fillForm" data-method="%name%" class="method">%name%</a>',
                '</td>'].join(''));

        app.send('/methods', function (resp) {
            var methods = resp.methods;

            if (methods) {
                methods = methods.map(function (method) {
                    var txt = '<tr>',
                        param,
                        params = [];

                    txt += methodTmpl({
                        name: method.name
                    });

                    for (param in method.params) {
                        if (method.params.hasOwnProperty(param)) {
                            params.push('<em>' + method.params[param].type + '</em> ' + param + (method.params[param].required == 1 ? ', <strong>required</strong>' : ''));
                        }
                    }

                    txt += '<td>' + (params.join('<br />') || '<em>none</em>') + '</td>';
                    txt += '</tr>';

                    return txt;
                });

                $methods.html(methods.join(''));

                $methods.find('.method').click(function (e) {
                    $command.val($(this).data('method'));
                });
            }
        });
    };

    new App();
} (window, jQuery, App));