#include "ChromeTracingBackend.h"

#include <thread>
namespace trbl::backends {
void ChromeTracingBackend::writeTrace(const std::string_view name, const Trace& trace, const Context& context)
{
    constexpr std::string_view CATEGORY = "blubl";
    constexpr int PID = 1;
    const auto tid = context.threadId;

    auto asMicroseconds = [](auto duration) {
        return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    };

    if (!first_)
    {
        outFile_ << ",";
    }

    first_ = false;

    outFile_ << "{";
    outFile_ << R"("name": ")" << name << "\", ";
    outFile_ << R"("cat": ")" << CATEGORY << "\", ";
    outFile_ << R"("ph": "X",)";
    outFile_ << R"("ts": ")" << asMicroseconds(trace.start.time_since_epoch()) << "\", ";
    auto duration = trace.end - trace.start;
    outFile_ << R"("dur": ")" << asMicroseconds(duration) << "\", ";
    outFile_ << R"("pid": ")" << PID << "\", ";
    outFile_ << R"("tid": ")" << tid << "\"";
    outFile_ << "}\n";
}

void ChromeTracingBackend::beginTracing()
{
    outFile_ = std::ofstream("out.json");
    outFile_ << "{";
    outFile_ << R"("traceEvents": [)";

    first_ = true;
}

void ChromeTracingBackend::endTracing()
{
    outFile_ << "]}";
}

}  // namespace trbl::backends
