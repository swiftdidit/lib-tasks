#ifndef __TIMER__
#define __TIMER__

#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct TaskTimer {  // Represents a timer that can be used to schedule
                        // tasks. Useful for anything.
  double startTime;     // Start Time (seconds)
  double lifeTime;      // Lifetime (seconds)
} TaskTimer;

typedef struct TaskTimerManager {
  TaskTimer **timers;  // Create a new Timer with NewTimer()
  time_t seconds;

  unsigned int totalTimers;         // total number of timers in memory
  unsigned int totalTimerCapacity;  // space allocated to Timers** array
} TaskTimerManager;

extern TaskTimerManager *taskTimerManager;

static void InitTimerManager(void) {
  taskTimerManager = malloc(sizeof(TaskTimerManager));
  taskTimerManager->totalTimerCapacity = 2;

  taskTimerManager->timers =
      malloc(taskTimerManager->totalTimerCapacity * sizeof(TaskTimer *));
  taskTimerManager->seconds = time(NULL);
  taskTimerManager->totalTimers = 0;

  if (taskTimerManager->timers == NULL) {
    fprintf(stderr,
            "[INIT MEMORY ERROR] Failed to allocate memory for Timers array\n");
    exit(EXIT_FAILURE);
  }
}


TaskTimer *NewTimer(double lifeTime);  // Create a new Timer
void StartTimer(TaskTimer *timer);     // Starts this Timer
bool TimerDone(
    TaskTimer timer);  // Check if this Timer has reached its 'lifetime' value

double GetElasped(TaskTimer timer);  // Returns the elapsed time in seconds since
                                 // 'StartTimer()' was called
time_t current_time(void);

#endif  // __TIMER__