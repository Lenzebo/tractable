#include "ConsoleBackend.h"

#include <iostream>

void trbl::backends::ConsoleBackend::writeSpan(const std::string_view name, const trbl::Span& span,
                                               const trbl::Context& context)
{
    {
        const auto duration = span.end - span.start;
        std::cout << "[" << name << "]\t" << span.start.time_since_epoch().count() << " -> "
                  << span.end.time_since_epoch().count() << " for "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "\n";
    }
}
