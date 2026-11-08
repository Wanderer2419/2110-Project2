
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Facility.h"
#include <vector>
#include <list>
#include <string>

class HashTable {
private:
    // Hash table: vector of linked lists
    std::vector<std::list<Facility>> table;

    // The Number of facilities
    size_t count;

    // The Number of collisions
    size_t collisions;

    // Convert ID into bucket index
    size_t hashFunction(const std::string& id) const;

public:
    // Constructor
    HashTable(size_t capacity = 101);

    // Insert a facility
    bool insert(const Facility& facility);

    // Search by facility ID
    const Facility* searchByID(const std::string& id) const;

    // Statistics
    size_t size() const;
    size_t collisionCount() const;

    // Display stored facilities
    void displayAll() const;
};

// Functions
std::string normalizeID(std::string id);
bool isValidID(const std::string& id);

#endif
