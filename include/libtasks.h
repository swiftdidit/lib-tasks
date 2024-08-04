#ifndef __LIBTASKS_H__
#define __LIBTASKS_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#include "liblist.h"
#include "libtimer.h"

/* 
  Automatically cleanup; Cancel tasks, free both tasks, threads, TaskTimerManager, and TaskManager.
  IF FALSE: Please refer to the #CleanupTasks() function to free all resources associated with Tasks and Threads.
*/
#define AT_EXIT_CLEANUP false

#define DEBUG_MODE false

typedef enum TaskType {
  TASK_TYPE_DEFAULT = 0,
  TASK_TYPE_INTERVAL,
  TASK_TYPE_DELAY
} TaskType;

typedef struct Task {
  pthread_t pid;
  bool isCanceled;
  int ticks;

  TaskType type;
  void *(*callback)(struct Task *reference);
  void (*cancel)(struct Task *reference);

  union data {
    void *retVal;
  } data;

  union timings {
    long interval;
    long delay;
  } timings;
} Task;

typedef struct TaskManager {
  TaskTimerManager *taskTimerManager;

  List *tasks;    // List<Task *>
  List *threads;  // List<pthread_t *>
} TaskManager;

extern TaskManager *taskManager;

static inline void CleanupTasks(void) {
  if (!isListEmpty(taskManager->tasks)) {
    foreach (void *i, taskManager->tasks) {
      Task *task = (Task *)i;

      if (task != NULL) {
        if (!task->isCanceled) {
          task->cancel(task);
        }

        free(task);
      }
    }
  }

  ListFreeMemory(taskManager->tasks);
  ListFreeMemory(taskManager->threads);

  for (int i = 0; i < taskTimerManager->totalTimers; i++) {
    free(taskTimerManager->timers[i]);
  }

  free(taskTimerManager->timers);
  free(taskTimerManager);

  free(taskManager);

#if DEBUG_MODE == true
  printf("\n End  of  Program\n Debug -> cleaned up successfully\n");
#endif  // DEBUG_MODE
}

void InitTaskManager(void);

/* Create a new Task * struct and add it to the TaskManager list of tasks, each
Task will run on it's own thread.

@param Task *task: A pointer to the Task structure that will be initialized if
not already, to add to the task list.

@param TaskType type: An enum value indicating the type of the task (e.g.,
default, interval, delay).

@param void *(*callback)(struct Task *reference): A function pointer to the
callback function that will be executed when the task is run. The callback
function takes a Task structure pointer as an argument for useful referencing
inside the callback. Optional Return value will be available once the task is
completed. Use NULL if you don't want to return anything.

@param long optionalValue: An optional value that can be used to specify
additional parameters for the task (e.g.,
interval, delay).
 */
void NewTask(Task *task, TaskType type,
             void *(*callback)(struct Task *reference), long optionalValue);

/* Run an already initialized Task * struct */
void RunTask(Task *task);

#endif  // __LIBTASKS_H__