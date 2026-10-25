
#include <algorithm>
#include <exception>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

#include "Config.h"
#include "IPFirewall.h"
#include "LoadBalancer.h"
#include "Logger.h"
#include "Request.h"

/**
 * @brief Runs the RR load balancer simulation.
 * @return Zero on success, nonzero on failure.
 */
int main() {
    try {
        std::cout << "================================\n";
        std::cout << "       RR LOAD BALANCER\n";
        std::cout << "         Rahul Ravi\n";
        std::cout << "================================\n\n";

        // Load configurable simulation settings.
        Config config = Config::loadFromFile("config.txt");

        int initialServers;
        int totalCycles;
        std::string mode;

        std::cout << "Enter number of web servers: ";
        if (!(std::cin >> initialServers) ||
            initialServers < config.minServers ||
            initialServers > 10000) {
            std::cerr << "Invalid server count.\n";
            return 1;
        }

        std::cout << "Enter number of clock cycles: ";
        if (!(std::cin >> totalCycles) || totalCycles < 1) {
            std::cerr << "Invalid cycle count.\n";
            return 1;
        }

        std::cout << "Mode (v = verbose, q = quiet): ";
        if (!(std::cin >> mode) ||
            (mode != "v" && mode != "V" &&
             mode != "q" && mode != "Q")) {
            std::cerr << "Invalid mode.\n";
            return 1;
        }

        bool verbose = mode == "v" || mode == "V";

        // Initialize random request generation.
        std::mt19937 rng(std::random_device{}());

        std::bernoulli_distribution newArrival(
            config.newRequestProbability
        );

        // Initialize firewall.
        IPFirewall firewall;
        firewall.addBlockedRange(
            config.blockedIpStart,
            config.blockedIpEnd
        );

        // Initialize load balancer.
        LoadBalancer loadBalancer(
            initialServers,
            config.scalingCooldown,
            config.minQueuePerServer,
            config.maxQueuePerServer,
            config.minServers
        );

        // Initialize file logging.
        Logger logger("simulation.log", verbose);

        int generated = 0;
        int accepted = 0;
        int blocked = 0;
        int serversAdded = 0;
        int serversRemoved = 0;

        // Generate 100 accepted requests per initial server.
        const int initialQueueSize = initialServers * 100;

        while (loadBalancer.getQueueSize() <
               initialQueueSize) {

            Request request = generateRandomRequest(
                rng,
                config.minRequestTime,
                config.maxRequestTime
            );

            ++generated;

            if (firewall.isBlocked(request.ipIn)) {
                ++blocked;
            }
            else {
                loadBalancer.addRequest(request);
                ++accepted;
            }
        }

        // Starting statistics.
        std::ostringstream start;

        start << "\n--- Starting Summary ---\n"
              << "Initial servers: " << initialServers << '\n'
              << "Clock cycles: " << totalCycles << '\n'
              << "Initial queue size: "
              << loadBalancer.getQueueSize() << '\n'
              << "Request time range: "
              << config.minRequestTime << "-"
              << config.maxRequestTime << '\n'
              << "Scaling cooldown: "
              << config.scalingCooldown << '\n'
              << "Queue thresholds per server: "
              << config.minQueuePerServer << "-"
              << config.maxQueuePerServer << '\n'
              << "Mode: "
              << (verbose ? "Verbose" : "Quiet") << '\n';

        logger.summary(start.str());

        // Record progress 10 times during verbose runs.
        const int checkpointInterval = std::max(
            1, totalCycles / 10
        );

        // Main simulation loop.
        for (int cycle = 1; cycle <= totalCycles; ++cycle) {

            // Random incoming request.
            if (newArrival(rng)) {
                Request request = generateRandomRequest(
                    rng,
                    config.minRequestTime,
                    config.maxRequestTime
                );

                ++generated;

                if (firewall.isBlocked(request.ipIn)) {
                    ++blocked;
                }
                else {
                    loadBalancer.addRequest(request);
                    ++accepted;
                }
            }

            int previousServers =
                loadBalancer.getServerCount();

            // Advance one clock cycle.
            loadBalancer.tick();

            int currentServers =
                loadBalancer.getServerCount();

            // Log server changes.
            if (currentServers != previousServers) {
                std::ostringstream event;

                if (currentServers > previousServers) {
                    ++serversAdded;
                    event << "[Cycle " << cycle
                          << "] Added server. Total: "
                          << currentServers;
                }
                else {
                    ++serversRemoved;
                    event << "[Cycle " << cycle
                          << "] Removed server. Total: "
                          << currentServers;
                }

                logger.event(event.str());
            }

            // Periodic statistics in verbose mode.
            if (verbose &&
                cycle % checkpointInterval == 0) {

                std::ostringstream progress;

                progress << "[Cycle " << cycle << "] "
                         << "Servers: "
                         << loadBalancer.getServerCount()
                         << " | Queue: "
                         << loadBalancer.getQueueSize()
                         << " | Completed: "
                         << loadBalancer.getCompletedRequests()
                         << " | Blocked: " << blocked;

                logger.event(progress.str());
            }
        }

        // Final statistics.
        const int waiting = loadBalancer.getQueueSize();
        const int completed =
            loadBalancer.getCompletedRequests();

        const int inProgress =
            accepted - completed - waiting;

        std::ostringstream result;

        result << "\n--- Final Summary ---\n"
               << "Total cycles: " << totalCycles << '\n'
               << "Initial servers: " << initialServers << '\n'
               << "Final servers: "
               << loadBalancer.getServerCount() << '\n'
               << "Generated requests: " << generated << '\n'
               << "Accepted requests: " << accepted << '\n'
               << "Blocked requests: " << blocked << '\n'
               << "Completed requests: " << completed << '\n'
               << "Waiting requests: " << waiting << '\n'
               << "Requests in progress: "
               << inProgress << '\n'
               << "Servers added: " << serversAdded << '\n'
               << "Servers removed: " << serversRemoved << '\n'
               << "\nSimulation completed successfully.";

        logger.summary(result.str());

        return 0;
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
