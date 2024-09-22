#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

namespace cppfreertos {

class LockGuard {
    SemaphoreHandle_t handle_;
    bool locked_;

    LockGuard(SemaphoreHandle_t handle, bool locked) : handle_(handle), locked_(locked) {}
    friend class BaseSemaphore;

   public:
    ~LockGuard() {
        if (locked_) {
            xSemaphoreGive(handle_);
        }
    }

    operator bool() const {
        return locked_;
    }
};

class BaseSemaphore {
   protected:
    SemaphoreHandle_t handle_{nullptr};

    BaseSemaphore() = default;

   public:
    SemaphoreHandle_t GetHandle() const {
        return handle_;
    }

    bool Take(TickType_t ticks_to_wait) {
        return xSemaphoreTake(handle_, ticks_to_wait) == pdTRUE;
    }

    bool TakeFromISR(BaseType_t& higher_prio_task_woken) {
        return xSemaphoreTakeFromISR(handle_, &higher_prio_task_woken) == pdTRUE;
    }

    bool Give() {
        return xSemaphoreGive(handle_) == pdTRUE;
    }

    bool GiveFromISR(BaseType_t& higher_prio_task_woken) {
        return xSemaphoreGiveFromISR(handle_, &higher_prio_task_woken) == pdTRUE;
    }

    LockGuard Lock(TickType_t ticks_to_wait) {
        return {handle_, Take(ticks_to_wait)};
    }
};

class StaticSemaphore : public BaseSemaphore {
    StaticSemaphore_t semaphore_;

   public:
    bool InitBinary() {
        handle_ = xSemaphoreCreateBinaryStatic(&semaphore_);
        return handle_ != nullptr;
    }

    bool InitMutex() {
        handle_ = xSemaphoreCreateMutexStatic(&semaphore_);
        return handle_ != nullptr;
    }

    bool InitCounting(UBaseType_t max_count, UBaseType_t initial_count) {
        handle_ = xSemaphoreCreateCountingStatic(max_count, initial_count, &semaphore_);
        return handle_ != nullptr;
    }

    bool InitRecursiveMutex() {
        handle_ = xSemaphoreCreateRecursiveMutexStatic(&semaphore_);
        return handle_ != nullptr;
    }
};

class Semaphore : public BaseSemaphore {
   public:
    ~Semaphore() {
        if (handle_ != nullptr) {
            vSemaphoreDelete(handle_);
        }
    }

    bool InitBinary() {
        handle_ = xSemaphoreCreateBinary();
        return handle_ != nullptr;
    }

    bool InitMutex() {
        handle_ = xSemaphoreCreateMutex();
        return handle_ != nullptr;
    }

    bool InitCounting(UBaseType_t max_count, UBaseType_t initial_count) {
        handle_ = xSemaphoreCreateCounting(max_count, initial_count);
        return handle_ != nullptr;
    }

    bool InitRecursiveMutex() {
        handle_ = xSemaphoreCreateRecursiveMutex();
        return handle_ != nullptr;
    }
};

}  // namespace cppfreertos
