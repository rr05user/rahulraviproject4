
#ifndef IPFIREWALL_H
#define IPFIREWALL_H

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

/**
 * @brief Blocks incoming requests from configured IPv4 ranges.
 */
class IPFirewall {
private:
    std::vector<std::pair<std::uint32_t, std::uint32_t>>
        blockedRanges;

    /**
     * @brief Converts an IPv4 address to a numeric value.
     * @param ip IPv4 address in dotted-decimal notation.
     * @return Numeric representation of the address.
     */
    static std::uint32_t parseIPv4(const std::string& ip);

public:
    /**
     * @brief Adds an inclusive IPv4 range to the blocklist.
     * @param start First IP address in the range.
     * @param end Last IP address in the range.
     */
    void addBlockedRange(
        const std::string& start,
        const std::string& end
    );

    /**
     * @brief Checks if a source IP is blocked.
     * @param ip Source IP address.
     * @return True if the IP is blocked.
     */
    bool isBlocked(const std::string& ip) const;
};

#endif
