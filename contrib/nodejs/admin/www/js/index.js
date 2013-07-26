(function (window, $) {
    var $command = $('#command'),
        $params = $('#paramsContainer');


    App.prototype.init = function showConfig () {
        var app = this;

        $command = $('#command').val('');
        $params = $('#paramsContainer');

        $('#sendForm').submit(function (e) {
            var data = {},
                params = $params.find('.param-input');

            e.preventDefault();
            e.stopPropagation();

            data.q = $command.val();

            if (!data.q) {
                $params.addClass('new-message');
                return;
            }
            $params.removeClass('new-message');

            params.each(function () {
                var param = $(this),
                    val = param.val(),
                    name = param.attr('name');

                if (val !== '') {
                    data.args = data.args || {};
                    data.args[name] = param.attr('type') === 'number' ? parseInt(val, 10) : val;
                }
            });

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
            paramTmpl = app.render([
                '<div><label>',
                    '<span class="span3">%required%%name%%requiredClose%</span>',
                    '<input class="param-input" name="%name%" value="" type="%type%" class="input-medium" placeholder="%paramtype%" />',
                '</label></div>'].join('')),
            paramTypes = {
                'String': 'text',
                'Int': 'number'
            };

        app.send('/methods', function (resp) {
            var methods = resp.methods,
                methodsSelect = [],
                methodParams = {};

            if (methods) {
                methodsSelect = methods.map(function (method, i) {
                    var param,
                        params = [];

                    methodParams[method.name] = [];

                    for (param in method.params) {
                        if (method.params.hasOwnProperty(param)) {
                            params.push('<em>' + method.params[param].type + '</em> ' + param + (method.params[param].required == 1 ? ', <em>required</em>' : ''));

                            methodParams[method.name].push({
                                name: param,
                                type: method.params[param].type,
                                required: method.params[param].required == 1
                            });
                        }
                    }

                    return {
                        id: method.name,
                        text: method.name,
                        params: (params.join('<br />') || '<em>no params</em>')
                    };
                });
            }

            function format (item) {
                return [
                        '<div><strong>',
                            item.id,
                        '</strong></div>',
                        '<div class="select-offset">',
                            item.params,
                        '</div>'
                    ].join('');
            }

            $command.select2({
                data: methodsSelect,
                formatResult: format,
                formatSelection: format
            }).on('change', function (e) {
                var method = e.val,
                    params = methodParams[method],
                    paramsHtml = [];

                $params.removeClass('new-message');
                $params.empty();
                params.forEach(function (param) {
                    paramsHtml.push(paramTmpl({
                        name: param.name,
                        type: paramTypes[param.type] || 'text',
                        paramtype: param.type,
                        required: param.required ? '<strong title="required">' : '',
                        requiredClose: param.required ? '*</strong>' : ''
                    }));
                });

                paramsHtml = paramsHtml.length > 0 ? paramsHtml.join('') : 'No params accepted';
                $params.html(paramsHtml);
            }).select2('enable', true);
        });
    };

    new App();
} (window, jQuery, App));