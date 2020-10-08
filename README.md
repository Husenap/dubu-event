![Windows](https://github.com/Husenap/cmake-project-template/workflows/Windows/badge.svg)
![Ubuntu](https://github.com/Husenap/cmake-project-template/workflows/Ubuntu/badge.svg)

```
   _     _                           _   
 _| |_ _| |_ _ _ ___ ___ _ _ ___ ___| |_ 
| . | | | . | | |___| -_| | | -_|   |  _|
|___|___|___|___|   |___|\_/|___|_|_|_|  
```

C++ Event Library

# Features

* Emit Events of any type
* Subscribe to Events of any type
* Manage subscription lifetime using scoped Tokens

# Examples

##### **`Event Emitter`**
```cpp
#include <iostream>

#include <dubu_event/dubu_event.h>

struct ResizeEvent {
	int width;
	int height;
};

class Window : public dubu::event::EventEmitter {
public:
	void Update() { Emit(ResizeEvent{.width = 800, .height = 600}); }
};

int main() {
	Window window;

	{
		auto token = window.Subscribe<ResizeEvent>([](const ResizeEvent& e) {
			std::cout << "Window Resized: (" << e.width << ", " << e.height
			          << ")" << std::endl;
		});

		window.Update();  // This call will trigger the callback
	}

	window.Update();  // This call won't trigger the callback because the token
	                  // has gone out of scope
}
```