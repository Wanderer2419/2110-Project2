#include "DepartmentTree.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>

static string trim(const string& s) {
    size_t b = 0, e = s.size();
    while (b < e && isspace(static_cast<unsigned char>(s[b]))) b++;
    while (e > b && isspace(static_cast<unsigned char>(s[e - 1]))) e--;
    return s.substr(b, e - b);
}

static bool equalsIgnoreCase(const string& a, const string& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++)
        if (tolower(static_cast<unsigned char>(a[i])) != tolower(static_cast<unsigned char>(b[i])))
            return false;
    return true;
}

DepartmentNode* DepartmentTree::findIn(DepartmentNode* n, const string& name) {
    if (n == NULL) return NULL;
    if (equalsIgnoreCase(n->name, name)) return n;
    for (size_t i = 0; i < n->children.size(); i++) {
        DepartmentNode* found = findIn(n->children[i], name);
        if (found != NULL) return found;
    }
    return NULL;
}

bool DepartmentTree::add(const string& name, const string& parentName, string& reason) {
    if (find(name) != NULL) {
        reason = "duplicate department name \"" + name + "\"";
        return false;
    }
    if (root == NULL) {
        root = new DepartmentNode(name, NULL);
        return true;
    }
    DepartmentNode* parent = find(parentName);
    if (parent == NULL) {
        reason = "unknown parent \"" + parentName + "\"";
        return false;
    }
    parent->children.push_back(new DepartmentNode(name, parent));
    return true;
}


void DepartmentTree::loadFromFile(const string& dataDir) {
    const string fileName = "departments.txt";
    ifstream in((dataDir + "/" + fileName).c_str());
    if (!in) cout << "Error: cannot open file " << fileName << "; treating it as empty." << endl;

    int accepted = 0, rejected = 0, lineNo = 0;
    string line;
    while (getline(in, line)) {  
        lineNo++;
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;  

        int fields = static_cast<int>(count(line.begin(), line.end(), '|')) + 1;
        size_t bar = line.find('|');
        string name = trim(line.substr(0, bar));
        string parent = (bar == string::npos) ? "" : trim(line.substr(bar + 1));

        string reason; 
        if (fields != 2)
            reason = "wrong number of fields (expected 2, found " + to_string(fields) + ")";
        else if (name.empty() || parent.empty())
            reason = "empty required field";
        else if (isEmpty() && !equalsIgnoreCase(parent, "NONE"))
            reason = "the first department must be the root with parent NONE";
        else
            add(name, parent, reason);  

        if (reason.empty()) {
            accepted++;
        } else {
            cout << "Warning: " << fileName << " line " << lineNo << ": " << reason
                 << " - record rejected." << endl;
            rejected++;
        }
    }
    cout << "Loaded " << fileName << ": " << accepted << " accepted, " << rejected << " rejected" << endl;
}

void DepartmentTree::displaySearch(const string& name) const {
    DepartmentNode* n = find(name);
    if (n == NULL) {
        cout << "Department \"" << name << "\" was not found." << endl;
        return;
    }
    int depth = 0;
    for (DepartmentNode* p = n->parent; p != NULL; p = p->parent) depth++;
    cout << "Found: " << n->name << endl;
    cout << "Parent: " << (n->parent == NULL ? string("None") : n->parent->name) << endl;
    cout << "Direct children: " << n->children.size() << endl;
    cout << "Depth: " << depth << endl;
}

void DepartmentTree::printIndented(const DepartmentNode* n, int depth) {
    cout << string(depth * 4, ' ') << n->name << endl;
    for (size_t i = 0; i < n->children.size(); i++) printIndented(n->children[i], depth + 1);
}

void DepartmentTree::printPreorder(const DepartmentNode* n, bool& first) {
    if (!first) cout << " -> ";
    cout << n->name;
    first = false;
    for (size_t i = 0; i < n->children.size(); i++) printPreorder(n->children[i], first);
}

void DepartmentTree::displayHierarchy() const {
    if (root == NULL) {
        cout << "No departments are loaded." << endl;
        return;
    }
    printIndented(root, 0);
}

void DepartmentTree::displayPreorder() const {
    if (root == NULL) {
        cout << "No departments are loaded." << endl;
        return;
    }
    bool first = true;
    cout << "Preorder: ";
    printPreorder(root, first);
    cout << endl;
}

