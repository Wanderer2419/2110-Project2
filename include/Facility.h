#ifndef Facility_H
#define Facility_H

#include <string>

class Facility {
private:
    std::string Facility_ID;      // string cause 1 word and 3 numbers
    std::string Facility_Name;
    std::string Building_Name;
    std::string Category;

public:

    // Make constructor using an initializer list
    Facility(const std::string& id, const std::string& name, const std::string& building, 
             const std::string& category);

    std::string getID() const;   // Returns the facility ID
    std::string getName() const;  // Returns the faciltiy name
    std::string getBuilding() const; // Return the building name
    std::string getCategory() const; // Return the category
    void display() const;    // Display all 

};


#endif