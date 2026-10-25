
#include "LoadBalancer.h"

#include <algorithm>
#include <stdexcept>

/**
 * @brief Initializes a configurable load balancer.
 */
LoadBalancer::LoadBalancer(
    int initialServers,
    int cooldown,
    int lower,
    int upper,
    int minServers
)
    : currentCycle(0),
      scalingCooldown(cooldown),
      lastScalingCycle(-cooldown),
      minimumServers(minServers),
      minQueuePerServer(lower),
      maxQueuePerServer(upper),
      nextServerID(1),
      completedRequests(0) {

    if (initialServers < minServers ||
        minServers < 1 ||
        cooldown < 0 ||
        lower < 0 ||
        upper <= lower) {
        throw std::invalid_argument(
            "Invalid load balancer settings"
        );
    }

    for (int i = 0; i < initialServers; ++i) {
        servers.emplace_back(nextServerID++);
    }
}

/**
 * @brief Enqueues an incoming request.
 */
void LoadBalancer::addRequest(const Request& request) {
    requestQueue.push(request);
}

/**
 * @brief Assigns requests to idle servers.
 */
void LoadBalancer::assignRequests() {
    for (WebServer& server : servers) {
        if (requestQueue.empty()) {
            break;
        }

        if (!server.isBusy()) {
            server.assignRequest(requestQueue.front());
            requestQueue.pop();
        }
    }
}

/**
 * @brief Advances one simulation cycle.
 */
void LoadBalancer::tick() {
    ++currentCycle;

    assignRequests();

    for (WebServer& server : servers) {
        if (server.tick()) {
            ++completedRequests;
        }
    }

    adjustServers();
}

/**
 * @brief Dynamically adjusts the server pool.
 *
 * Adds a server above the upper queue threshold.
 * Removes an idle server below the lower threshold.
 * Respects the configured scaling cooldown.
 */
void LoadBalancer::adjustServers() {
    if (currentCycle - lastScalingCycle <
        scalingCooldown) {
        return;
    }

    const int count = getServerCount();
    const int waiting = getQueueSize();

    if (waiting > maxQueuePerServer * count) {
        servers.emplace_back(nextServerID++);
        lastScalingCycle = currentCycle;
    }
    else if (waiting < minQueuePerServer * count &&
             count > minimumServers) {

        auto idleServer = std::find_if(
            servers.begin(),
            servers.end(),
            [](const WebServer& server) {
                return !server.isBusy();
            }
        );

        if (idleServer != servers.end()) {
            servers.erase(idleServer);
            lastScalingCycle = currentCycle;
        }
    }
}

/**
 * @brief Returns current server count.
 */
int LoadBalancer::getServerCount() const {
    return static_cast<int>(servers.size());
}

/**
 * @brief Returns queued request count.
 */
int LoadBalancer::getQueueSize() const {
    return static_cast<int>(requestQueue.size());
}

/**
 * @brief Returns number of completed requests.
 */
int LoadBalancer::getCompletedRequests() const {
    return completedRequests;
}
