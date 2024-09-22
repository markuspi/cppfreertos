#include "cppfreertos/timers.h"

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>

namespace cppfreertos {

bool BaseTimer::Start(TickType_t ticks_to_wait) const {
    return xTimerStart(handle_, ticks_to_wait) == pdPASS;
}

bool BaseTimer::StartFromISR(BaseType_t& higher_prio_task_woken) const {
    return xTimerStartFromISR(handle_, &higher_prio_task_woken) == pdPASS;
}

bool BaseTimer::Stop(TickType_t ticks_to_wait) const {
    return xTimerStop(handle_, ticks_to_wait) == pdPASS;
}

bool BaseTimer::StopFromISR(BaseType_t& higher_prio_task_woken) const {
    return xTimerStopFromISR(handle_, &higher_prio_task_woken) == pdPASS;
}

bool BaseTimer::ChangePeriod(TickType_t new_period, TickType_t ticks_to_wait) const {
    return xTimerChangePeriod(handle_, new_period, ticks_to_wait) == pdPASS;
}

bool BaseTimer::ChangePeriodFromISR(TickType_t new_period,
                                    BaseType_t& higher_prio_task_woken) const {
    return xTimerChangePeriodFromISR(handle_, new_period, &higher_prio_task_woken) == pdPASS;
}

bool BaseTimer::Reset(TickType_t ticks_to_wait) const {
    return xTimerReset(handle_, ticks_to_wait) == pdPASS;
}

bool BaseTimer::ResetFromISR(BaseType_t& higher_prio_task_woken) const {
    return xTimerResetFromISR(handle_, &higher_prio_task_woken) == pdPASS;
}

StaticTimer::StaticTimer(CbFunction callback) : runnable_(callback) {}

bool StaticTimer::Init(const char* name, TickType_t period, bool auto_reload) {
    handle_ = xTimerCreateStatic(name, period, auto_reload ? pdTRUE : pdFALSE, &runnable_,
                                 Runnable::DispatchTimer, &timer_);
    return handle_ != nullptr;
}

Timer::Timer(CbFunction callback) : runnable_(callback) {}

bool Timer::Init(const char* name, TickType_t period, bool auto_reload) {
    handle_ = xTimerCreate(name, period, auto_reload ? pdTRUE : pdFALSE, &runnable_,
                           Runnable::DispatchTimer);
    return handle_ != nullptr;
}

}  // namespace cppfreertos
