---
published: false
---
## Use cases & code demo for std::future

### Run parallel (if posible) with std::launch::async

std::launch::async will make your program run on multiple cpus (if it have)

### Run as a lazier with std::launch::deffered

Firstly, I had many confuses about calling a std::launch::deferred with a normal function.

After try to find out use cases for the function, I think this would be a best suitable one

```
#include <iostream>
#include <future>

// liba.cpp

int heavy_task(void) {
    std::cout << "a heavy computation\n";
    return 0;
}

std::future<int> code_init() {
    auto ret = std::async(std::launch::deferred, heavy_task);
    return ret;
}

// main.cpp
// #include liba.hpp

int main(int argc, char*argv[]) {
   int ret = 0;
   if(argc == 2 ) {
      ret = atoi(argv[1]);
   }
   else {
      // heave task is called when needed
      ret = code_init().get();
      ret = heavy_task();
   }
   
   return ret;
}
```
the code make lose couple knowleadge about code_init and what inside it which seem like a wrapper.
