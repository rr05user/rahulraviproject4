
#include "IPFirewall.h"

#include <cctype>
#include <sstream>
#include <stdexcept>

/**
 * @brief Converts an IPv4 address into a 32-bit integer.
 * @param ip Address in dotted-decimal format.
 * @return Numeric IPv4 address.
 */
std::uint32_t IPFirewall::parseIPv4(
    const std::string& ip
) {
    std::istringstream stream(ip);
    std::string part;
    std::uint32_t address = 0;

    for (int i = 0; i < 4; ++i) {
        if (!std::getline(stream, part, '.')) {
            throw std::invalid_argument("Invalid IPv4 address");
        }

        if (part.empty()) {
            throw std::invalid_argument("Invalid IPv4 address");
        }

        for (unsigned char c : part) {
            if (!std::isdigit(c)) {
                throw std::invalid_argument("Invalid IPv4 address");
            }
        }

        unsigned long octet = std::stoul(part);

        if (octet > 255) {
            throw std::invalid_argument("Invalid IPv4 octet");
        }

        address = (address << 8)
                | static_cast<std::uint32_t>(octet);
    }

    if (std::getline(stream, part, '.')) {
        throw std::invalid_argument("Too many IPv4 octets");
    }

    return address;
}

/**
 * @brief Adds an inclusive blocked IP range.
 */
void IPFirewall::addBlockedRange(
    const std::string& start,
    const std::string& end
) {
    std::uint32_t first = parseIPv4(start);
    std::uint32_t last = parseIPv4(end);

    if (first > last) {
        throw std::invalid_argument("Invalid IP range");
    }

    blockedRanges.emplace_back(first, last);
}

/**
 * @brief Determines whether an IP is within any blocked range.
 * @return True if blocked, false otherwise.
 */
bool IPFirewall::isBlocked(const std::string& ip) const {
    std::uint32_t address = parseIPv4(ip);

    for (const auto& range : blockedRanges) {
        if (address >= range.first &&
            address <= range.second) {
            return true;
        }
    }

    return false;
}
