#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <utility>

#include "utils.h"

namespace cppfreertos {

class BaseTask {
   protected:
    TaskHandle_t handle_{nullptr};

    BaseTask() = default;

   public:
    ~BaseTask() = default;
    BaseTask(const BaseTask&) = delete;
    BaseTask& operator=(const BaseTask&) = delete;
    BaseTask(BaseTask&&) = delete;
    BaseTask& operator=(BaseTask&&) = delete;

    TaskHandle_t GetHandle() const;

    void NotifyGive(UBaseType_t index_to_notify = 0) const;

    void NotifyGiveFromISR(BaseType_t& higher_prio_task_woken,
                           UBaseType_t index_to_notify = 0) const;

    static uint32_t NotifyTake(bool clear_count_on_exit, TickType_t ticks_to_wait,
                               UBaseType_t index_to_wait = 0);

    void Resume() const;

    void Suspend() const;

    static void SuspendSelf();

    static void Delay(TickType_t delay_ms);
};

template <size_t TStackSize>
class StaticTask : public BaseTask {
    StackType_t storage_[TStackSize] = {};
    StaticTask_t task_{};
    Runnable runnable_;

   public:
    explicit StaticTask(CbFunction callback) : runnable_(callback) {}

    void Init(const char* name, UBaseType_t priority = 1, BaseType_t core_id = tskNO_AFFINITY) {
        handle_ = xTaskCreateStaticPinnedToCore(Runnable::Dispatch, name, TStackSize, &runnable_,
                                                priority, storage_, &task_, core_id);
    }
};

class Task : public BaseTask {
    Runnable runnable_;

   public:
    explicit Task(CbFunction callback);
    ~Task();

    bool Init(const char* name, uint32_t stack_size, UBaseType_t priority = 1,
              BaseType_t core_id = tskNO_AFFINITY);
};

}  // namespace cppfreertos
