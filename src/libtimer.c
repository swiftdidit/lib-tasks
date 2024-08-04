#include "libtimer.h"

TaskTimerManager *taskTimerManager;

TaskTimer *NewTimer(double lifeTime) {
  TaskTimer *timer = malloc(sizeof(TaskTimer));
  if (timer == NULL) {
    fprintf(stderr,
            "[MEMORY ERROR] Failed to allocate memory for "
            "NewTimer();\n");
    exit(EXIT_FAILURE);
  }

  timer->startTime = 0.0;
  timer->lifeTime = lifeTime;

  if (taskTimerManager->totalTimers >= taskTimerManager->totalTimerCapacity) {
    taskTimerManager->totalTimerCapacity *= 2;
    taskTimerManager->timers =
        realloc(taskTimerManager->timers,
                taskTimerManager->totalTimerCapacity * sizeof(TaskTimer *));
  }

  taskTimerManager->timers[taskTimerManager->totalTimers] = timer;
  taskTimerManager->totalTimers += 1;
  return timer;
}

time_t current_time(void) {
    taskTimerManager->seconds = time(NULL);
}

void StartTimer(TaskTimer *timer) { timer->startTime = current_time(); }

bool TimerDone(TaskTimer timer) { return GetElasped(timer) >= timer.lifeTime; }

double GetElasped(TaskTimer timer) { return current_time() - timer.startTime; }