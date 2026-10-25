
#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <queue>
#include <vector>

#include "Request.h"
#include "WebServer.h"

/**
 * @brief Manages web servers, queued requests, and scaling.
 */
class LoadBalancer {
private:
    std::queue<Request> requestQueue;
    std::vector<WebServer> servers;

    int currentCycle;
    int scalingCooldown;
    int lastScalingCycle;

    int minimumServers;
    int minQueuePerServer;
    int maxQueuePerServer;

    int nextServerID;
    int completedRequests;

public:
    /**
     * @brief Creates the initial server pool.
     * @param initialServers Starting server count.
     * @param cooldown Minimum cycles between scaling actions.
     * @param lower Lower queue threshold per server.
     * @param upper Upper queue threshold per server.
     * @param minServers Minimum allowed server count.
     */
    LoadBalancer(
        int initialServers,
        int cooldown,
        int lower,
        int upper,
        int minServers
    );

    /** @brief Adds a request to the queue. */
    void addRequest(const Request& request);

    /** @brief Assigns requests to available servers. */
    void assignRequests();

    /** @brief Advances processing by one clock cycle. */
    void tick();

    /** @brief Adds or removes servers when appropriate. */
    void adjustServers();

    /** @brief Returns active server count. */
    int getServerCount() const;

    /** @brief Returns waiting request count. */
    int getQueueSize() const;

    /** @brief Returns completed request count. */
    int getCompletedRequests() const;
};

#endif
