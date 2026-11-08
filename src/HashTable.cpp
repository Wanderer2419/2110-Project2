
#include "HashTable.h"
#include <iostream>
#include <cctype>

// Constructor: Initialize hash table and counters
HashTable::HashTable(size_t capacity)
    : table(capacity == 0 ? 101 : capacity),
      count(0),
      collisions(0) {}

// Hash function: Convert facility ID into bucket index
size_t HashTable::hashFunction(const std::string& id) const {
    size_t hash = 0;

    for (char ch : id) {
        hash = (hash * 31 + static_cast<unsigned char>(ch))
               % table.size();
    }

    return hash;
}

// Insert a facility into the hash table
bool HashTable::insert(const Facility& facility) {
    std::string id = facility.getID();

    // Calculate bucket index
    size_t index = hashFunction(id);

    // Check for duplicate facility IDs
    for (const Facility& existing : table[index]) {
        if (existing.getID() == id) {
            return false;
        }
    }

    // Count collision if bucket is not empty
    if (!table[index].empty()) {
        collisions++;
    }

    // Store facility with normalized ID
    Facility normalizedFacility(
        id,
        facility.getName(),
        facility.getBuilding(),
        facility.getCategory()
    );

    // Insert facility into the linked list
    table[index].push_back(facility);

    // Increase facility count
    count++;

    return true;
}

// Search for a facility using its ID

const Facility* HashTable::searchByID(const std::string& id) const {

    std::string normalizedID = normalizeID(id);

    if(!isValidID(normalizedID)) {
        return nullptr;
    }

    size_t index = hashFunction(normalizedID);

    for(const Facility& facility : table[index]) {
        if(facility.getID() == normalizedID) {
            return &facility;
        }
    }

    return nullptr;
}

// Return number of stored facilities
size_t HashTable::size() const {
    return count;
}

// Return number of collisions
size_t HashTable::collisionCount() const {
    return collisions;
}

// Display all facilities
void HashTable::displayAll() const {
    for (const auto& bucket : table) {
        for (const Facility& facility : bucket) {
            facility.display();
            std::cout << "--------------------\n";
        }
    }
}

bool isValidID(const std::string& id) {
    if(id.length() != 4 || id[0] != 'F') {
        return false;
    }

    for(int i = 1; i < 4; i++) {
        if(id[i] < '0' || id[i] > '9') {
            return false;
        }
    }

    return true;
}
