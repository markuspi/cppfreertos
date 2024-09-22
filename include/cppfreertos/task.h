#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <utility>

namespace cppfreertos {

using CbFunction = void (*)();

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

    void NotifyGive(UBaseType_t index_to_notify = 0);

    void NotifyGiveFromISR(BaseType_t& higher_prio_task_woken, UBaseType_t index_to_notify = 0);

    static uint32_t NotifyTake(bool clear_count_on_exit, TickType_t ticks_to_wait,
                               UBaseType_t index_to_wait = 0);

    void Resume();

    void Suspend();

    static void SuspendSelf();

    static void Delay(TickType_t delay_ms);
};

class TaskJob {
    CbFunction callback_;

   public:
    TaskJob(CbFunction callback);

    static void Dispatch(void* param);
};

template <size_t TStackSize>
class StaticTask : public BaseTask {
    StackType_t stack_storage_[TStackSize];
    StaticTask_t task_storage_{};
    TaskJob job_;

   public:
    StaticTask(CbFunction callback) : job_(std::move(callback)) {}

    void Init(const char* name, UBaseType_t priority = 1, BaseType_t core_id = tskNO_AFFINITY) {
        handle_ = xTaskCreateStaticPinnedToCore(TaskJob::Dispatch, name, TStackSize, &job_,
                                                priority, stack_storage_, &task_storage_, core_id);
    }
};

class Task : public BaseTask {
    TaskJob job_;

   public:
    Task(CbFunction callback);
    ~Task();

    bool Init(const char* name, uint32_t stack_size, UBaseType_t priority = 1,
              BaseType_t core_id = tskNO_AFFINITY);
};

}  // namespace cppfreertos
