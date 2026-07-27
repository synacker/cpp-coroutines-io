#include "precompiled.hpp"

// 1. Basic Awaitable Coroutine Task Skeleton
struct CoroTask {
    struct promise_type {
        CoroTask get_return_object() { return {}; }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
};

// 2. A coroutine consuming an stdexec sender via co_await
CoroTask run_async_pipeline(auto scheduler) {
    std::cout << "[Coro] Suspended. Awaiting stdexec execution context..." << std::endl;
    
    // co_await works natively on stdexec senders!
    co_await stdexec::schedule(scheduler);
    
    std::cout << "[Coro] Resumed cleanly on the target thread pool scheduler context!" << std::endl;
}

int main() {
    std::cout << "[System] Booting stdexec + io_uring environment..." << std::endl;

    // Verify liburing bindings
    struct io_uring ring;
    if (io_uring_queue_init(8, &ring, 0) == 0) {
        std::cout << "[io_uring] Subsystem allocated." << std::endl;
        io_uring_queue_exit(&ring);
    }

    // Verify stdexec composition
    using namespace stdexec;
    exec::static_thread_pool pool{4}; // Create a 4-thread execution resource
    auto my_scheduler = pool.get_scheduler();

    // Construct a sender computation chain using functional pipelining
    auto work_chain = schedule(my_scheduler) 
                    | then([]() { 
                          std::cout << "[stdexec] Pipelined task executed on worker thread." << std::endl;
                          return 42; 
                      })
                    | then([](int val) {
                          std::cout << "[stdexec] Received data down the pipeline: " << val << std::endl;
                      });

    // Run the functional work chain synchronously to block main until complete
    sync_wait(work_chain);

    // Trigger the Coroutine-to-Sender integration pathway
    run_async_pipeline(my_scheduler);

    std::cout << "[System] Core tasks completed." << std::endl;
    return 0;
}
