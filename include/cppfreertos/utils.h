#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>

namespace cppfreertos {

using CbFunction = void (*)();

class Runnable {
    CbFunction callback_;

   public:
    /* implicit */ Runnable(CbFunction callback);

    static void Dispatch(void* param);

    static void DispatchTimer(TimerHandle_t timer);
};

}  // namespace cppfreertos
