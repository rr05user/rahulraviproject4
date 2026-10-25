
#include "WebServer.h"
#include <stdexcept>

/**
 * @brief Creates an idle web server.
 * @param id Unique identifier for the server.
 */
WebServer::WebServer(int id)
    : serverID(id), busy(false), remainingTime(0),
      currentRequest{} {
}

/**
 * @brief Assigns a request to the server.
 * @param request Request to process.
 * @throws std::logic_error If the server is already busy.
 * @throws std::invalid_argument If processing time is invalid.
 */
void WebServer::assignRequest(const Request& request) {
    if (busy) {
        throw std::logic_error("Server is already busy");
    }

    if (request.time <= 0) {
        throw std::invalid_argument(
            "Request processing time must be positive"
        );
    }

    currentRequest = request;
    remainingTime = request.time;
    busy = true;
}

/**
 * @brief Advances request processing by one clock cycle.
 * @return True when a request completes.
 */
bool WebServer::tick() {
    if (!busy) {
        return false;
    }

    --remainingTime;

    if (remainingTime == 0) {
        busy = false;
        return true;
    }

    return false;
}

/**
 * @brief Checks whether the server is busy.
 * @return True if processing a request.
 */
bool WebServer::isBusy() const {
    return busy;
}

/**
 * @brief Gets the server identifier.
 * @return Unique server ID.
 */
int WebServer::getID() const {
    return serverID;
}
