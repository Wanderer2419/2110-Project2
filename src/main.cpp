
#include <iostream>
#include <string>
#include <limits>
#include "FacilityLoader.h"
#include "HashTable.h"

using namespace std;

int main() {

    HashTable ht;

    // Load facilities from file
    if(!loadFacilities("data/facilities.txt", ht)) {
        return 1;
    }

    int choice = 0;

    while(choice != 10) {

        cout << "\n========== SMART CAMPUS NAVIGATION ==========\n";
        cout << "1. View all facilities\n";
        cout << "2. Search facility by ID\n";
        cout << "3. View campus graph\n";
        cout << "4. Display graph counts and degrees\n";
        cout << "5. Find reachable buildings (BFS)\n";
        cout << "6. Add department\n";
        cout << "7. Search department\n";
        cout << "8. Display department hierarchy\n";
        cout << "9. Run preorder traversal\n";
        cout << "10. Exit\n";

        cout << "\nEnter your choice: ";

        if(!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice!\n";
            choice = 0;
        }

        switch(choice) {

            case 1:
                cout << "\n===== All Facilities =====\n";
                ht.displayAll();
                break;

            case 2: {
                string id;

                cout << "Enter Facility ID: ";
                cin >> id;

                const Facility* result = ht.searchByID(id);

                if(result != nullptr) {
                    cout << "\nFacility Found!\n";
                    result->display();
                }
                else {
                    cout << "Facility Not Found!\n";
                }

                break;
            }

            case 3:
            case 4:
            case 5:
                cout << "Graph feature: Teammate B\n";
                break;

            case 6:
            case 7:
            case 8:
            case 9:
                cout << "Department feature: Teammate C\n";
                break;

            case 10:
                cout << "Exiting program...\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }
    }

    return 0;
}
