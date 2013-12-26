/**
 * JobQueue assumes there are limited workers available and
 * those workers can each do a job from the queue which is a
 * list of jobs to be done.
 */

/**
 * @param queue a js list of things to be done
 * @param doWork a function which takes 2 parameters, 1 the job to do and 2 the callback when done.
 * @param concurrentWorkers the number of workers which can run concurrently
 * @param onComplete called when all jobs are completed.
 */
module.exports.run = function(workQueue, doWork, concurrentWorkers, onComplete) {
  var complete = false;
  var queue = [];
  queue.push.apply(queue, workQueue);
  var workers = 0;
  var next = function() {
    if (workers < 1) { throw new Error(); }
    workers--;
    if (queue.length === 0) {
      if (workers === 0) {
        if (complete) { throw new Error(); }
        complete = true;
        onComplete();
      } else {
        console.log("Waiting for [" + workers + "] processes");
      }
      return;
    }
    while (workers < concurrentWorkers) {
      workers++;
      doWork(queue.shift(), next);
    }
  };
  workers++;
  next();
};
