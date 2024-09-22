#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

namespace cppfreertos {

template <typename TItem>
class BaseQueue {
   protected:
    QueueHandle_t handle_{nullptr};

    BaseQueue() = default;

   public:
    ~BaseQueue() = default;
    BaseQueue(const BaseQueue&) = delete;
    BaseQueue& operator=(const BaseQueue&) = delete;
    BaseQueue(BaseQueue&&) = delete;
    BaseQueue& operator=(BaseQueue&&) = delete;

    QueueHandle_t GetHandle() const {
        return handle_;
    }

    bool Receive(TItem& item, TickType_t ticksToWait) {
        return xQueueReceive(handle_, &item, ticksToWait) == pdTRUE;
    }

    bool ReceiveFromISR(TItem& item, BaseType_t& higher_prio_task_woken) {
        return xQueueReceiveFromISR(handle_, &item, &higher_prio_task_woken) == pdTRUE;
    }

    void Overwrite(const TItem& item) {
        xQueueOverwrite(handle_, &item);
    }

    void OverwriteFromISR(const TItem& item, BaseType_t& higher_prio_task_woken) {
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
    StaticQueue_t queue_;
    uint8_t storage_[sizeof(TItem) * TLength];

   public:
    void Init() {
        this->handle_ =
            xQueueCreateStatic(static_cast<UBaseType_t>(TLength), sizeof(TItem), storage_, &queue_);
    }
};

template <typename TItem>
class Queue : public BaseQueue<TItem> {
   public:
    ~Queue() {
        if (this->handle_ != nullptr) {
            vQueueDelete(this->handle_);
        }
    }

    bool Init(size_t length) {
        this->handle_ = xQueueCreate(static_cast<UBaseType_t>(length), sizeof(TItem));
        return this->handle_ != nullptr;
    }
};

}  // namespace cppfreertos
