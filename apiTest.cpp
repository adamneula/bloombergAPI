#include <iostream>
#include <blpapi_session.h>

using namespace BloombergLP::blpapi;

SessionOptions options;

options.setServerHost("localhost")
options.setServerPort(8194)
Session session(options)

if (!session.start()) {
    std::cerr << "Failed to connect! Check terminal connection." << std::endl;
    return -1.0;
}

std::cout << "Connected to bloomberg successfully" << std::endl;

double getPrice(std::string ticker) {
    
}

int main() {
    std::cout << "Here is the most recent price of AAPL: " << getPrice(std::string("AAPL")) << std::endl;
}