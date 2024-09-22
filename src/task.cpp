#include "cppfreertos/task.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace cppfreertos {

TaskHandle_t BaseTask::GetHandle() const {
    return handle_;
}

void BaseTask::NotifyGive(UBaseType_t index_to_notify) {
    xTaskNotifyGiveIndexed(handle_, index_to_notify);
}

void BaseTask::NotifyGiveFromISR(BaseType_t& higher_prio_task_woken, UBaseType_t index_to_notify) {
    vTaskNotifyGiveIndexedFromISR(handle_, index_to_notify, &higher_prio_task_woken);
}

uint32_t BaseTask::NotifyTake(bool clear_count_on_exit, TickType_t ticks_to_wait,
                              UBaseType_t index_to_wait) {
    return ulTaskNotifyTakeIndexed(index_to_wait, clear_count_on_exit ? pdTRUE : pdFALSE,
                                   ticks_to_wait);
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

Task::Task(CbFunction callback) : job_(std::move(callback)) {}

Task::~Task() {
    if (handle_ != nullptr) {
        vTaskDelete(handle_);
    }
}

bool Task::Init(const char* name, uint32_t stack_size, UBaseType_t priority, BaseType_t core_id) {
    auto status = xTaskCreatePinnedToCore(TaskJob::Dispatch, name, stack_size, &job_, priority,
                                          &handle_, core_id);
    if (status == pdPASS) {
        return true;
    }
    handle_ = nullptr;
    return false;
}

void TaskJob::Dispatch(void* param) {
    static_cast<TaskJob*>(param)->callback_();
    abort();
}

TaskJob::TaskJob(CbFunction callback) : callback_(callback) {}

}  // namespace cppfreertos
