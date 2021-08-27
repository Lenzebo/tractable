#include "backends/AsyncBackend.h"
#include "backends/ChromeTracingBackend.h"
#include "backends/LockedThreadsafeBackend.h"
#include "trbl/Session.h"
#include "trbl/Trace.h"

#include <benchmark/benchmark.h>

#include <random>

namespace {
int calculate(size_t num)
{
    std::mt19937 rand{28837};
    int result = 0;
    for (size_t i = 0; i < num; ++i)
    {
        result += rand();
    }
    return result;
}

int calculateInstrumented(size_t num)
{
    trbl::Trace t{"Main"};
    return calculate(num);
}

void benchmarkNotInstrumented(benchmark::State& state)
{
    auto count = state.range();
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(calculate(count));
    }
}

void benchmarkMTNotInstrumented(benchmark::State& state)
{
    trbl::Session session;
    auto count = state.range(0);
    auto numThreads = 8;

    for (auto _ : state)
    {
        std::vector<std::thread> threads;
        for (size_t t = 0; t < numThreads; ++t)
        {
            threads.emplace_back([&]() {
                for (size_t i = 0; i < 1000; ++i)
                {
                    benchmark::DoNotOptimize(calculate(count));
                }
            });
        }
        for (auto& thread : threads) thread.join();
    }
}

void benchmarkInstrumented(benchmark::State& state)
{
    trbl::Session session;
    auto count = state.range();
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(calculateInstrumented(count));
    }
}

void benchmarkThreadedInstrumented(benchmark::State& state)
{
    trbl::Session session;
    auto count = state.range(0);
    auto numThreads = 8;

    for (auto _ : state)
    {
        std::vector<std::thread> threads;
        for (size_t t = 0; t < numThreads; ++t)
        {
            threads.emplace_back([&]() {
                for (size_t i = 0; i < 1000; ++i)
                {
                    benchmark::DoNotOptimize(calculateInstrumented(count));
                }
            });
        }
        for (auto& thread : threads) thread.join();
    }
}

void benchmarkChromeTracing(benchmark::State& state)
{
    trbl::setBackend(std::make_unique<trbl::backends::ChromeTracingBackend>());
    benchmarkInstrumented(state);
}

void benchmarkChromeTracingAsync(benchmark::State& state)
{
    trbl::setBackend(
        std::make_unique<trbl::backends::AsyncBackend>(std::make_unique<trbl::backends::ChromeTracingBackend>()));
    benchmarkInstrumented(state);
}

void benchmarkChromeTracingThreadsafe(benchmark::State& state)
{
    trbl::setBackend(std::make_unique<trbl::backends::LockedThreadsafeBackend>(
        std::make_unique<trbl::backends::ChromeTracingBackend>()));
    benchmarkInstrumented(state);
}

void benchmarkDisabledTracing(benchmark::State& state)
{
    trbl::setBackend(std::make_unique<trbl::NullBackend>());
    benchmarkInstrumented(state);
}

void benchmarkMTChromeTracingAsync(benchmark::State& state)
{
    trbl::setBackend(
        std::make_unique<trbl::backends::AsyncBackend>(std::make_unique<trbl::backends::ChromeTracingBackend>()));
    benchmarkThreadedInstrumented(state);
}

void benchmarkMTChromeTracingThreadsafe(benchmark::State& state)
{
    trbl::setBackend(std::make_unique<trbl::backends::LockedThreadsafeBackend>(
        std::make_unique<trbl::backends::ChromeTracingBackend>()));
    benchmarkThreadedInstrumented(state);
}

constexpr int CALCULATION_SIZE = 100;
BENCHMARK(benchmarkNotInstrumented)->Arg(CALCULATION_SIZE);
BENCHMARK(benchmarkDisabledTracing)->Arg(CALCULATION_SIZE);
BENCHMARK(benchmarkChromeTracing)->Arg(CALCULATION_SIZE);
BENCHMARK(benchmarkChromeTracingAsync)->Arg(CALCULATION_SIZE);
BENCHMARK(benchmarkChromeTracingThreadsafe)->Arg(CALCULATION_SIZE);

BENCHMARK(benchmarkMTNotInstrumented)->Arg(CALCULATION_SIZE);
BENCHMARK(benchmarkMTChromeTracingAsync)->Arg(CALCULATION_SIZE);
BENCHMARK(benchmarkMTChromeTracingThreadsafe)->Arg(CALCULATION_SIZE);

}  // namespace