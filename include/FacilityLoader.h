
#ifndef FACILITYLOADER_H
#define FACILITYLOADER_H

#include "HashTable.h"
#include "FacilityLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Read facility records from a text file and insert them into the hash table
bool loadFacilities(const std::string& filename, HashTable& ht);

// Checking the ID and to make it valid
std::string normalizeID(std::string id);
#endif
