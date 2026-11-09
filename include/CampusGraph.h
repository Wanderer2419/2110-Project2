#ifndef CAMPUSGRAPH_H
#define CAMPUSGRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <cctype>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

class CampusGraph {
    public:
        // Menu option: View campus Graph
        void displayCampusGraph() const;
        // Menu option: Display graph counts and degrees
        void displayGraphInfo() const;
        // Menu option: Find reachable buildings
        void displayReachability(const string& building) const;

        // Display all buildings in the graph
        void displayBuildings() const;

        // Functions to load the graph from input files
        void loadFiles(const string& dataDir);

        // Number of Vertices and Edges
        size_t numVertices;
        size_t numEdges;
    private:
        // node and pathway storage: An adjecncy list was chosen for the ease of element accessibility.
        unordered_map<string, vector<string>> adjacencyList;
        // All of the buildings stored within the graph
        vector<string> buildingsVec;
};

#endif