#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>

#include "utils.h"

namespace cppfreertos {

class BaseTimer {
   protected:
    TimerHandle_t handle_{nullptr};

   public:
    bool Start(TickType_t ticks_to_wait) const;

    bool StartFromISR(BaseType_t& higher_prio_task_woken) const;

    bool Stop(TickType_t ticks_to_wait) const;

    bool StopFromISR(BaseType_t& higher_prio_task_woken) const;

    bool ChangePeriod(TickType_t new_period, TickType_t ticks_to_wait) const;

    bool ChangePeriodFromISR(TickType_t new_period, BaseType_t& higher_prio_task_woken) const;

    bool Reset(TickType_t ticks_to_wait) const;

    bool ResetFromISR(BaseType_t& higher_prio_task_woken) const;
};

class StaticTimer : public BaseTimer {
    StaticTimer_t timer_{};
    Runnable runnable_;

   public:
    explicit StaticTimer(CbFunction callback);

    bool Init(const char* name, TickType_t period, bool auto_reload);
};

class Timer : public BaseTimer {
    Runnable runnable_;

   public:
    explicit Timer(CbFunction callback);

    bool Init(const char* name, TickType_t period, bool auto_reload);
};

}  // namespace cppfreertos
