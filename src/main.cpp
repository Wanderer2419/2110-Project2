
#include <iostream>
#include <string>
#include <limits>
#include "FacilityLoader.h"
#include "HashTable.h"
#include "CampusGraph.h"
#include "DepartmentTree.h"

using namespace std;

int main(int argc, char* argv[]) {

    string dataDirectory;
    cout << "Number of command line arguments = " + to_string(argc) << endl;
// This causes segmentation: Core dump ______________
/*    if (argc >= 2) {
        cout << "Too many command line arguments: Using data/ as data directory." << endl;
        dataDirectory = "data";
    } else {
        dataDirectory = argv[1];
    }*/

    dataDirectory = "data";

    HashTable ht;
    CampusGraph graph;
    string startingBuilding;
    DepartmentTree departments;

    // Load facilities from file
    if(!loadFacilities("data/facilities.txt", ht)) {
        return 1;
    }
    graph.loadFiles(dataDirectory);
    departments.loadFromFile(dataDirectory);

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
                graph.displayCampusGraph();
                break;
            
            case 4:
                graph.displayGraphInfo();
                break;
            
            case 5:
                cin.ignore();
                cout << "Enter the building to start the BFS algorithm from: " << endl;
                getline(cin, startingBuilding);
                graph.displayReachability(startingBuilding);
                break;
            case 6:
                departments.addFromMenu();
                break;

            case 7:
                departments.searchFromMenu();
                break;

            case 8:
                departments.displayHierarchy();
                break;

            case 9:
                departments.displayPreorder();
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
