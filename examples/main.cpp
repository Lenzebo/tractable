#include "trbl/Counter.h"
#include "trbl/Session.h"
#include "trbl/Trace.h"
#include "trbl/backends/AsyncBackend.h"
#include "trbl/backends/ChromeTracingBackend.h"
#include "trbl/backends/ConsoleBackend.h"
#include "trbl/backends/LockedThreadsafeBackend.h"

#include <barrier>
#include <sstream>
#include <thread>
#include <vector>

constexpr size_t NUM_THREADS = 8;

void subfun()
{
    trbl::Trace t{"Really"};
    std::this_thread::sleep_for(std::chrono::milliseconds{30});
}

void longSubfun()
{
    trbl::Trace t{"Really long"};

    trbl::Counter counter{"LongFunc", 10};

    for (size_t i = 0; i < 250; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds{1});
        counter.increment();
    }
}

void recursFun(int i = 10)
{
    trbl::Trace t{"Rec"};
    static trbl::Counter counter{"RecFun", 10};
    counter.increment();

    std::this_thread::sleep_for(std::chrono::milliseconds{1});
    if (i > 0) recursFun(i - 1);
}

int func()
{
    trbl::Trace t{"Awesome"};

    std::this_thread::sleep_for(std::chrono::milliseconds{10});
    trbl::Trace t1{"Superbad"};
    subfun();
    std::this_thread::sleep_for(std::chrono::milliseconds{30});
    t1.stop();
    {
        subfun();
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
        trbl::Trace t2{"Baaaad"};
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }
    recursFun(10);

    longSubfun();
    trbl::getBackend().writeEvent("reached", std::chrono::high_resolution_clock::now());
    longSubfun();
    trbl::getBackend().writeEvent("done", std::chrono::high_resolution_clock::now());

    return 2;
}

int main()
{
    trbl::setBackend(std::make_unique<trbl::backends::LockedThreadsafeBackend>(
        std::make_unique<trbl::backends::ChromeTracingBackend>()));
    trbl::Session session;
    std::vector<std::thread> threads;
    for (size_t i = 0; i < NUM_THREADS; ++i)
    {
        threads.emplace_back(func);
    }
    func();
    for (auto& thread : threads)
    {
        thread.join();
    }
    return 0;
}
