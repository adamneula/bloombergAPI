#include <iostream>
#include <blpapi_session.h>
#include <chrono>
#include <thread>
#include <vector>
#include <map>

using namespace std;

struct DailyRecord {
    string ticker; //ticker
    string date; //date stamp (eg. 2026-07-14)
    map<string, double> metrics; //dictionary to store bloomberg's raw response
};


using namespace BloombergLP::blpapi;

void wait(int time) {
    std::this_thread::sleep_for(std::chrono::seconds(time));
}

//TODO: make this take in a vector of tickers and return a 3d array of tickers->dates->price/eps/yield/etc
std::vector<DailyRecord> bbInfoFetch(std::string ticker) {
    std::vector<DailyRecord> resultsTable;
    
    SessionOptions options;
//Connect to bloomberg on machine
    options.setServerHost("localhost");
    options.setServerPort(8194);
    Session session(options);

    if (!session.start()) {
        std::cerr << "Failed to connect! Check terminal connection." << std::endl;
        wait(5);
        return resultsTable;
    }

    std::cout << "Connected to bloomberg successfully" << std::endl;

    if (!session.openService("//blp/refdata")) {
        std::cerr << "Failed to open //blp/refdata service" << std::endl;
        session.stop();
        wait(5);
        return resultsTable;
    }

    //Retrieve reference data
    Service refDataService = session.getService("//blp/refdata");
    std::cout << "Reference Data Service has been successfully opened!" << std::endl;

    //Build request in abstract data type
    Request request = refDataService.createRequest("ReferenceDataRequest");
    request.append("securities", ticker.c_str());
    request.append("fields", "PX_LAST");
    std::cout << "Request form build cleanly for ticker: " << ticker << std::endl;

    //Send request
    session.sendRequest(request);

    double price = -1.0;
    bool done = false;

    while (!done) {
        //pauses execution to await package arrival
        Event event = session.nextEvent();

        if (event.eventType() == Event::PARTIAL_RESPONSE || event.eventType() == Event::RESPONSE) {
            MessageIterator msgIter(event);
            while (msgIter.next()) {
                Message msg = msgIter.message();
                msg.print(std::cout);
            }
        }
        if (event.eventType() == Event::RESPONSE) {
            done = true;
        }
    }

    session.stop();
    return resultsTable;
}

int main() {
    while(true) {
        std::string ticker;
        std::cout << "Enter your ticker (US): ";
        std::cin >> ticker;
        ticker += " US EQUITY";
        auto myTable = bbInfoFetch(ticker);
        std::cout << "Fetch finished! Total rows received: " << std::endl;
    }
    return 0;
}