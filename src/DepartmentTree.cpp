#include "DepartmentTree.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>

static string trim(const string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    return b == string::npos ? "" : s.substr(b, s.find_last_not_of(" \t\r\n") - b + 1);
}

static bool equalsIgnoreCase(const string& a, const string& b) {
    return a.size() == b.size() && equal(a.begin(), a.end(), b.begin(),
        [](char x, char y) { return tolower((unsigned char)x) == tolower((unsigned char)y); });
}

static bool askName(const string& prompt, string& name) {
    cout << prompt;
    getline(cin, name);
    name = trim(name);
    if (name.empty()) cout << "Error: name cannot be blank.\n";
    return !name.empty();
}

DepartmentNode* DepartmentTree::findIn(DepartmentNode* n, const string& name) {
    if (n == NULL || equalsIgnoreCase(n->name, name)) return n;
    for (size_t i = 0; i < n->children.size(); i++)
        if (DepartmentNode* found = findIn(n->children[i], name)) return found;
    return NULL;
}


bool DepartmentTree::add(const string& name, const string& parentName, string& reason) {
    reason.clear();
    DepartmentNode* parent = find(parentName);
    if (find(name) != NULL) reason = "duplicate department name \"" + name + "\"";
    else if (root == NULL) root = new DepartmentNode(name, NULL);
    else if (parent == NULL) reason = "unknown parent \"" + parentName + "\"";
    else parent->children.push_back(new DepartmentNode(name, parent));
    return reason.empty();
}

void DepartmentTree::loadFromFile(const string& dataDir) {
    ifstream in((dataDir + "/departments.txt").c_str());
    if (!in) cout << "Error: cannot open file departments.txt; treating it as empty.\n";

    int accepted = 0, rejected = 0, lineNo = 0;
    string line;
    while (getline(in, line)) {
        line = trim(line);
        lineNo++;
        if (line.empty() || line[0] == '#') continue;

        int fields = (int)count(line.begin(), line.end(), '|') + 1;
        size_t bar = line.find('|');
        string name = trim(line.substr(0, bar));
        string parent = (bar == string::npos) ? "" : trim(line.substr(bar + 1));

        string reason;   
        if (fields != 2) reason = "wrong number of fields (expected 2, found " + to_string(fields) + ")";
        else if (name.empty() || parent.empty()) reason = "empty required field";
        else if (isEmpty() && !equalsIgnoreCase(parent, "NONE")) reason = "the first department must be the root with parent NONE";
        else add(name, parent, reason);

        if (reason.empty()) accepted++;
        else {
            cout << "Warning: departments.txt line " << lineNo << ": " << reason << " - record rejected.\n";
            rejected++;
        }
    }
    cout << "Loaded departments.txt: " << accepted << " accepted, " << rejected << " rejected\n";
}


void DepartmentTree::addFromMenu() {
    string name, parent, reason;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');   // discard the Enter left after the menu number
    if (!askName("Enter new department name: ", name)) return;
    if (!isEmpty() && !askName("Enter parent department name: ", parent)) return;
    if (add(name, parent, reason))
        cout << "Added \"" << name << "\"" << (parent.empty() ? " as the root department." : " under \"" + find(parent)->name + "\".") << "\n";
    else
        cout << "Error: " << reason << ". Tree unchanged.\n";
}


void DepartmentTree::searchFromMenu() const {
    string name;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');   
    if (!askName("Enter department name: ", name)) return;
    DepartmentNode* n = find(name);
    if (n == NULL) { cout << "Department \"" << name << "\" was not found.\n"; return; }
    int depth = 0;
    for (DepartmentNode* p = n->parent; p != NULL; p = p->parent) depth++;
    cout << "Found: " << n->name << "\nParent: " << (n->parent ? n->parent->name : "None")
         << "\nDirect children: " << n->children.size() << "\nDepth: " << depth << "\n";
}


string DepartmentTree::indented(const DepartmentNode* n, int depth) {
    string s = string(depth * 4, ' ') + n->name + "\n";
    for (size_t i = 0; i < n->children.size(); i++) s += indented(n->children[i], depth + 1);
    return s;
}


string DepartmentTree::preorder(const DepartmentNode* n) {
    string s = n->name;
    for (size_t i = 0; i < n->children.size(); i++) s += " -> " + preorder(n->children[i]);
    return s;
}


void DepartmentTree::displayHierarchy() const {
    cout << (root ? indented(root, 0) : "No departments are loaded.\n");
}


void DepartmentTree::displayPreorder() const {
    cout << (root ? "Preorder: " + preorder(root) + "\n" : "No departments are loaded.\n");
}
