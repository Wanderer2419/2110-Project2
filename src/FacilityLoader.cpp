#include "Facility.h"
#include "HashTable.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

// Read facility records from a text file and insert them into the hash table
bool loadFacilities(const std::string& filename, HashTable& ht) {

    // Try to open the file
    std::ifstream file(filename);
    if(!file.is_open()) {
        std::cout << "File could not open" << std::endl;
        return false;
    }

    std::string line;
    while(getline(file, line)) {
        std::string id, name, building, category;
        std::stringstream ss(line);

        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, building, '|');
        getline(ss, category, '|');

        // check if name and category has values
        if(!id.empty() && !name.empty() && !building.empty() && !category.empty()) {

            // Normalize the Facility ID
            id = normalizeID(id);

            // Check if ID is valid
            if(!isValidID(id)) {
                std::cout << "Invalid Facility ID: "
                          << id << std::endl;
            }
            else {
                Facility facility(id, name, building, category);

                // Check duplicate ID
                if(!ht.insert(facility)) {
                    std::cout << "Duplicate facility ID: "
                              << id << std::endl;
                }
            }
        }
        else {
            std::cout << "Invalid Facility Record: "
                      << line << std::endl;
        }
    }

    return true;
}

std::string normalizeID(std::string id) {

    // Convert lowercase letters to uppercase
    for (char& ch : id) {
        ch = static_cast<char>(
            std::toupper(static_cast<unsigned char>(ch))
        );
    }

    // Add F if only three digits were entered
    if (id.length() == 3) {
        id = "F" + id;
    }

    return id;
}

