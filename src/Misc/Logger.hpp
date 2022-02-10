#pragma once

#pragma warning(disable : 4996)

#ifdef _WIN32

#include <Windows.h>

#endif

#include <iostream>
#include <ctime>
#include <string>
#include <glm/glm.hpp>
#include <type_traits>

using std::string;

/// \brief
/// This class is used to print to command window
/// \details
/// You can throw different sort of messages with fancy colors
class Logger final
{
  public:
    Logger() = delete;

    /// Prints informatic stuff to the CMD window
    /// Usage with variadic templates, example:
    /// C++ call: throwInfo("this is a string", 35, 0.938943f, 'c')
    /// Output: > this is a string350.938943fc
    /// You can pass multiple different-type arguments
    /// and they will be concatenated and printed together on 1 line
    template <typename T, typename... Rest>
    static inline void throwInfo(T first, Rest... rest)
    {
        m_printPrefix(MessageType::INFO, first, rest...);
    }

    /// Prints an error message to the CMD window and exits the application
    /// Usage with variadic templates, example:
    /// C++ call: throwInfo("this is a string", 35, 0.938943f, 'c')
    /// Output: > this is a string350.938943fc
    /// You can pass multiple different-type arguments
    /// and they will be concatenated and printed together on 1 line
    template <typename T, typename... Rest>
    static inline void throwError(T first, Rest... rest)
    {
        std::cout << std::endl;
        m_printPrefix(MessageType::ERR, first, rest...);
        std::cout << std::endl;
        throwInfo("Press enter to continue or press Ctrl-c to quit...");
        std::cin.get();
    }

    /// Prints debugging stuff to the CMD window
    /// Usage with variadic templates, example:
    /// C++ call: throwInfo("this is a string", 35, 0.938943f, 'c')
    /// Output: > this is a string350.938943fc
    /// You can pass multiple different-type arguments
    /// and they will be concatenated and printed together on 1 line
    template <typename T, typename... Rest>
    static inline void throwDebug(T first, Rest... rest)
    {
        m_printPrefix(MessageType::DEBUG, first, rest...);
    }

    /// Prints warning stuff to the CMD window
    /// Usage with variadic templates, example:
    /// C++ call: throwInfo("this is a string", 35, 0.938943f, 'c')
    /// Output: > this is a string350.938943fc
    /// You can pass multiple different-type arguments
    /// and they will be concatenated and printed together on 1 line
    template <typename T, typename... Rest>
    static inline void throwWarning(T first, Rest... rest)
    {
        m_printPrefix(MessageType::WARNING, first, rest...);
    }

  private:
    enum class MessageType
    {
        INFO,
        ERR,
        DEBUG,
        WARNING
    };

    static inline string m_levelString[4] = {"Info", "Error", "Debug", "Warn"};

    /// Prints the message prefix (including type, time, '>')
    template <typename T, typename... Rest>
    static inline void m_printPrefix(MessageType type, T first, Rest&&... rest)
    {
// If supported by OS
#ifdef _WIN32
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // Console access
#endif

        // Current time
        char timeBuffer[64];
        auto t   = std::time(nullptr);
        auto foo = *std::localtime(&t);
        std::strftime(timeBuffer, 64, "%H:%M:%S", &foo);

// If supported by OS
#ifdef _WIN32
        SetConsoleTextAttribute(console, 6); // White
#endif
        std::cout << "[" << timeBuffer << "]";
#ifdef _WIN32
        SetConsoleTextAttribute(console, 12); // Red
#endif
        std::cout << "[" + m_levelString[static_cast<int>(type)] + "]";
#ifdef _WIN32
        SetConsoleTextAttribute(console, 7); // Yellow
#endif

        // Proper indentation
        if (type == MessageType::DEBUG || type == MessageType::ERR) // 5 chars
        {
            std::cout << "> ";
        }
        else // 4 chars
        {
            std::cout << " > ";
        }

        m_printMessage(first, rest...);
    }

    /// Prints the actual arguments
    template <typename T, typename... Rest>
    static inline void m_printMessage(T first, Rest&&... rest)
    {
        std::cout << first;
        (std::cout << ... << rest);
        std::cout << std::endl;
    }
};
