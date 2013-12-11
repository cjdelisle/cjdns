# nThen... Do this... nThen... Do that.

nThen is designed to be the most small, simple and intuitive asynchronous library.

## How you used to do things

    x = synchronous_call();
    y = use(x);
    a = another_sync_call(x);          # Hey!
    b = yet_another_sync_call(x);      # these two could run at the same time!
    z = something_else_synchronous(b);
    c = last_synchronous_call(z);
    do_next_job(c);

Ok so we learned that doing it that way is slow and we have asynchronous so we never have to be slow anymore.

## The naive asynchronous approach

    var context = {};
    async_call(function(x) {
        context.y = use(x);
        another_call(x, function(a) {
            context.a = a;
            if (context.c) { do_next_job(context); }
        });
        yet_another_call(x, function(b) {
            something_else(b, function(z) {
                last_call(z, function(c) {
                    context.c = c;
                    if (context.a) { do_next_job(context); }
                });
            });
        });
    });

That doesn't look like very much fun :(
And to make matters worse, what happens if one of those functions never returns?

    var to = setTimeout(function() { abort_process(); }, 3000);

You can see where this is going.


## nThen to the rescue

    var nThen = require('nthen');
    var context = {};
    nThen(function(waitFor) {
        // Remember to wrap all of your async callbacks with waitFor()
        // otherwise they won't block the next nThen block.
        asyncCall(waitFor(function(x) {
            context.x = x;
        }));
    }).nThen(function(waitFor) {
        // These two functions run at the same time :D
        anotherCall(x, waitFor(function(a) {
            context.a = a;
        }));
        yetAnotherCall(x, waitFor(function(b) {
            context.b = b;
        }));
    }).nThen(function(waitFor) {
        somethingElse(b, waitFor(function(z) {
            context.z = z;
        }));
    }).nThen(function(waitFor) {
        lastCall(z, waitFor(function(c) {
            context.c = c;
        }));
    }).nThen(function(waitFor) {

        doNextJob(context);

    }).orTimeout(function(waitFor) {

        // Using the orTimeout() function, you can chain a failure timeout.
        abortProcess();

    }, 3000).nThen(function(waitFor) {
        // Finally, you can chain more nThen calls after the timeout
        // these will be excuted regardless of whether the chain times out or not.
        // think of ajax.onComplete
        someFinalCleanup();
    });


## Chaining is Allowed

This is perfectly ok, your second call to waitFor() will cause the second function
to block the entry of the next nThen block and it will perform as expected.

    nThen(function(waitFor) {
        asyncCall(waitFor(function(x) {
            iWouldReallyRatherChainThisCall(x, waitFor(function(y) {
                context.y = y;
            }));
        }));
    }).nThen(function(waitFor) {

## Disclamer

The variable names used in this readme are only for example and to make you mad.

## License

Public Domain

## ChangeLog

* 1.0 The first versions, practically perfect in every way.
* 1.1 Ok maybe practically perfect was a stretch, fixed a bug which causes incorrect behavior if
the the first nThen function returns synchronously.

    nThen(function(waitFor) {
        console.log("this function returns synchronously");
    }).nThen(function(waitFor) {
        console.log("This never gets executed because nThen 1.0 has a bug :(");
    });
