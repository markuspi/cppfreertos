#include <cppfreertos/cppfreertos.h>
#include <esp_log.h>

using namespace cppfreertos;

StaticTask<4096> myTask{[]() {}};

extern "C" void app_main() {
    myTask.Init("mytask", tskIDLE_PRIORITY);
}
