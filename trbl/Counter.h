#pragma once
#include "trbl/Backend.h"

#include <atomic>
#include <iostream>
#include <string_view>

namespace trbl {

using CounterId = uint64_t;

template <typename ClockT>
class CounterT
{
  public:
    using ValueT = uint64_t;

    CounterT(std::string_view name, ValueT reportEvery = 1)
        : name_(name), value_(0), id_(std::hash<std::string_view>{}(name)), reportEvery_(reportEvery)
    {
    }

    void increment()
    {
        auto oldVal = value_.fetch_add(1);
        if ((oldVal + 1) % reportEvery_ == 0)
        {
            getBackend().writeCounter(name_, {ClockT::now(), oldVal + 1});
        }
    }

  private:
    std::string_view name_;
    std::atomic<ValueT> value_;
    const CounterId id_;
    const ValueT reportEvery_;
};

using Counter = CounterT<std::chrono::high_resolution_clock>;

}  // namespace trbl
