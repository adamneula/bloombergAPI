#include <iostream>
#include <blpapi_session.h>
#include <chrono>
#include <thread>
#include <vector>
#include <map>

using namespace std; using namespace BloombergLP::blpapi;

struct DailyRecord {
    string ticker; //ticker
    string date; //date stamp (eg. 2026-07-14)
    map<string, double> metrics; //dictionary to store bloomberg's raw response
};

void wait(int time) {
    this_thread::sleep_for(chrono::seconds(time));
}

vector<DailyRecord> bbInfoFetch(const vector<string>& tickers,
                                const vector<string>& fields,
                                const string& startDate,
                                const string& endDate) {
    vector<DailyRecord> resultsTable;
    
    SessionOptions options;
//Connect to bloomberg on machine
    options.setServerHost("localhost");
    options.setServerPort(8194);
    Session session(options);

    if (!session.start()) {
        cerr << "Failed to connect! Check terminal connection." << endl;
        wait(5);
        return resultsTable;
    }

    cout << "Connected to bloomberg successfully" << endl;

    if (!session.openService("//blp/refdata")) {
        cerr << "Failed to open //blp/refdata service" << endl;
        session.stop();
        wait(5);
        return resultsTable;
    }

    //Retrieve reference data
    Service refDataService = session.getService("//blp/refdata");
    cout << "Reference Data Service has been successfully opened!" << endl;

    //Build request in abstract data type
    Request request = refDataService.createRequest("HistoricalDataRequest");
    for (const auto& t: tickers) {request.append("securities", t.c_str());}
    for (const auto& f: fields) {request.append("fields", f.c_str());}
    request.set("startDate", startDate.c_str());
    request.set("endDate", endDate.c_str());
    cout << "Request form build cleanly for tickers: " << tickers.size() << endl;

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
                msg.print(cout);
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
    // 1. Create our array of stocks:
    vector<string> myStocks = { "AAPL US Equity", "MSFT US Equity" };
    // 2. Create our array of metrics:
    vector<string> myMetrics = { "PX_LAST", "PE_RATIO" };
    // 3. Create our date horizon:
    string start = "20260101";
    string end   = "20260714";
    // 4. Call our upgraded bbInfoFetch with all 4 arguments!
    auto myTable = bbInfoFetch(myStocks, myMetrics, start, end);
    
    cout << "Fetch finished! Total rows received: " << myTable.size() << endl;
    wait(60);
    return 0;
}