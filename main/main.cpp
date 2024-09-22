
#include <esp_log.h>

#include "cppfreertos/queue.h"
#include "cppfreertos/ringbuf.h"
#include "cppfreertos/semphr.h"
#include "cppfreertos/task.h"

using namespace cppfreertos;

static constexpr auto TAG = "main";

StaticQueue<int, 1> queue;
RingBuffer ringbuf;
Semaphore sem;

StaticTask<4096> consumer{[]() {
    int item;
    while (true) {
        BaseTask::NotifyTake(true, pdMS_TO_TICKS(100));
        if (queue.Receive(item, 0)) {
            ESP_LOGI(TAG, "Received from queue: %d", item);
        }
        if (auto item = ringbuf.Receive(0)) {
            ESP_LOGI(TAG, "Received from buffer: %d bytes: %lu", item.size(),
                     *item.data<uint32_t>());
            ESP_LOG_BUFFER_HEX(TAG, item.data<char>(), item.size());
        }
    }
}};

Task producer{[]() {
    for (int i = 0; true; i++) {
        queue.SendToBack(i, portMAX_DELAY);
        consumer.NotifyGive();
        Task::Delay(1000);

        uint32_t x = 123;
        ringbuf.Send(x, portMAX_DELAY);
        consumer.NotifyGive();
        Task::Delay(1000);
    }
}};

extern "C" void app_main() {
    queue.Init();
    ringbuf.Init(100, RINGBUF_TYPE_BYTEBUF);
    sem.InitBinary();

    if (auto guard = sem.Lock(0)) {
    }

    consumer.Init("consumer", 2);
    producer.Init("producer", 4096);
}
