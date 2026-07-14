#include <iostream>
#include <blpapi_session.h>
#include <chrono>
#include <thread>

using namespace BloombergLP::blpapi;

void wait(int time) {
    std::this_thread::sleep_for(std::chrono::seconds(time));
}

double getPrice(std::string ticker) {
    SessionOptions options;

    options.setServerHost("localhost");
    options.setServerPort(8194);
    Session session(options);

    if (!session.start()) {
        std::cerr << "Failed to connect! Check terminal connection." << std::endl;
        wait(5);
        return -1.0;
    }

    std::cout << "Connected to bloomberg successfully" << std::endl;

    if (!session.openService("//blp/refdata")) {
        std::cerr << "Failed to open //blp/refdata service" << std::endl;
        session.stop();
        wait(5);
        return -1;
    }

    Service refDataService = session.getService("//blp/refdata");
    std::cout << "Reference Data Service has been successfully opened!" << std::endl;

    Request request = refDataService.createRequest("ReferenceDataRequest");
    request.append("securities", ticker.c_str());
    request.append("fields", "PX_LAST");
    std::cout << "Request form build cleanly for ticker: " << ticker << std::endl;
    
    wait(5);
    return 0.0;
}

int main() {
    std::cout << "Here is the most recent price of AAPL: " << getPrice(std::string("AAPL")) << std::endl;
    return 0;
}