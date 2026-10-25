
#ifndef REQUEST_H
#define REQUEST_H

#include <random>
#include <string>

/**
 * @brief Stores one web request.
 */
struct Request {
    std::string ipIn;   ///< Source IP address
    std::string ipOut;  ///< Destination IP address
    int time;           ///< Required processing cycles
    char jobType;       ///< 'P' or 'S'
};

/**
 * @brief Generates a random web request.
 * @param rng Random number generator.
 * @param minTime Minimum processing cycles.
 * @param maxTime Maximum processing cycles.
 * @return Newly generated request.
 */
Request generateRandomRequest(
    std::mt19937& rng,
    int minTime,
    int maxTime
);

#endif
