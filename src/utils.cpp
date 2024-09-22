#include "cppfreertos/utils.h"

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>

namespace cppfreertos {

Runnable::Runnable(CbFunction callback) : callback_(callback) {}

void Runnable::Dispatch(void* param) {
    static_cast<Runnable*>(param)->callback_();
}

void Runnable::DispatchTimer(TimerHandle_t timer) {
    static_cast<Runnable*>(pvTimerGetTimerID(timer))->callback_();
}

}  // namespace cppfreertos
