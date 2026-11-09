output: src/main.o src/Facility.o src/FacilityLoader.o src/HashTable.o src/DepartmentTree.o src/CampusGraph.o
	g++ src/main.o src/Facility.o src/FacilityLoader.o src/HashTable.o src/DepartmentTree.o src/CampusGraph.o -o output
	rm -f src/*.o

src/main.o: src/main.cpp
	g++ -c src/main.cpp -Iinclude -o src/main.o

src/Facility.o: src/Facility.cpp include/Facility.h
	g++ -c src/Facility.cpp -Iinclude -o src/Facility.o

src/FacilityLoader.o: src/FacilityLoader.cpp include/FacilityLoader.h include/Facility.h
	g++ -c src/FacilityLoader.cpp -Iinclude -o src/FacilityLoader.o

src/HashTable.o: src/HashTable.cpp include/HashTable.h include/Facility.h
	g++ -c src/HashTable.cpp -Iinclude -o src/HashTable.o

src/DepartmentTree.o: src/DepartmentTree.cpp include/DepartmentTree.h
	g++ -c src/DepartmentTree.cpp -Iinclude -o src/DepartmentTree.o

src/CampusGraph.o: src/CampusGraph.cpp include/CampusGraph.h
	g++ -c src/CampusGraph.cpp -Iinclude -o src/CampusGraph.o

clean:
	rm -f src/*.o output
