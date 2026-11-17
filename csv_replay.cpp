#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include "gnss_processor.h"

using namespace std;

int main(int argc, char* argv[]) {
    string path = "gnss.csv";
    if (argc > 1) path = argv[1];

    ifstream fin(path);
    if (!fin.is_open()) {
        cerr << "Error: cannot open " << path << endl;
        return 1;
    }

    // Skip header line
    string line;
    getline(fin, line);

    GnssProcessor proc;
    int total = 0, accepted = 0, rejected = 0;

    while (getline(fin, line)) {
        if (line.empty()) continue;

        istringstream ss(line);
        string tok;
        GnssFrame f;

        getline(ss, tok, ','); f.timestamp_ms    = stoull(tok);
        getline(ss, tok, ','); f.lat              = stod(tok);
        getline(ss, tok, ','); f.lon              = stod(tok);
        getline(ss, tok, ','); f.hdop             = stof(tok);
        getline(ss, tok, ','); f.satellites_used   = stoi(tok);
        getline(ss, tok, ','); f.fix_type          = stoi(tok);

        total++;
        if (proc.process(f)) {
            accepted++;
        } else {
            rejected++;
            cout << "[REJECTED] t=" << f.timestamp_ms << " ms" << endl;
        }
    }

    cout << "\n=== Replay Summary ===" << endl;
    cout << "Total:    " << total << endl;
    cout << "Accepted: " << accepted << endl;
    cout << "Rejected: " << rejected << endl;

    return 0;
}
