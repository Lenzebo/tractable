add_library(tractable
        Backend.cpp
        Context.cpp
        Context.h
        Attribute.h
        backends/ChromeTracingBackend.h
        backends/ChromeTracingBackend.cpp
        backends/ConsoleBackend.cpp
        backends/ConsoleBackend.h
        backends/LockedThreadsafeBackend.cpp
        backends/LockedThreadsafeBackend.h
        backends/AsyncBackend.cpp
        backends/AsyncBackend.h
        )

