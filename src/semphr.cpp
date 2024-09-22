#include "cppfreertos/semphr.h"

namespace cppfreertos {
LockGuard::LockGuard(SemaphoreHandle_t handle, bool locked) : handle_(handle), locked_(locked) {}

LockGuard::~LockGuard() {
    if (locked_) {
        xSemaphoreGive(handle_);
    }
}

LockGuard::operator bool() const {
    return locked_;
}

SemaphoreHandle_t BaseSemaphore::GetHandle() const {
    return handle_;
}

bool BaseSemaphore::Take(TickType_t ticks_to_wait) const {
    return xSemaphoreTake(handle_, ticks_to_wait) == pdTRUE;
}

bool BaseSemaphore::TakeFromISR(BaseType_t& higher_prio_task_woken) const {
    return xSemaphoreTakeFromISR(handle_, &higher_prio_task_woken) == pdTRUE;
}

bool BaseSemaphore::Give() const {
    return xSemaphoreGive(handle_) == pdTRUE;
}

bool BaseSemaphore::GiveFromISR(BaseType_t& higher_prio_task_woken) const {
    return xSemaphoreGiveFromISR(handle_, &higher_prio_task_woken) == pdTRUE;
}

LockGuard BaseSemaphore::Lock(TickType_t ticks_to_wait) {
    return {handle_, Take(ticks_to_wait)};
}

bool StaticSemaphore::InitBinary() {
    handle_ = xSemaphoreCreateBinaryStatic(&semaphore_);
    return handle_ != nullptr;
}

bool StaticSemaphore::InitMutex() {
    handle_ = xSemaphoreCreateMutexStatic(&semaphore_);
    return handle_ != nullptr;
}

bool StaticSemaphore::InitCounting(UBaseType_t max_count, UBaseType_t initial_count) {
    handle_ = xSemaphoreCreateCountingStatic(max_count, initial_count, &semaphore_);
    return handle_ != nullptr;
}

bool StaticSemaphore::InitRecursiveMutex() {
    handle_ = xSemaphoreCreateRecursiveMutexStatic(&semaphore_);
    return handle_ != nullptr;
}

Semaphore::~Semaphore() {
    if (handle_ != nullptr) {
        vSemaphoreDelete(handle_);
    }
}

bool Semaphore::InitBinary() {
    handle_ = xSemaphoreCreateBinary();
    return handle_ != nullptr;
}

bool Semaphore::InitMutex() {
    handle_ = xSemaphoreCreateMutex();
    return handle_ != nullptr;
}

bool Semaphore::InitCounting(UBaseType_t max_count, UBaseType_t initial_count) {
    handle_ = xSemaphoreCreateCounting(max_count, initial_count);
    return handle_ != nullptr;
}

bool Semaphore::InitRecursiveMutex() {
    handle_ = xSemaphoreCreateRecursiveMutex();
    return handle_ != nullptr;
}

}  // namespace cppfreertos
