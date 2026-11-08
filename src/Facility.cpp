#include "Facility.h"
#include <iostream>

// Make constructor using an initializer list
Facility::Facility(const std::string& id, const std::string& name, const std::string& building, 
             const std::string& category) : Facility_ID(id), Facility_Name(name), Building_Name(building),
             Category(category) {}


// Returns the facility ID
std::string Facility::getID() const {
    return Facility_ID;
}   

// Returns the faciltiy name
std::string Facility::getName() const {
    return Facility_Name;
}  

// Return the building name
std::string Facility::getBuilding() const {
    return Building_Name;
}

// Return the category
std::string Facility::getCategory() const {
    return Category;
}

// Display All
void Facility::display() const {
    std::cout << "ID: " << Facility_ID << std::endl;
    std::cout << "Name: " << Facility_Name << std::endl;
    std::cout << "Building: " << Building_Name << std::endl;
    std::cout << "Category: " << Category << std::endl;
} 