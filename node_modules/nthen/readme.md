# nThen... Do this... nThen... Do that.

nThen is designed to be the most small, simple and intuitive asynchronous library.

## How you used to do things

```javascript
let x = synchronous_call();
let y = use(x);
let a = another_sync_call(x);          // Hey!
let b = yet_another_sync_call(x);      // these two could run at the same time!
let z = something_else_synchronous(b);
let c = last_synchronous_call(z);
do_next_job(c);
```

Ok so we learned that doing it that way is slow and we have asynchronous so we never have to be slow anymore.

## The naive asynchronous approach

```javascript
asyncCall(function (x) {
    let y = use(x);
    let aa;
    let cc;
    anotherCall(x, function (a) {
        aa = a;
        if (cc) { doNextJob(cc); }
    });
    yetAnotherCall(x, function (b) {
        somethingElse(b, function (z) {
            lastCall(z, function (c) {
                cc = c;
                if (aa) { doSomethingElse(aa); }
            });
        });
    });
});
```

That doesn't look like very much fun :(
And to make matters worse, what happens if one of those functions never returns?

```javascript
var to = setTimeout(function() { stopEverything(); }, 3000);
```

You can see where this is going.


## nThen to the rescue

nThen allows you to do things in parallel by putting them in *blocks* and one block will not
execute until all of the callbacks from the previous one have completed.

How do we know when all callbacks have been completed ? You *wrap* them using the waitFor
function wrapper. Waitfor wraps a function, so the result of waitFor is another function and
when that is called, all arguments are passed through to the function which was given to waitFor,
however, nThen will block until all waitFors have been called.

You can use waitFor as many times as you want in an nThen block, you can even use a waitFor
invocation inside of another waitFor invocation and it will keep the nThen block from terminating.

 For example this works:

```javascript
const nThen = require('nthen');
const Fs = require('fs');
nThen(function (waitFor) {
    Fs.readdir('/dev', waitFor(function (err, array) {
        if (err) { throw err; }
        if (array.indexOf('random') > -1) {
            Fs.stat('/dev/random', waitFor(function (err, stat) {
                if (err) { throw err; }
                console.log(stat);
            }));
        }
    }));
}).nThen(function (waitFor) {
    console.log('completed');
});
```

### Using nThen to sequencialize a list of async operations

Sometimes you want to do a variable number of things in sequence. Suppose you wanted to stat
every file in the /dev directory but you want to do them one at a time.

You can loop over all files using nThen to access each one sequencially like this:

```javascript
const nThen = require('./index.js');
const Fs = require('fs');
var nt = nThen;
Fs.readdir('/dev', function (err, array) {
    if (err) { throw err; }
    array.forEach(function (x) {
        nt = nt(function (waitFor) {
            Fs.stat('/dev/' + x, waitFor(function (err, stat) {
                if (err) { throw err; }
                console.log(stat);
            }));
        }).nThen;
    });
    nt(function (waitFor) {
        console.log('done');
    });
});
```

### Nested nThen

You can use multiple levels of nesting of nThen, for example you might use a loop of async
operations inside of one block of larger nThen chain, like this. Notice waitFor can be
used without a wrapped function.

```javascript
const nThen = require('./index.js');
const Fs = require('fs');
let fileList;
nThen(function (waitFor) {
    Fs.readdir('/dev', waitFor(function (err, array) {
        if (err) { throw err; }
        fileList = array;
    }));
}).nThen(function (waitFor) {
    let nt = nThen;
    fileList.forEach(function (x) {
        nt = nt(function (waitFor) {
            Fs.stat('/dev/' + x, waitFor(function (err, stat) {
                if (err) { throw err; }
                console.log(stat);
            }));
        }).nThen;
    });
    // This line is what will hold open this nThen block until
    // all of the files in fileList have been stated.
    nt(waitFor());
}).nThen(function (waitFor) {
    console.log('done');
});
```

### orTimeout

You can end your nThen chain with an orTimeout call which will abort the execution of the nThen
chain if the execution takes more than a given number of milliseconds.

```javascript
const nThen = require('./index.js');
nThen(function (waitFor) {
    let done = waitFor(function () { console.log('this never gets invoked'); });
    if (1 === 2) {
        // never happens
        done();
    }
}).nThen(function (waitFor) {
    console.log('so we never get to this point');
}).orTimeout(function () {
    console.log('timed out after 1 second');
}, 1000);
```


### Details

* If you do not call waitFor at all within an nThen block, the block will exit synchronously
* You can use waitFor without wrapping any callback function, it works fine with nothing
* nThen is fully ECMA-5 compatible, it is also compatible with flow typing system
* nThen does operations in sequence (no parallelism), if you'd like to do *n* parallel operations
check out [saferphore](https://github.com/cjdelisle/saferphore)


## License

Public Domain or MIT license, as you wish.

## Contact

[@cjd@mastodon.social](https://mastodon.social/@cjd)