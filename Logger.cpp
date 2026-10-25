
#include "Logger.h"

#include <iostream>
#include <stdexcept>

/**
 * @brief Creates a logger and opens the output file.
 */
Logger::Logger(
    const std::string& filename,
    bool verboseMode
)
    : logFile(filename),
      verbose(verboseMode) {

    if (!logFile.is_open()) {
        throw std::runtime_error(
            "Could not create log file: " + filename
        );
    }
}

/**
 * @brief Prints a summary and saves it to the log.
 */
void Logger::summary(const std::string& message) {
    std::cout << message << '\n';
    logFile << message << '\n';
    logFile.flush();
}

/**
 * @brief Prints and saves important verbose events.
 */
void Logger::event(const std::string& message) {
    if (verbose) {
        std::cout << message << '\n';
        logFile << message << '\n';
    }
}
