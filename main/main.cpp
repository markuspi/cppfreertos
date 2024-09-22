
#include <esp_log.h>

#include "cppfreertos/queue.h"
#include "cppfreertos/task.h"

using namespace cppfreertos;

static constexpr auto TAG = "main";

StaticQueue<int, 1> queue;
StaticTask<4096> consumer{[]() {
    int item;
    while (true) {
        if (queue.Receive(item, portMAX_DELAY)) {
            ESP_LOGI(TAG, "Received: %d", item);
        }
    }
}};

Task producer{[]() {
    for (int i = 0; true; i++) {
        queue.SendToBack(i, portMAX_DELAY);
        Task::Delay(1000);
    }
}};

extern "C" void app_main() {
    queue.Init();
    consumer.Init("consumer");
    producer.Init("producer", 4096);
}
