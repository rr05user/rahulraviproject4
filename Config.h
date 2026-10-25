
#ifndef CONFIG_H
#define CONFIG_H

#include <string>

/**
 * @brief Stores configurable simulation settings.
 */
struct Config {
    int minRequestTime = 5;
    int maxRequestTime = 100;
    double newRequestProbability = 0.10;

    int scalingCooldown = 100;
    int minQueuePerServer = 50;
    int maxQueuePerServer = 80;
    int minServers = 1;

    std::string blockedIpStart = "192.168.1.0";
    std::string blockedIpEnd = "192.168.1.255";

    /**
     * @brief Reads settings from a text file.
     * @param filename Path to configuration file.
     * @return Loaded and validated configuration.
     */
    static Config loadFromFile(
        const std::string& filename
    );
};

#endif
