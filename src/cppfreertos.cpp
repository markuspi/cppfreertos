#include "cppfreertos/cppfreertos.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace cppfreertos {

void BaseTask::Dispatch(void* param) {
    static_cast<BaseTask*>(param)->callback_();
    abort();
}

BaseTask::BaseTask(CbFunction callback) : callback_(callback) {}

bool BaseTask::NotifyGiveFromISR() {
    BaseType_t xHigherPriorityTaskWoken;
    vTaskNotifyGiveFromISR(handle_, &xHigherPriorityTaskWoken);
    return xHigherPriorityTaskWoken == pdTRUE;
}

void BaseTask::Resume() {
    vTaskResume(handle_);
}

void BaseTask::Suspend() {
    vTaskSuspend(handle_);
}

void BaseTask::SuspendSelf() {
    vTaskSuspend(nullptr);
}

Task::~Task() {
    if (handle_ != nullptr) {
        vTaskDelete(handle_);
    }
}

bool Task::Init(const char* name, UBaseType_t priority, uint32_t stack_size, BaseType_t core_id) {
    auto status = xTaskCreatePinnedToCore(BaseTask::Dispatch, name, stack_size, this, priority,
                                          &handle_, core_id);
    if (status == pdPASS) {
        return true;
    }
    handle_ = nullptr;
    return false;
}

}  // namespace cppfreertos
