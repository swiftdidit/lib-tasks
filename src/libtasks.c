#include "libtasks.h"
#include <stdlib.h>

TaskManager *taskManager;

void InitTaskManager(void) {
  taskManager = (TaskManager *)malloc(sizeof(TaskManager));
  taskManager->tasks = NewList(NULL);
  taskManager->threads = NewList(NULL);

  if (atexit(cleanup) != 0) {
    fprintf(
        stderr,
        "Error: Unable to register #cleanup function in library 'libtasks'.\n");
    exit(EXIT_FAILURE);
  }
}

static void task_runner(Task *task) {
  if (task == NULL) {
    printf(
        "NullPointerException: Task * passed in task_runner function is NULL "
        "in library 'libtasks'. Something went terribly wrong, this should'nt "
        "happen!\n");
    return;
  }

  if (pthread_create(&task->pid, NULL, NULL, NULL) != 0) {
    fprintf(stderr,
            "Thread Creation Exception: Unable to create thread for struct "
            "'Task *' in library 'libtasks'.\n");
    free(task);
  } else {
    printf("Created a new thread\n");
    ListAdd(taskManager->threads, &task->pid);
  }

  switch (task->type) {
    case TASK_TYPE_INTERVAL:
      struct timespec tsInterval;
      tsInterval.tv_sec = task->timings.interval / 1000;
      tsInterval.tv_nsec = (task->timings.interval % 1000) * 1000000;

      while (!task->isCanceled) {
        task->ticks += 1;
        task->callback(task);

        nanosleep(&tsInterval, NULL);
      }
      break;
    case TASK_TYPE_DELAY:
      task->ticks += 1;

      struct timespec tsDelay;
      tsDelay.tv_sec = task->timings.interval / 1000;
      tsDelay.tv_nsec = (task->timings.interval % 1000) * 1000000;
      nanosleep(&tsDelay, NULL);

      task->callback(task);
      break;
    case TASK_TYPE_DEFAULT:
    default:
      while (!task->isCanceled) {
        task->ticks += 1;
        task->callback(task);
      }
      break;
  }

#if DEBUG_MODE == true
  printf("Task Runner exiting for task with ID: %lu\n", task->pid);
#endif  // DEBUG_MODE
}

static void cancel(Task *task) {
  task->isCanceled = true;

#if DEBUG_MODE == true
  printf("\n * Cancelling task with id= %lu\n", (unsigned long)task->pid);
#endif  // DEBUG_MODE

  pthread_cancel(task->pid);
  pthread_join(task->pid, NULL);
}

// Debugging Utility Function
static char *getTaskName(TaskType type) {
  switch (type) {
    case TASK_TYPE_INTERVAL:
      return "TASK_TYPE_INTERVAL";
    case TASK_TYPE_DELAY:
      return "TASK_TYPE_DELAY";
    case TASK_TYPE_DEFAULT:
    default:
      return "TASK_TYPE_DEFAULT";
  }
}

static Task *create_task(Task *task, TaskType type,
                         void (*callback)(struct Task *reference),
                         long optionalValue) {
  task = (Task *)malloc(sizeof(Task));

#if DEBUG_MODE == true
  printf("New Task\n");
#endif  // DEBUG_MODE

  if (task == NULL) {
    printf(
        "TASKException Error: Failed to allocate memory for struct 'Task *' in "
        "'libtasks' library!\n");
    return NULL;
  }

  task->type = type;
  task->callback = callback;
  task->cancel = cancel;
  task->isCanceled = false;
  task->ticks = 0;

  if (task->type == TASK_TYPE_INTERVAL) {
    task->timings.interval = optionalValue;
  } else if (task->type == TASK_TYPE_DELAY) {
    task->timings.delay = optionalValue;
  }

  return task;
}

void NewTask(Task *task, TaskType type,
             void (*callback)(struct Task *reference), long optionalValue) {
  task = (Task *)create_task(task, type, callback, optionalValue);
  ListAdd(taskManager->tasks, task);

  task_runner(task);
}

void RunTask(Task *task) {
  switch (task->type) {
    case TASK_TYPE_INTERVAL:
      NewTask(task, task->type, task->callback, task->timings.interval);
      break;
    case TASK_TYPE_DELAY:
      NewTask(task, task->type, task->callback, task->timings.delay);
      break;
    case TASK_TYPE_DEFAULT:
    default:
      NewTask(task, task->type, task->callback, 0);
      break;
  }
}