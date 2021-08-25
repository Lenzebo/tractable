#include "ConsoleBackend.h"

#include <iostream>

void trbl::backends::ConsoleBackend::writeTrace(const std::string_view name, const trbl::Trace& trace,
                                                const trbl::Context& context)
{
    {
        const auto duration = trace.end - trace.start;
        std::cout << "[" << name << "]\t" << trace.start.time_since_epoch().count() << " -> "
                  << trace.end.time_since_epoch().count() << " for "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "\n";
    }
}
