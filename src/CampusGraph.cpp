#include "CampusGraph.h"
using namespace std;

static string clean(const string& str) {
    const string whitespace = " \t\r\n";
    size_t start = str.find_first_not_of(whitespace);
    if (start == string::npos) {
        return "";
    }
    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

void CampusGraph::displayCampusGraph() const {
    cout << "Campus graph displayed in added order:" << endl;
    for (const auto& pair : adjacencyList) {
        cout << "\t" << pair.first;
        if (pair.second.size() == 0) {
            cout << " -> (No connections)" << endl;
        } else {
            for (int i = 0; i < pair.second.size(); i++) {
                cout << " -> " << pair.second[i];
            }
            cout << endl;
        }
    }
}


void CampusGraph::displayGraphInfo() const {
    cout << "Total number of Buildings: " << numVertices << endl;
    cout << "Total number of Connections: " << numEdges << endl;
    cout << "Node degree count:" << endl;
    for (const auto& pair : adjacencyList) {
        cout << "\t" << pair.first << ": " << pair.second.size() << endl;
    }
}


void CampusGraph::displayReachability(const string &input) const {
    string building = clean(input);
    building[0] = std::toupper(static_cast<unsigned char>(building[0]));
    for (int i = 1; i < building.length(); i++) {
        building[i] = std::tolower(static_cast<unsigned char>(building[i]));
    }
    // Edge case: check if the starting building exists in our graph
    if (adjacencyList.find(building) == adjacencyList.end()) {
        cout << "Starting building not found in the graph.\n";
        return;
    }

    unordered_set<string> visited; // Tracks visited buildings
    queue<string> q;               // FIFO Queue for exploration
    vector<string> traversalOrder; // Stores chronological sequence

    // Initialize the BFS queue
    q.push(building);
    visited.insert(building);

    while (!q.empty()) {
        // Get the front building from the queue
        string current = q.front();
        q.pop();
        traversalOrder.push_back(current);

        // Explore all neighboring buildings connected to the current one
        for (const auto& pair : adjacencyList) {
            string neighbor = pair.first;
            // If the neighbor hasn't been visited yet, queue it up
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }

    // Print the completed BFS traversal trajectory
    cout << "BFS Traversal Order: ";
    for (int i = 0; i < traversalOrder.size(); ++i) {
        cout << traversalOrder[i];
        if (i < traversalOrder.size() - 1) {
            cout << " -> ";
        }
    }
    cout << endl;
}


void CampusGraph::displayBuildings() const {
    cout << "All campus buildings:" << endl;
    for (const auto& building : buildingsVec) {
        cout << building << endl;
    }
}


void CampusGraph::loadFiles(const string& dataDir) {

    ifstream buildFiles((dataDir + "/buildings.txt").c_str());

    if (!buildFiles.is_open()) {

        cout << "Error opening file containing buildings." << endl;
        cout << "File pathway: " + dataDir + "/buildings.txt" << endl;
        cout << "Campus graph will be empty as buildings.txt is required to open an connections." << endl;

    } else {

        string lineB, lineC;
        int numLinesB = 0, numAcceptB = 0, numRejectB = 0;
        int numLinesC = 0, numAcceptC = 0, numRejectC = 0;

        while(getline(buildFiles, lineB)) {

            string building = clean(lineB);
            building[0] = std::toupper(static_cast<unsigned char>(building[0]));
            for (int i = 1; i < building.length(); i++) {
                building[i] = std::tolower(static_cast<unsigned char>(building[i]));
            }
            numLinesB++;
            
            if (building.empty() || building[0] == '#') {
                continue;

            }

            auto it = find(buildingsVec.begin(), buildingsVec.end(), building);

            if (it != buildingsVec.end()) {
                // Reject the record if a duplicate building exists
                cout << "Record rejected due to existing duplicate." << endl;
                cout << "File " + dataDir + "/buildings.txt @ Line #" + to_string(numLinesB) << endl;
                numRejectB++;

            } else {
                // Accept the record and add it to the storage vector, and unordered map (for connections)
                buildingsVec.push_back(building);
                adjacencyList[building] = vector<string>();
                numAcceptB++;
                numVertices++;

            }
        }

        buildFiles.close();

        ifstream connectFiles((dataDir + "/connections.txt").c_str());

        if(!connectFiles.is_open()) {
        
            cout << "Error opening file containing building connections." << endl;
            cout << "File pathway: " + dataDir + "/connections.txt" << endl;

        } else {

            while(getline(connectFiles, lineC)) {

                lineC = clean(lineC);
                numLinesC++;
                
                if (lineC.empty() || lineC[0] == '#') {

                    continue;

                }

                int numFields = static_cast<int>(count(lineC.begin(), lineC.end(), '|')) + 1;
                int barPos = lineC.find('|');
                
                string buildingA = lineC.substr(0, barPos);
                buildingA = clean(buildingA);
                buildingA[0] = std::toupper(static_cast<unsigned char>(buildingA[0]));
                for (int i = 1; i < buildingA.length(); i++) {
                    buildingA[i] = std::tolower(static_cast<unsigned char>(buildingA[i]));
                }
                
                string buildingB = lineC.substr(barPos + 1, lineC.length());
                buildingB = clean(buildingB);
                buildingB[0] = std::toupper(static_cast<unsigned char>(buildingB[0]));
                for (int i = 1; i < buildingB.length(); i++) {
                    buildingB[i] = std::tolower(static_cast<unsigned char>(buildingB[i]));
                }

                // Search for duplicate records
                bool duplicate = false;
                for (int i = 0; i < adjacencyList[buildingA].size(); i++) {
                    if (adjacencyList[buildingA][i] == buildingB) {
                        duplicate = true;
                    }
                }
                for (int i = 0; i < adjacencyList[buildingB].size(); i++) {
                    if (adjacencyList[buildingB][i] == buildingA) {
                        duplicate = true;
                    }
                }

                if (numFields != 2) {
                    // Reject the record if the number of data fields != 2
                    cout << "Record rejected due to an incorrect number of data fields." << endl;
                    cout << "File " + dataDir + "/connections.txt @ Line # " + to_string(numLinesC) << endl;
                    numRejectC++;

                } else if (buildingA.empty() || buildingB.empty()) {
                    // Reject the record if either field is empty
                    cout << "Record rejected due to an empty data field." << endl;
                    cout << "File " + dataDir + "/connections.txt @ Line # " + to_string(numLinesC) << endl;
                    numRejectC++;

                } else if (duplicate) {
                    // Reject the record if a duplicate was found
                    cout << "Record rejected due to existing duplicate." << endl;
                    cout << "File " + dataDir + "/connections.txt @ Line # " + to_string(numLinesC) << endl;
                    numRejectC++;

                } else {
                    // Add edge to both keys for building A and B
                    adjacencyList[buildingA].push_back(buildingB);
                    adjacencyList[buildingB].push_back(buildingA);
                    numAcceptC++;
                    numEdges++;

                }
            }
            connectFiles.close();
        }

        cout << "Loaded " + dataDir + "/buildings.txt: " << endl;
        cout << "\tAccepted: " << numAcceptB << endl;
        cout << "\tRejected: " << numRejectB << endl;
        cout << "Loaded " + dataDir + "/connections.txt: " << endl;
        cout << "\tAccepted: " << numAcceptC << endl;
        cout << "\tRejected: " << numRejectC << endl;
    }
}
