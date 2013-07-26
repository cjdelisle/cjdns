(function (window, $) {
    var gLogs = [];

    App.prototype.init = function connectToLogs () {
        var app = this,
            $logs = $('#logs'),
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
            ts = (new Date()).getTime() - 60000,
            tmpl = this.render([
                '<div class="new-message message">',
                    '<strong>[%time%]</strong> %level% ',
                    '<i>%file%%line%</i> ',
                    '<span class="message-text">%msg%</span>',
                '</div>'].join(''));

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

            return tmpl({
                time: text.join(':'),
                level: msg.level,
                file: msg.file,
                line: msg.line ? ':' + msg.line : '',
                msg: msg.message
            });
        }

        function getNewLogs() {
            if (pause.is('.active')) {
                window.setTimeout(getNewLogs, 1000);
                return;
            }

            app.send('/logs', {ts:ts}, function (resp) {
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
    };

    new App();
} (window, jQuery));