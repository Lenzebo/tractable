#pragma once

#include "../Backend.h"

#include <mutex>
#include <queue>

namespace trbl::backends {
class AsyncBackend : public Backend
{
  public:
    AsyncBackend(std::unique_ptr<Backend>&& backend);

    void beginTracing() override;
    void endTracing() override;
    void writeTrace(const std::string_view name, const Trace& trace, const Context& context) override;

  private:
    struct Entry
    {
        std::string name;
        Trace trace;
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
