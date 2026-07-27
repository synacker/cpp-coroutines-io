#include "precompiled.hpp"

struct MinimalTask {
    // 1. The compiler strictly looks for this exact nested struct name
    struct promise_type {

        // Factory method: constructs the outer MinimalTask object returned to the caller
        MinimalTask get_return_object() {
            std::cout << "[Promise] Creating Return Object\n";
            return MinimalTask{};
        }

        // Dictates if the coroutine halts BEFORE executing any user code
        std::suspend_never initial_suspend() noexcept {
            std::cout << "[Promise] initial_suspend triggered\n";
            return {};
        }

        // Dictates if the coroutine halts AFTER user code finishes (before destruction)
        std::suspend_never final_suspend() noexcept {
            std::cout << "[Promise] final_suspend triggered\n";
            return {};
        }

        // Triggered when the coroutine hits "co_return;" or runs out of code
        void return_void() {
            std::cout << "[Promise] return_void triggered\n";
        }

        // Mandated to catch any unhandled exceptions thrown inside the coroutine body
        void unhandled_exception() {
            std::cout << "[Promise] Exception caught!\n";
            std::terminate();
        }
    };
};

MinimalTask run_example() {
    std::cout << "  -> Inside Coroutine: Step A\n";
    co_return; // This triggers promise.return_void()
    std::cout << "  -> Inside Coroutine: Step B (This will never run)\n";
}

int main() {
    std::cout << "1. Calling run_example()...\n";
    run_example();
    std::cout << "2. Returned back to main.\n";
    return 0;
}