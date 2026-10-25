
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

/**
 * @brief Simulates a web server processing requests.
 */
class WebServer {
private:
    int serverID;
    bool busy;
    int remainingTime;
    Request currentRequest;

public:
    /**
     * @brief Creates a web server with a unique ID.
     * @param id Server identifier.
     */
    WebServer(int id);

    /**
     * @brief Assigns a new request to an idle server.
     * @param request The request to process.
     */
    void assignRequest(const Request& request);

    /**
     * @brief Advances processing by one clock cycle.
     * @return True if a request finishes during this cycle.
     */
    bool tick();

    /**
     * @brief Checks whether the server is processing a request.
     * @return True if busy, false if idle.
     */
    bool isBusy() const;

    /**
     * @brief Returns the server identifier.
     */
    int getID() const;
};

#endif
