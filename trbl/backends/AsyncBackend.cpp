#include "AsyncBackend.h"

namespace trbl::backends {

AsyncBackend::AsyncBackend(std::unique_ptr<Backend>&& backend) : backend_(std::move(backend)) {}

void AsyncBackend::writeSpan(const std::string_view name, const Span& span, const Context& context)
{
    std::scoped_lock lock(mutex_);
    queue_.push(Entry{std::string(name), span, context});
    cv_.notify_one();
}

void AsyncBackend::beginTracing()
{
    backend_->beginTracing();

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
                backend_->writeSpan(entry.name, entry.span, entry.context);
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
        mutex_.lock();
        cv_.notify_one();
        mutex_.unlock();
        thread_.join();
    }
    while (!queue_.empty())
    {
        auto& entry = queue_.front();
        backend_->writeSpan(entry.name, entry.span, entry.context);
        queue_.pop();
    }
    assert(queue_.empty());
    backend_->endTracing();
}
void AsyncBackend::writeCounter(const std::string_view name, const CounterEvent event, const Context& context)
{
    assert(false);
}
void AsyncBackend::writeEvent(const std::string_view name, const TimeStamp ts, const Context& context)
{
    assert(false);
}

}  // namespace trbl::backends