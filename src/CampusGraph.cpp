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

    unordered_set<string> visited;              // Tracks visited buildings
    queue<pair<string, int>> q;                 // FIFO Queue for exploration
    vector<pair<string, int>> traversalOrder;   // Stores chronological sequence along with distance from origin
    // vector<string> traversalOrder;           // Stores chronological sequence (No distance stored)
    int reachable = 1;
    int distance = 0;

    // Initialize the BFS queue
    q.push({building, 0});
    visited.insert(building);

    while (!q.empty()) {
        // Get the front building from the queue
        //string current = q.front().first;
        auto [current, distance] = q.front();
        q.pop();
        traversalOrder.push_back({current, distance});

        // Explore all neighboring buildings connected to the current one
        for (const string& neighbor : adjacencyList.at(current)) {
            // If the neighbor hasn't been visited yet, queue it up
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push({neighbor, distance+1});
                reachable++;
            }
        }
    }

    // Print the completed BFS traversal trajectory
    cout << "\nBFS Traversal Order from " << building << ":\n";
    for (int i = 0; i < traversalOrder.size(); i++) {
        cout << "\t" << traversalOrder[i].first << " (Distance = " << traversalOrder[i].second << ")\n";
    }
    cout << "Reachable: " << reachable << " out of " << numVertices << " buildings\n\n";
}


void CampusGraph::shortestPath(const string& inStart, const string& inEnd) const {
    // Source building    ===============================================
    string source = clean(inStart);
    source[0] = std::toupper(static_cast<unsigned char>(source[0]));
    for (int i = 1; i < source.length(); i++) {
        source[i] = std::tolower(static_cast<unsigned char>(source[i]));
    }
    // Edge case: check if the starting building exists in our graph
    if (adjacencyList.find(source) == adjacencyList.end()) {
        cout << "Source building not found in the graph.\n";
        return;
    }

    // Destination building    ===============================================
    string destination = clean(inEnd);
    destination[0] = std::toupper(static_cast<unsigned char>(destination[0]));
    for (int i = 1; i < destination.length(); i++) {
        destination[i] = std::tolower(static_cast<unsigned char>(destination[i]));
    }
    // Edge case: check if the ending building exists in our graph
    if (adjacencyList.find(destination) == adjacencyList.end()) {
        cout << "Destination building not found in the graph.\n";
        return;
    }

    // Edge case for when the Source is the same as the Destination
    if (source == destination) {
        cout << "BFS shortest path:\n";
        cout << "Distance = 0\n";
        cout << "\t" << source << endl;
        return;
    }

    // BFS path finding    ===============================================
    unordered_set<string> visited;              // Tracks visited buildings
    unordered_map<string, string> parent;       // Mapped parent nodes
    queue<string> q;                            // FIFO Queue for exploration
    vector<string> path;                        // Vector to store the path from start to end

    // Initialize the BFS queue
    q.push({source, 0});
    visited.insert(source);

    bool found = false;

    while (!q.empty()) {
        // Get the front building from the queue
        string current = q.front();
        q.pop();
        
        // Check whether current is the destination
        if (current == destination) {
            found = true;
            break;
        }

        // Explore all neighboring buildings connected to the current one
        for (const string& neighbor : adjacencyList.at(current)) {
            // If the neighbor hasn't been visited yet, queue it up
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                parent[neighbor] = current;
                q.push(neighbor);
            }
        }
    }

    // If the destination wasn't found, output a message saying so
    if (!found) {
        cout << "No path between " << source << " and " << destination << " found.\n";
        return;
    }

    // Create a path back from the destination to the original source
    string current = destination;
    while (current != source) {
        path.push_back(current);
        current = parent.at(current);
    }
    path.push_back(source);

    reverse(path.begin(), path.end());

    // Output the BFS path in a nice message
    cout << "BFS Shortest path:\n";
    cout << "Path distance = " << path.size() - 1 << endl;
    for (size_t i = 0; i < path.size(); i++) {
        cout << "\t" << path[i] << endl;
    }
}


void CampusGraph::connectivityReport() const {

    cout << "=== Graph Connectivity report ===\n";

    // Check to make sure there are buidlings       ==============================
    if (adjacencyList.empty()) {
        cout << "\tThe graph is empty\n";
        cout << "\tTotal number of Buildings = 0\n";
        cout << "\tTotal number of Connections = 0\n";
        cout << "\tTotal number of connected Componenet = 0\n";
        return;
    }

    // Total # of buildings     ==============================
    cout << "\tTotal number of Buildings = " << numVertices << endl;

    // Total # of connections               ==============================
    cout << "\tTotal number of Connections = " << numEdges << endl;

    // Highest degree buildings     ==============================
    size_t maxNum = 0;
    for (const auto& pair : adjacencyList) {
        if (pair.second.size() > maxNum) {
            maxNum = pair.second.size();
        }
    }
    cout << "\tBuildings with a maximum number of connections = " << maxNum << ": " << endl;
    for (const auto& pair : adjacencyList) {
        if (pair.second.size() == maxNum) {
            cout << "\t\t" << pair.first << endl;
        }
    }

    // Isolated buildings (if any)      ==============================
    bool isolatedFound = false;
    for (const auto& pair : adjacencyList) {
        if (pair.second.size() == 0) {
            isolatedFound = true;
        }
    }
    if (isolatedFound) {
        cout << "\tIsolated buildings:\n";
        for (const auto& pair : adjacencyList) {
            if (pair.second.size() == 0) {
                cout << "\t\t" << pair.first << endl;
            }
        }
    }

    // Total # of connected components      ==============================
    // Process: Run BFS on every component in the graph until each one has been marked as visisted.
    unordered_set<string> visited;
    queue<string> q;

    int components = 0;

    for (const auto& pair : adjacencyList) {
        // If a building has been visisted, skip it
        if (visited.find(pair.first) != visited.end()) {
            continue;
        }

        // If the building hasn't been visisted, that means it's part of a new set individually connected vertices (set/componenets)
        components++;

        q.push(pair.first);
        visited.insert(pair.first);

        while (!q.empty()) {
            // Get the front building from the queue
            //string current = q.front().first;
            string current = q.front();
            q.pop();

            if (adjacencyList.at(current).size() != 0) {
                for (const string& neighbor : adjacencyList.at(current)) {
                    if (visited.find(neighbor) == visited.end()) {
                        visited.insert(neighbor);
                        q.push(neighbor);
                    }
                }
            }
        }
    }
    cout << "\tTotal number of connected Componenets = " << components << endl;
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
