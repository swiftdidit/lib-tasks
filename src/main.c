#include <stdio.h>
#include <stdlib.h>
#include "libtasks.h"
#include <signal.h>

// Debugging Utility Macro
#define lambda(lambda$_ret, lambda$_args, lambda$_body) \
  ({ lambda$_ret lambda$__anon$ lambda$_args lambda$_body &lambda$__anon$; })

// Debugging Utility Function
void handle_sigint(int sig) {
#if DEBUG_MODE == true
  printf("\nCaught signal %d (Ctrl+C). Cleaning up...\n", sig);
#endif  // DEBUG_MODE

  CleanupTasks();
}

int main() {
  signal(SIGINT, handle_sigint);  // for debugging
  InitTaskManager();

  // Task *normalTask = malloc(sizeof(Task));
  // NewTask(normalTask, TASK_TYPE_DEFAULT,
  //         lambda(void *, (struct Task * reference),
  //                {
  //                  Task *ref = reference;

  //                  printf("Task Default Runnable is running...\n");
  //                  printf("Tick count: %d\n", ref->ticks);

  //                  return NULL;
  //                }),
  //         0);

  // if (normalTask->data.retVal == NULL) {
  //   printf("\nNormal Task Return Value -> NULL\n");
  // } else {
  //   printf("\nNormal Task Return Value (VALID) -> %p\n",
  //   normalTask->data.retVal);
  // }

  Task *intervalTask = malloc(sizeof(Task));

  intervalTask->type = TASK_TYPE_INTERVAL;
  intervalTask->callback = lambda(void *, (struct Task * reference), {
    Task *ref = reference;
    printf("Task Interval is running...\n");
    printf("Tick count: %d\n", ref->ticks);

    int *test = malloc(sizeof(int));
    *test = 23 * 3;
    printf("\nMath test-> %d\n", *test);

    // if (ref->ticks % 5 == 0) {
    //   printf("\nAttempting to cancel Task Interval\n");
    //   ref->cancel(ref);
    // }
    return test;
  });
  intervalTask->timings.interval = 1;

  RunTask(intervalTask);

  printf("\nInterval Task Return Value -> %d\n",
         *(Task *)intervalTask->data.retVal);

  // Task *intervalTask = malloc(sizeof(Task));
  // NewTask(intervalTask, TASK_TYPE_INTERVAL,
  //         lambda(void *, (struct Task * reference),
  //                {
  //                  Task *ref = reference;

  //                  printf("Task Interval is running...\n");
  //                  printf("Tick count: %d\n", ref->ticks);

  //                  if (ref->ticks % 5 == 0) {
  //                    printf("\nAttempting to cancel Task Interval 2\n");
  //                    ref->cancel(ref);
  //                  }

  //                  return NULL;
  //                }),
  //         3);

  // Task *delayTask = malloc(sizeof(Task));
  // NewTask(delayTask, TASK_TYPE_DELAY,
  //         lambda(void *, (struct Task * reference),
  //                {
  //                  Task *ref = reference;
  //                  printf("Async Delayed task has started...\n");

  //                  return NULL;
  //                }),
  //         3);

  printf("After cancel\n");
  printf(" Math test-> 4+4=%d\n", 4 + 4);
  return 0;
}