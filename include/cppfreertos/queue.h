#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

namespace cppfreertos {

template <typename TItem>
class BaseQueue {
   protected:
    QueueHandle_t handle_{nullptr};

   public:
    bool Receive(TItem& item, TickType_t ticksToWait) {
        return xQueueReceive(handle_, &item, ticksToWait) == pdTRUE;
    }

    bool ReceiveFromISR(TItem& item, BaseType_t& higher_prio_task_woken) {
        return xQueueReceiveFromISR(handle_, &item, &higher_prio_task_woken) == pdTRUE;
    }

    void Overwrite(const TItem& item) {
        xQueueOverwrite(handle_, &item);
    }

    void Overwrite(const TItem& item, BaseType_t& higher_prio_task_woken) {
        xQueueOverwriteFromISR(handle_, &item, &higher_prio_task_woken);
    }

    bool SendToBack(const TItem& item, TickType_t ticksToWait) {
        return xQueueSendToBack(handle_, &item, ticksToWait) == pdTRUE;
    }

    bool SendToBackFromISR(const TItem& item, BaseType_t& higher_prio_task_woken) {
        return xQueueSendToBackFromISR(handle_, &item, &higher_prio_task_woken) == pdTRUE;
    }

    bool SendToFront(const TItem& item, TickType_t ticksToWait) {
        return xQueueSendToFront(handle_, &item, ticksToWait) == pdTRUE;
    }

    bool SendToFrontFromISR(const TItem& item, BaseType_t& higher_prio_task_woken) {
        return xQueueSendToFrontFromISR(handle_, &item, &higher_prio_task_woken) == pdTRUE;
    }
};

template <typename TItem, size_t TLength>
class StaticQueue : public BaseQueue<TItem> {
    StaticQueue_t queue_storage_;
    uint8_t queue_buffer_[sizeof(TItem) * TLength];

   public:
    void Init() {
        this->handle_ = xQueueCreateStatic(TLength, sizeof(TItem), queue_buffer_, &queue_storage_);
    }
};

}  // namespace cppfreertos
