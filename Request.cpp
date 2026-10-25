
#include "Request.h"

#include <stdexcept>

/**
 * @brief Generates a random web request.
 *
 * Creates random IP addresses, processing times,
 * and job types. Approximately 5% of requests
 * use IP addresses from a blocked range.
 *
 * @param rng Random number generator.
 * @param minTime Minimum processing time.
 * @param maxTime Maximum processing time.
 * @return A randomly generated Request.
 */
Request generateRandomRequest(
    std::mt19937& rng,
    int minTime,
    int maxTime
) {
    if (minTime < 1 || maxTime < minTime) {
        throw std::invalid_argument(
            "Invalid request processing time range"
        );
    }

    std::uniform_int_distribution<int> octet(0, 255);
    std::uniform_int_distribution<int> duration(
        minTime, maxTime
    );
    std::uniform_int_distribution<int> jobType(0, 1);
    std::uniform_int_distribution<int> blockedChance(1, 20);

    Request request;

    // Approximately 5% are from the blocked range.
    if (blockedChance(rng) == 1) {
        request.ipIn = "192.168.1." +
                       std::to_string(octet(rng));
    }
    else {
        request.ipIn = "10." +
                       std::to_string(octet(rng)) + "." +
                       std::to_string(octet(rng)) + "." +
                       std::to_string(octet(rng));
    }

    // Generate destination IP.
    request.ipOut = "172.16." +
                    std::to_string(octet(rng)) + "." +
                    std::to_string(octet(rng));

    // Generate processing time.
    request.time = duration(rng);

    // Randomly choose Processing or Streaming.
    request.jobType = jobType(rng) == 0 ? 'P' : 'S';

    return request;
}
