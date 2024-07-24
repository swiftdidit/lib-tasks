#include "libtimer.h"

TimerManager *timerManager;

Timer *NewTimer(double lifeTime) {
  Timer *timer = malloc(sizeof(Timer));
  if (timer == NULL) {
    fprintf(stderr,
            "[MEMORY ERROR] Failed to allocate memory for "
            "NewTimer();\n");
    exit(EXIT_FAILURE);
  }

  timer->startTime = 0.0;
  timer->lifeTime = lifeTime;

  if (timerManager->totalTimers >= timerManager->totalTimerCapacity) {
    timerManager->totalTimerCapacity *= 2;
    timerManager->timers =
        realloc(timerManager->timers,
                timerManager->totalTimerCapacity * sizeof(Timer *));
  }

  timerManager->timers[timerManager->totalTimers] = timer;
  timerManager->totalTimers += 1;
  return timer;
}

time_t current_time(void) {
    timerManager->seconds = time(NULL);
}

void StartTimer(Timer *timer) { timer->startTime = current_time(); }

bool TimerDone(Timer timer) { return GetElasped(timer) >= timer.lifeTime; }

double GetElasped(Timer timer) { return current_time() - timer.startTime; }