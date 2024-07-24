#include <stdio.h>
#include <stdlib.h>
#include "libtasks.h"
#include <signal.h>

// Debugging Utility Macro
#define lambda(lambda$_ret, lambda$_args, lambda$_body) \
  ({ lambda$_ret lambda$__anon$ lambda$_args lambda$_body &lambda$__anon$; })

// Debugging Utility Function
void handle_sigint(int sig) {
  if (!isListEmpty(taskManager->tasks)) {
    foreach (void *i, taskManager->tasks) {
      Task *task = (Task *)i;
      task->cancel(task);
      free(task);
    }

    ListClear(taskManager->tasks);
  }
#if DEBUG_MODE == true
  printf("\nCaught signal %d (Ctrl+C). Cleaning up...\n", sig);
#endif  // DEBUG_MODE
}

int main() {
  signal(SIGINT, handle_sigint);  // for debugging
  InitTaskManager();

  // Task normalTask;
  // NewTask(&normalTask, TASK_TYPE_DEFAULT,
  //         lambda(void, (struct Task * reference),
  //                {
  //                  Task *ref = reference;

  //                  printf("Task Default Runnable is running...\n");
  //                  printf("Tick count: %d\n", ref->ticks);
  //                }), 0);

  // Task intervalTask = {
  //     .type = TASK_TYPE_INTERVAL,
  //     .callback = lambda(void, (struct Task * reference),
  //                        {
  //                          Task *ref = reference;
  //                          printf("Task Interval is running...\n");
  //                          printf("Tick count: %d\n", ref->ticks);
  //                        }),
  //     .timings.interval = 1};

  // RunTask(&intervalTask);

  // Task intervalTask;
  // NewTask(&intervalTask, TASK_TYPE_INTERVAL,
  //         lambda(void, (struct Task * reference),
  //                {
  //                  Task *ref = reference;

  //                  printf("Task Interval is running...\n");
  //                  printf("Tick count: %d\n", ref->ticks);

  //                  if (ref->ticks % 5 == 0) {
  //                    printf("\nAttempting to cancel Task Interval 2\n");
  //                    ref->cancel(ref);
  //                  }
  //                }),
  //         1000);

  // Task delayTask;
  // NewTask(&delayTask, TASK_TYPE_DELAY,
  //         lambda(void, (struct Task * reference),
  //                {
  //                  Task *ref = reference;
  //                  printf("Async Delayed task has ran successfully after 3 seconds...\n");
  //                }),
  //         3);

  printf("After cancel\n");
  printf(" Math test-> 4+4=%d\n", 4 + 4);
  return 0;
}