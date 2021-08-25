#include "AsyncBackend.h"

namespace trbl::backends {

AsyncBackend::AsyncBackend(std::unique_ptr<Backend>&& backend) : backend_(std::move(backend)) {}

void AsyncBackend::writeTrace(const std::string_view name, const Trace& trace, const Context& context)
{
    std::scoped_lock lock(mutex_);
    queue_.push(Entry{std::string(name), trace, context});
    cv_.notify_one();
}

void AsyncBackend::beginTracing()
{
    shutdown_ = false;
    thread_ = std::thread([this]() {
        std::queue<Entry> localQueue;

        while (!shutdown_)
        {
            {
                std::unique_lock lock(mutex_);
                cv_.wait(lock);
                std::swap(localQueue, queue_);
            }
            while (!localQueue.empty())
            {
                auto& entry = localQueue.front();
                backend_->writeTrace(entry.name, entry.trace, entry.context);
                localQueue.pop();
            }
        }
    });
}

void AsyncBackend::endTracing()
{
    shutdown_ = true;
    if (thread_.joinable())
    {
        cv_.notify_one();
        thread_.join();
    }
}

}  // namespace trbl::backends