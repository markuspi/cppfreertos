#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/ringbuf.h>

#include <cstdint>

namespace cppfreertos {

class RingbufferItem {
    RingbufHandle_t handle_;
    void* data_;
    size_t item_size_;

    RingbufferItem(RingbufHandle_t handle, void* data, size_t item_size);
    friend class BaseRingBuffer;

   public:
    ~RingbufferItem();

    RingbufferItem(const RingbufferItem&) = delete;
    RingbufferItem& operator=(const RingbufferItem&) = delete;
    RingbufferItem(RingbufferItem&&) = delete;
    RingbufferItem& operator=(RingbufferItem&&) = delete;

    operator bool() const;

    template <typename T>
    const T* data() const {
        return static_cast<const T*>(data_);
    }

    size_t size() const;
};

class BaseRingBuffer {
   protected:
    RingbufHandle_t handle_{nullptr};

    BaseRingBuffer() = default;

   public:
    ~BaseRingBuffer() = default;
    BaseRingBuffer(const BaseRingBuffer&) = delete;
    BaseRingBuffer& operator=(const BaseRingBuffer&) = delete;
    BaseRingBuffer(BaseRingBuffer&&) = delete;
    BaseRingBuffer& operator=(BaseRingBuffer&&) = delete;

    RingbufHandle_t GetHandle() const;

    bool Send(const void* buffer, size_t item_size, TickType_t ticks_to_wait);

    template <typename T>
    bool Send(const T& item, TickType_t ticks_to_wait) {
        return Send(&item, sizeof(T), ticks_to_wait);
    }

    RingbufferItem Receive(TickType_t ticks_to_wait);

    RingbufferItem ReceiveUpTo(TickType_t ticks_to_wait, size_t max_size);
};

template <size_t TBufferSize>
class StaticRingBuffer : public BaseRingBuffer {
    uint8_t storage_[TBufferSize];
    StaticRingbuffer_t ringbuffer_;

   public:
    void Init(RingbufferType_t buffer_type) {
        this->handle_ = xRingbufferCreateStatic(TBufferSize, buffer_type, storage_, &ringbuffer_);
    }
};

class RingBuffer : public BaseRingBuffer {
   public:
    ~RingBuffer() {
        if (handle_ != nullptr) {
            vRingbufferDelete(handle_);
        }
    }

    void Init(size_t buffer_size, RingbufferType_t buffer_type) {
        handle_ = xRingbufferCreate(buffer_size, buffer_type);
    }
};

}  // namespace cppfreertos
