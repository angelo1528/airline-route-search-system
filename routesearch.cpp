#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <string>
#include <climits>
#include <algorithm>
#include <sstream>
using namespace std;

// Global adjacency list: each city maps to a list of cities it flies to directly
unordered_map<string, vector<string>> graph;

// Stores all city names for iteration in question 4
vector<string> allCities;

// Reads flight.txt and builds the adjacency list
// Format: "From: CityName" followed by destination lines starting with "To :" or indented
void loadGraph(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: could not open " << filename << endl;
        exit(1);
    }
    string line;
    string currentCity = "";
    while (getline(file, line)) {
        if (line.empty()) continue;

        if (line.find("From:") != string::npos) {
            // Extract the source city name after "From:"
            size_t pos = line.find("From:") + 5;
            currentCity = line.substr(pos);
            currentCity.erase(0, currentCity.find_first_not_of(" \t"));
            currentCity.erase(currentCity.find_last_not_of(" \t\r\n") + 1);
            if (graph.find(currentCity) == graph.end()) {
                graph[currentCity] = {};
                allCities.push_back(currentCity);
            }
        } else if (!currentCity.empty()) {
            string dest = line;

            // Strip "To :" prefix if present on destination lines
            size_t toPos = dest.find("To :");
            if (toPos != string::npos) {
                dest = dest.substr(toPos + 4);
            }

            // Trim leading and trailing whitespace
            dest.erase(0, dest.find_first_not_of(" \t"));
            dest.erase(dest.find_last_not_of(" \t\r\n") + 1);

            if (dest.empty()) continue;
            if (dest.find("From:") != string::npos) continue;

            // Add directed edge from currentCity to dest
            if (!dest.empty()) {
                graph[currentCity].push_back(dest);
                if (graph.find(dest) == graph.end()) {
                    graph[dest] = {};
                    allCities.push_back(dest);
                }
            }
        }
    }
    file.close();
}

// BFS from a start city
// Returns:
//   dist   - shortest distance (in connections) from start to every reachable city
//   parent - which city each city was reached from, used to reconstruct paths
pair<unordered_map<string,int>, unordered_map<string,string>> bfs(string start) {
    unordered_map<string, int> dist;
    unordered_map<string, string> parent;
    queue<string> q;
    dist[start] = 0;
    parent[start] = "";
    q.push(start);
    while (!q.empty()) {
        string curr = q.front(); q.pop();
        for (string neighbor : graph[curr]) {
            // Only visit unvisited cities
            if (dist.find(neighbor) == dist.end()) {
                dist[neighbor] = dist[curr] + 1;
                parent[neighbor] = curr;
                q.push(neighbor);
            }
        }
    }
    return {dist, parent};
}

// Traces back through the parent map from dest to source
// Returns the path as a vector of city names in order from source to dest
vector<string> getPath(unordered_map<string,string>& parent, string dest) {
    vector<string> path;
    string curr = dest;
    while (curr != "") {
        path.push_back(curr);
        curr = parent[curr];
    }
    reverse(path.begin(), path.end());
    return path;
}

// Prints a path as "City A to City B to City C..."
void printPath(vector<string>& path) {
    for (int i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i < path.size() - 1) cout << " to ";
    }
    cout << endl;
}

// Question 1: Find shortest route from cityA to cityB with fewer than maxConn connections
// Uses BFS to find the shortest path, then checks if it meets the connection limit
void solveQ1(string cityA, string cityB, int maxConn) {
    auto [dist, parent] = bfs(cityA);
    if (dist.find(cityB) == dist.end()) {
        cout << "There is no route from " << cityA << " to " << cityB << endl;
        return;
    }
    int connections = dist[cityB];
    if (connections >= maxConn) {
        cout << "No route with less than " << maxConn << " connections exists." << endl;
        cout << "Shortest available is " << connections << " connections." << endl;
        return;
    }
    vector<string> path = getPath(parent, cityB);
    printPath(path);
    cout << "Total connections: " << connections << endl;
}

// Question 2: Shortest path from cityA to cityD passing through both cityB and cityC
// Tries both orderings (A->B->C->D and A->C->B->D) and picks the shorter one
void solveQ2(string cityA, string cityB, string cityC, string cityD) {
    auto [distA, parentA] = bfs(cityA);
    auto [distB, parentB] = bfs(cityB);
    auto [distC, parentC] = bfs(cityC);
    auto [distC2, parentC2] = bfs(cityC);
    auto [distB2, parentB2] = bfs(cityB);
    int total1 = INT_MAX, total2 = INT_MAX;

    // Order 1: A -> B -> C -> D
    if (distA.count(cityB) && distB.count(cityC) && distC.count(cityD))
        total1 = distA[cityB] + distB[cityC] + distC[cityD];

    // Order 2: A -> C -> B -> D
    if (distA.count(cityC) && distC2.count(cityB) && distB2.count(cityD))
        total2 = distA[cityC] + distC2[cityB] + distB2[cityD];

    if (total1 == INT_MAX && total2 == INT_MAX) {
        cout << "No such route exists." << endl;
        return;
    }

    // Build and print the path for whichever ordering was shorter
    if (total1 <= total2) {
        vector<string> p1 = getPath(parentA, cityB);
        vector<string> p2 = getPath(parentB, cityC);
        vector<string> p3 = getPath(parentC, cityD);
        vector<string> full = p1;
        for (int i = 1; i < p2.size(); i++) full.push_back(p2[i]);
        for (int i = 1; i < p3.size(); i++) full.push_back(p3[i]);
        printPath(full);
        cout << "Smallest number of connections: " << total1 << endl;
    } else {
        vector<string> p1 = getPath(parentA, cityC);
        vector<string> p2 = getPath(parentC2, cityB);
        vector<string> p3 = getPath(parentB2, cityD);
        vector<string> full = p1;
        for (int i = 1; i < p2.size(); i++) full.push_back(p2[i]);
        for (int i = 1; i < p3.size(); i++) full.push_back(p3[i]);
        printPath(full);
        cout << "Smallest number of connections: " << total2 << endl;
    }
}

// Recursive DFS helper for question 3
// Visits all unvisited neighbors of current city and adds them to the tour
void dfs(string current, unordered_set<string>& visited, vector<string>& tour) {
    for (string neighbor : graph[current]) {
        if (visited.find(neighbor) == visited.end()) {
            visited.insert(neighbor);
            tour.push_back(neighbor);
            dfs(neighbor, visited, tour);
        }
    }
}

// Question 3: Start at cityA, visit all reachable cities, return to cityA
// Uses DFS to visit as many cities as possible following directed edges
// Then uses BFS to find the shortest return path back to the start
void solveQ3(string cityA) {
    unordered_set<string> visited;
    vector<string> tour;

    visited.insert(cityA);
    tour.push_back(cityA);

    // DFS to visit all reachable cities
    dfs(cityA, visited, tour);

    if (tour.size() == 1) {
        cout << "No other cities are reachable from " << cityA << endl;
        return;
    }

    // BFS from last visited city to find shortest path back to start
    string lastCity = tour.back();
    auto [distBack, parentBack] = bfs(lastCity);

    if (distBack.find(cityA) == distBack.end()) {
        cout << "No route back to " << cityA << " exists from " << lastCity << endl;
        cout << "Partial tour: ";
        printPath(tour);
        return;
    }

    // Append the return path to the tour
    vector<string> returnPath = getPath(parentBack, cityA);
    for (int i = 1; i < returnPath.size(); i++) tour.push_back(returnPath[i]);

    // Total connections = number of hops = cities visited minus 1
    int totalCost = tour.size() - 1;

    printPath(tour);
    cout << "Smallest number of connections: " << totalCost << endl;
}

// Question 4: Find the city that minimizes total connections for 3 people to meet
// Runs BFS from each of the 3 cities, then checks every candidate city
// Picks the city with the lowest combined distance from all three starting cities
void solveQ4(string cityA, string cityB, string cityC) {
    auto [distA, parentA] = bfs(cityA);
    auto [distB, parentB] = bfs(cityB);
    auto [distC, parentC] = bfs(cityC);
    string bestCity = "";
    int bestTotal = INT_MAX;

    for (string candidate : allCities) {
        // Skip the three starting cities
        if (candidate == cityA || candidate == cityB || candidate == cityC) continue;

        // Only consider cities reachable from all three starting cities
        if (distA.count(candidate) && distB.count(candidate) && distC.count(candidate)) {
            int total = distA[candidate] + distB[candidate] + distC[candidate];
            if (total < bestTotal) {
                bestTotal = total;
                bestCity = candidate;
            }
        }
    }

    if (bestCity == "") {
        cout << "No such meeting city exists." << endl;
        return;
    }

    cout << "You three should meet at " << bestCity << endl;

    vector<string> pathA = getPath(parentA, bestCity);
    cout << "Route for first person: ";
    printPath(pathA);
    cout << "(" << distA[bestCity] << " connections)" << endl;

    vector<string> pathB = getPath(parentB, bestCity);
    cout << "Route for second person: ";
    printPath(pathB);
    cout << "(" << distB[bestCity] << " connections)" << endl;

    vector<string> pathC = getPath(parentC, bestCity);
    cout << "Route for third person: ";
    printPath(pathC);
    cout << "(" << distC[bestCity] << " connections)" << endl;

    cout << "Total number of connections: " << bestTotal << endl;
}

int main(int argc, char* argv[]) {
    // Load the flight graph from file
    loadGraph("flight.txt");

    // Temporary debug: print Moscow's direct neighbors to verify parser
    for (string neighbor : graph["Moscow, Russia"]) {
        cout << "NEIGHBOR: " << neighbor << endl;
    }

    if (argc < 2) {
        cout << "Usage: routeSearch <question#> <args...>" << endl;
        return 1;
    }

    // Parse question number and dispatch to the correct solver
    int question = atoi(argv[1]);
    if (question == 1) {
        if (argc < 5) { cout << "Usage: routeSearch 1 <cityA> <cityB> <num_connections>" << endl; return 1; }
        solveQ1(argv[2], argv[3], atoi(argv[4]));
    } else if (question == 2) {
        if (argc < 9) { cout << "Usage: routeSearch 2 <cityA> through <cityB> and <cityC> to <cityD>" << endl; return 1; }
        solveQ2(argv[2], argv[4], argv[6], argv[8]);
    } else if (question == 3) {
        if (argc < 3) { cout << "Usage: routeSearch 3 <cityA>" << endl; return 1; }
        solveQ3(argv[2]);
    } else if (question == 4) {
        if (argc < 5) { cout << "Usage: routeSearch 4 <cityA> <cityB> <cityC>" << endl; return 1; }
        solveQ4(argv[2], argv[3], argv[4]);
    } else {
        cout << "Question must be 1-4" << endl;
        return 1;
    }
    return 0;
}