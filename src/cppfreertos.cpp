#include "cppfreertos.h"

#include <esp_log.h>

static const auto TAG = "cppfreertos";

namespace cppfreertos {

void work() {
    ESP_LOGI(TAG, "work()");
}

}  // namespace cppfreertos
