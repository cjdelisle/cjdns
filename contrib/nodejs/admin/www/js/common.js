var App = function () {
    var app = this;

    this.templates = {
        outputTmpl: this.render('<div class="new-message"><strong>[%time%]</strong> %msg%</div>')
    };

    $(function () {
        app.create();
    });
};

App.prototype.create = function () {
    var app = this;

    this.output = $('#output');

    //Check if backend working
    this.send({lorem: 'ipsum'}, function (res) {
        if (!res.hello || res.hello !== 'world!') {
            alert('Check your node.js backend!');
        }
    });

    $('#clear').click(function () {
        app.output.empty();
    });

    this.init();
};

App.prototype.send = function send (url, msg, callback) {
    var app = this;

    if (typeof(url) === 'object') {
        callback = msg;
        msg = url;
        url = '';
    }

    if (typeof(msg) === 'function') {
        callback = msg;
        msg = undefined;
    }

    $.ajax({
        url: '/api' + url,
        type: 'POST',
        contentType: 'application/json',
        dataType: 'json',
        data: msg ? JSON.stringify(msg) : '',
        success: function (res) {
            app.updateOutput(res);

            if (callback) {
                callback(res);
            }
        }
    });
};

App.prototype.updateOutput = function updateOutput (msg, container) {
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

    text = this.templates.outputTmpl({
        time: text.join(':'),
        msg: JSON.stringify(msg)
    });

    container = container || this.output;

    if (container) {
        container.find('.new-message').removeClass('new-message');
        container.prepend(text);
    }

    return text;
};

App.prototype.render = function render(template) {
    if (!template || typeof(template) !== 'string') {
        throw 'Template must be a string';
    }

    return function (params) {
        var p,
            html = template;

        if (params) {
            for (p in params) {
                if (params.hasOwnProperty(p)) {
                    html = html.replace(
                        (new RegExp('%' + p + '%', 'g')),
                        params[p]
                    );
                }
            }
        }

        return html;
    };
};