#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

namespace cppfreertos {

class LockGuard {
    SemaphoreHandle_t handle_;
    bool locked_;

    LockGuard(SemaphoreHandle_t handle, bool locked);
    friend class BaseSemaphore;

   public:
    ~LockGuard();

    operator bool() const;
};

class BaseSemaphore {
   protected:
    SemaphoreHandle_t handle_{nullptr};

    BaseSemaphore() = default;

   public:
    SemaphoreHandle_t GetHandle() const;

    bool Take(TickType_t ticks_to_wait) const;

    bool TakeFromISR(BaseType_t& higher_prio_task_woken) const;

    bool Give() const;

    bool GiveFromISR(BaseType_t& higher_prio_task_woken) const;

    LockGuard Lock(TickType_t ticks_to_wait);
};

class StaticSemaphore : public BaseSemaphore {
    StaticSemaphore_t semaphore_{};

   public:
    bool InitBinary();

    bool InitMutex();

    bool InitCounting(UBaseType_t max_count, UBaseType_t initial_count);

    bool InitRecursiveMutex();
};

class Semaphore : public BaseSemaphore {
   public:
    ~Semaphore();

    bool InitBinary();

    bool InitMutex();

    bool InitCounting(UBaseType_t max_count, UBaseType_t initial_count);

    bool InitRecursiveMutex();
};

}  // namespace cppfreertos
