#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace cppfreertos {

using CbFunction = void (*)();

class BaseTask {
   protected:
    TaskHandle_t handle_{nullptr};
    CbFunction callback_{nullptr};

    static void Dispatch(void* param);

   public:
    BaseTask(CbFunction callback);
    ~BaseTask() = default;

    BaseTask(const BaseTask&) = delete;
    BaseTask& operator=(const BaseTask&) = delete;
    BaseTask(BaseTask&&) = delete;
    BaseTask& operator=(BaseTask&&) = delete;

    bool NotifyGiveFromISR();

    void Resume();

    void Suspend();

    static void SuspendSelf();
};

template <size_t TStackSize>
class StaticTask : public BaseTask {
    StackType_t stack_storage_[TStackSize];
    StaticTask_t task_storage_{};

   public:
    using BaseTask::BaseTask;

    void Init(const char* name, UBaseType_t priority, BaseType_t core_id = tskNO_AFFINITY) {
        handle_ = xTaskCreateStaticPinnedToCore(BaseTask::Dispatch, name, TStackSize, this,
                                                priority, stack_storage_, &task_storage_, core_id);
    }
};

class Task : public BaseTask {
   public:
    using BaseTask::BaseTask;

    ~Task();

    bool Init(const char* name, UBaseType_t priority, uint32_t stack_size,
              BaseType_t core_id = tskNO_AFFINITY);
};

}  // namespace cppfreertos
