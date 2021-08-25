#include "Tracer.h"
#include "backends/AsyncBackend.h"
#include "backends/ChromeTracingBackend.h"
#include "backends/LockedThreadsafeBackend.h"

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

int testbedNotInstrumented(size_t num)
{
    int res = calculate(num);
    res += calculate(num);
    res += calculate(num);
    res += calculate(num);
    res += calculate(num);
    res += calculate(num);
    return res;
}

int testbedInstrumented(size_t num)
{
    int res = 0;
    trbl::Tracer t{"Main"};
    {
        trbl::Tracer ts{"Sub1"};
        res += calculate(num);
        {
            trbl::Tracer tss{"SubSub1"};
            res += calculate(num);
        }
    }

    {
        trbl::Tracer ts{"Sub2"};
        res += calculate(num);
        {
            trbl::Tracer tss{"SubSub2"};
            res += calculate(num);
        }
    }

    res += calculate(num);

    {
        trbl::Tracer ts{"Sub3"};
        res += calculate(num);
    }
    return res;
}

void benchmarkNotInstrumented(benchmark::State& state)
{
    auto count = state.range();
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(testbedNotInstrumented(count));
    }
}

void benchmarkInstrumented(benchmark::State& state)
{
    trbl::getBackend().beginTracing();
    auto count = state.range();
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(testbedInstrumented(count));
    }
    trbl::getBackend().endTracing();
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

BENCHMARK(benchmarkNotInstrumented)->Arg(1e2);
BENCHMARK(benchmarkDisabledTracing)->Arg(1e2);
BENCHMARK(benchmarkChromeTracing)->Arg(1e2);
BENCHMARK(benchmarkChromeTracingAsync)->Arg(1e2);
BENCHMARK(benchmarkChromeTracingThreadsafe)->Arg(1e2);

}  // namespace