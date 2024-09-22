#include "cppfreertos/task.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace cppfreertos {

void BaseTask::Dispatch(void* param) {
    static_cast<BaseTask*>(param)->callback_();
    abort();
}

BaseTask::BaseTask(CbFunction callback) : callback_(callback) {}

TaskHandle_t BaseTask::GetHandle() const {
    return handle_;
}

void BaseTask::NotifyGiveFromISR(BaseType_t& higher_prio_task_woken) {
    vTaskNotifyGiveFromISR(handle_, &higher_prio_task_woken);
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

void BaseTask::Delay(TickType_t delay_ms) {
    vTaskDelay(pdMS_TO_TICKS(delay_ms));
}

Task::~Task() {
    if (handle_ != nullptr) {
        vTaskDelete(handle_);
    }
}

bool Task::Init(const char* name, uint32_t stack_size, UBaseType_t priority, BaseType_t core_id) {
    auto status = xTaskCreatePinnedToCore(BaseTask::Dispatch, name, stack_size, this, priority,
                                          &handle_, core_id);
    if (status == pdPASS) {
        return true;
    }
    handle_ = nullptr;
    return false;
}

}  // namespace cppfreertos
