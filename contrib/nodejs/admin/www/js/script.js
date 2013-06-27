(function (window, $) {
    function init () {
        //Check if backend working
        $.post('/api', {lorem: 'ipsum'}, function (res) {
            if (!res.hello || res.hello !== 'world!') {
                alert('Check your node.js backend!');
            }
        });
    }

    $(init);
} (window, jQuery));