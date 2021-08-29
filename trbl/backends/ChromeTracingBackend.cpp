#include "ChromeTracingBackend.h"

#include <thread>
namespace trbl::backends {

namespace {
auto asMicroseconds(auto duration)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
};
}  // namespace

ChromeTracingBackend::ChromeTracingBackend(std::string filename) : filename_(std::move(filename)) {}

void ChromeTracingBackend::writeSpan(const std::string_view name, const Span& span, const Context& context)
{
    constexpr std::string_view CATEGORY = "trace";
    constexpr int PID = 1;
    const auto tid = context.threadId;

    if (!first_)
    {
        outFile_ << ",";
    }

    first_ = false;

    outFile_ << "{";
    outFile_ << R"("name": ")" << name << "\", ";
    outFile_ << R"("cat": ")" << CATEGORY << "\", ";
    outFile_ << R"("ph": "X",)";
    outFile_ << R"("ts": ")" << asMicroseconds(span.start.time_since_epoch()) << "\", ";
    auto duration = span.end - span.start;
    outFile_ << R"("dur": ")" << asMicroseconds(duration) << "\", ";
    outFile_ << R"("pid": ")" << PID << "\", ";
    outFile_ << R"("tid": ")" << tid << "\"";
    outFile_ << "}\n";
}

void ChromeTracingBackend::writeCounter(const std::string_view name, const CounterEvent event, const Context& context)
{
    if (!first_)
    {
        outFile_ << ",";
    }
    first_ = false;

    constexpr std::string_view CATEGORY = "counter";
    constexpr int PID = 1;
    const auto tid = context.threadId;

    outFile_ << "{";
    outFile_ << R"("name": ")" << name << "\", ";
    outFile_ << R"("cat": ")" << CATEGORY << "\", ";
    outFile_ << R"("ph": "C",)";
    outFile_ << R"("ts": ")" << asMicroseconds(event.timestamp.time_since_epoch()) << "\", ";
    outFile_ << R"("pid": ")" << PID << "\", ";
    outFile_ << R"("tid": ")" << tid << "\", ";
    outFile_ << R"("args": {"value":  )" << event.value << "} ";
    outFile_ << "}\n";
}

void ChromeTracingBackend::writeEvent(const std::string_view name, const TimeStamp ts, const Context& context)
{
    if (!first_)
    {
        outFile_ << ",";
    }
    first_ = false;

    constexpr std::string_view CATEGORY = "event";
    constexpr int PID = 1;
    const auto tid = context.threadId;

    outFile_ << "{";
    outFile_ << R"("name": ")" << name << "\", ";
    outFile_ << R"("cat": ")" << CATEGORY << "\", ";
    outFile_ << R"("ph": "i",)";
    outFile_ << R"("ts": ")" << asMicroseconds(ts.time_since_epoch()) << "\", ";
    outFile_ << R"("pid": ")" << PID << "\", ";
    outFile_ << R"("tid": ")" << tid << "\", ";
    outFile_ << R"("s": "g")";
    outFile_ << "}\n";
}

void ChromeTracingBackend::beginTracing()
{
    outFile_ = std::ofstream(filename_);
    outFile_ << "{";
    outFile_ << R"("traceEvents": [)";

    first_ = true;
}

void ChromeTracingBackend::endTracing()
{
    outFile_ << "]}";
}

}  // namespace trbl::backends
