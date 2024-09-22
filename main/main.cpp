
#include <esp_log.h>

#include "cppfreertos/queue.h"
#include "cppfreertos/task.h"

using namespace cppfreertos;

StaticTask<4096> myTask{[]() {}};
StaticQueue<int, 5> myQueue;

extern "C" void app_main() {
    myTask.Init("mytask", tskIDLE_PRIORITY);
    myQueue.Init();

    myQueue.SendToBack(5, portMAX_DELAY);
    int x;
    myQueue.Receive(x, portMAX_DELAY);
}
