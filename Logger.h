
#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

/**
 * @brief Writes simulation summaries and events to a log.
 */
class Logger {
private:
    std::ofstream logFile;
    bool verbose;

public:
    /**
     * @brief Opens a simulation log file.
     * @param filename Output filename.
     * @param verboseMode Whether to print detailed events.
     */
    Logger(
        const std::string& filename,
        bool verboseMode
    );

    /**
     * @brief Writes a summary to terminal and log file.
     */
    void summary(const std::string& message);

    /**
     * @brief Writes an event in verbose mode only.
     */
    void event(const std::string& message);
};

#endif
