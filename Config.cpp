
#include "Config.h"

#include <fstream>
#include <stdexcept>
#include <string>

/**
 * @brief Reads and validates load balancer settings.
 */
Config Config::loadFromFile(
    const std::string& filename
) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error(
            "Cannot open configuration file: " + filename
        );
    }

    Config config;
    std::string line;

    while (std::getline(file, line)) {
        // Remove comments.
        size_t comment = line.find('#');
        if (comment != std::string::npos) {
            line = line.substr(0, comment);
        }

        // Remove whitespace.
        auto trim = [](const std::string& text) {
            size_t first = text.find_first_not_of(" \t\r\n");

            if (first == std::string::npos) {
                return std::string();
            }

            size_t last = text.find_last_not_of(" \t\r\n");
            return text.substr(first, last - first + 1);
        };

        line = trim(line);
        if (line.empty()) {
            continue;
        }

        size_t equals = line.find('=');
        if (equals == std::string::npos) {
            throw std::runtime_error(
                "Invalid config line: " + line
            );
        }

        std::string key = trim(line.substr(0, equals));
        std::string value = trim(line.substr(equals + 1));

        if (key == "MIN_REQUEST_TIME") {
            config.minRequestTime = std::stoi(value);
        }
        else if (key == "MAX_REQUEST_TIME") {
            config.maxRequestTime = std::stoi(value);
        }
        else if (key == "NEW_REQUEST_PROBABILITY") {
            config.newRequestProbability = std::stod(value);
        }
        else if (key == "SCALING_COOLDOWN") {
            config.scalingCooldown = std::stoi(value);
        }
        else if (key == "MIN_QUEUE_PER_SERVER") {
            config.minQueuePerServer = std::stoi(value);
        }
        else if (key == "MAX_QUEUE_PER_SERVER") {
            config.maxQueuePerServer = std::stoi(value);
        }
        else if (key == "MIN_SERVERS") {
            config.minServers = std::stoi(value);
        }
        else if (key == "BLOCKED_IP_START") {
            config.blockedIpStart = value;
        }
        else if (key == "BLOCKED_IP_END") {
            config.blockedIpEnd = value;
        }
        else {
            throw std::runtime_error(
                "Unknown config setting: " + key
            );
        }
    }

    if (config.minRequestTime < 1 ||
        config.maxRequestTime < config.minRequestTime ||
        config.newRequestProbability < 0.0 ||
        config.newRequestProbability > 1.0 ||
        config.scalingCooldown < 0 ||
        config.minQueuePerServer < 0 ||
        config.maxQueuePerServer <= config.minQueuePerServer ||
        config.minServers < 1) {

        throw std::runtime_error(
            "Invalid values in configuration file"
        );
    }

    return config;
}
