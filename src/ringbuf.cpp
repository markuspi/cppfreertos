#include "cppfreertos/ringbuf.h"

namespace cppfreertos {

RingbufferItem::RingbufferItem(RingbufHandle_t handle, void* data, size_t item_size)
    : handle_(handle), data_(data), item_size_(item_size) {}

RingbufferItem::~RingbufferItem() {
    if (data_ != nullptr) {
        vRingbufferReturnItem(handle_, data_);
    }
}

RingbufferItem::operator bool() const {
    return data_ != nullptr;
}

size_t RingbufferItem::size() const {
    return item_size_;
}

RingbufHandle_t BaseRingBuffer::GetHandle() const {
    return handle_;
}

bool BaseRingBuffer::Send(const void* buffer, size_t item_size, TickType_t ticks_to_wait) const {
    return xRingbufferSend(handle_, buffer, item_size, ticks_to_wait) == pdTRUE;
}

RingbufferItem BaseRingBuffer::Receive(TickType_t ticks_to_wait) {
    size_t item_size = 0;
    void* data = xRingbufferReceive(handle_, &item_size, ticks_to_wait);
    return {handle_, data, item_size};
}

RingbufferItem BaseRingBuffer::ReceiveUpTo(TickType_t ticks_to_wait, size_t max_size) {
    size_t item_size = 0;
    void* data = xRingbufferReceiveUpTo(handle_, &item_size, ticks_to_wait, max_size);
    return {handle_, data, item_size};
}

}  // namespace cppfreertos