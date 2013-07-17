(function (window, $) {
    App.prototype.init = function () {
        var map = $('#map'),
            nodesTmpl = this.render([
                '<tr>',
                    '<td>%ip%</td>',
                    '<td>%link%</td>',
                    '<td>%path%</td>',
                    '<td>%version%</td>',
                '</tr>'
            ].join(''));

        this.send('/map', function (resp) {
            var nodes = resp.nodes;

            if (nodes) {
                nodes = nodes.map(nodesTmpl);

                map.html(nodes.join(''));
            }
        });
    };

    new App();
} (window, jQuery));