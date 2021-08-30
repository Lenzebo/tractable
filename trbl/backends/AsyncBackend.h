#pragma once

#include "trbl/Backend.h"

#include <mutex>
#include <queue>

namespace trbl::backends {
class AsyncBackend : public Backend
{
  public:
    AsyncBackend(std::unique_ptr<Backend>&& backend);

    void beginTracing() override;
    void endTracing() override;
    void writeSpan(const std::string_view name, const Span& span, const Context& context) override;
    void writeCounter(const std::string_view name, const CounterEvent event, const Context& context) override;
    void writeEvent(const std::string_view name, const TimeStamp ts, const Context& context) override;

  private:
    struct Entry
    {
        std::string name;
        Span span;
        Context context;
    };

    std::unique_ptr<Backend> backend_;
    std::queue<Entry> queue_;
    std::thread thread_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<bool> shutdown_{false};
};

}  // namespace trbl::backends
