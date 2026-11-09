#ifndef DEPARTMENTTREE_H
#define DEPARTMENTTREE_H

#include <string>
#include <vector>
using namespace std;

struct DepartmentNode {
    string name;
    DepartmentNode* parent;
    vector<DepartmentNode*> children;  

    DepartmentNode(const string& n, DepartmentNode* p) : name(n), parent(p) {}
  
    ~DepartmentNode() {
        for (size_t i = 0; i < children.size(); i++) delete children[i];
    }
};

class DepartmentTree {
public:
    DepartmentTree() : root(NULL) {}
    ~DepartmentTree() { delete root; }

    bool isEmpty() const { return root == NULL; }
    DepartmentNode* find(const string& name) const { return findIn(root, name); }
    bool add(const string& name, const string& parentName, string& reason);


    void loadFromFile(const string& dataDir);

    void displaySearch(const string& name) const;  
    void displayHierarchy() const;                 
    void displayPreorder() const;                  

private:
    DepartmentNode* root;
    static DepartmentNode* findIn(DepartmentNode* n, const string& name);
    static void printIndented(const DepartmentNode* n, int depth);
    static void printPreorder(const DepartmentNode* n, bool& first);

    DepartmentTree(const DepartmentTree&);
    DepartmentTree& operator=(const DepartmentTree&);
};

#endif
