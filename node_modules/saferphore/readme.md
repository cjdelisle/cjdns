# Saferphore - Node semaphore with protection against double-returning

    # node
    npm install --save saferphore

    # Browser
    bower install --save saferphore


```javascript
const Saferphore = require('saferphore');

var sem = Saferphore.create(4);
for (var i = 0; i < 10000; i++) {
    sem.take(function (returnAfter) {
        Fs.writeFile('file_' + i, 'hi', returnAfter(function (err) {
            if (err) { throw err; }
        });
    });
}
```

You can only return what you take, if you try to call returnAfter() twice then it will throw a
clear error instead of creating a leaky semaphore.

```javascript
sem.take(function (returnAfter) {
    stream.on('data', returnAfter(processData)); // BZZZZZZZT error when it's called more than once
});
```
