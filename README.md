# cppfreertos

A FreeRTOS C++ wrapper for ESP32. Compatible with PlatformIO and ESP-IDF.

## Installation

### Platformio

In your platformio.ini file, add this repository to `lib_deps`:

```bash
[env:some_env]
platform = espressif32
framework = ...
board = ...
lib_deps =
    https://github.com/markuspi/cppfreertos.git#v0.1.3
```

### ESP-IDF

In your component directory (either `main` or any other component in `components/<name>`), edit or create the `idf_component.yml` manifest file and add this repository as a dependency:

```yaml
dependencies:
  markuspi/cppfreertos:
    git: https://github.com/markuspi/cppfreertos.git
    version: v0.1.3
```

## Usage Example

```cpp
#include <cppfreertos/task.h>
#include <cppfreertos/queue.h>

// define a queue of integers with a capacity of 5 items
cppfreertos::StaticQueue<int, 5> myQueue;

// define a task with 4096 bytes of stack size in static memory
cppfreertos::StaticTask<4096> myTask([](){
    int item;
    while (true) {
        if (myQueue.Receive(item, portMAX_DELAY)) {
            std::printf("Received: %d\n", item);
        }
    }
});

// can also use void setup() { ... } for Arduino
extern "C" void app_main() {
    // initialize queue
    myQueue.Init();

    // initialize the task and start it right away
    myTask.Init("MyTask");

    // add an item to the queue
    myQueue.Send(123);
}
```
