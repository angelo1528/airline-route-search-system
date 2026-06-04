# World Airline Route Search

A graph-based airline route analysis system developed in C++ for CSCE 2110 (Foundations of Data Structures).

The program models a worldwide flight network as a directed graph and uses graph traversal algorithms to solve route planning and optimization problems.

## Technologies

- C++
- Graph Data Structures
- Breadth-First Search (BFS)
- Depth-First Search (DFS)
- STL Containers
- File Processing

## Features

### Route Search with Connection Limits
Find the shortest route between two cities while respecting a maximum number of allowed connections.

### Multi-Stop Route Planning
Compute the shortest route that passes through required waypoint cities.

### Graph Traversal Tour
Use DFS to visit reachable cities and BFS to determine the shortest return path.

### Optimal Meeting Location
Determine the city that minimizes total travel distance for multiple travelers.

## Algorithms Used

| Feature | Algorithm |
|----------|-----------|
| Route Search | BFS |
| Multi-Stop Planning | BFS |
| City Tour | DFS + BFS |
| Meeting Location | Multi-source BFS |

## Complexity

All route searches execute in:

O(V + E)

where:
- V = Number of cities
- E = Number of flight routes

## How to Compile

g++ -std=c++17 -o routeSearch main.cpp

## How to Run

Example:

./routeSearch 1 "Moscow, Russia" "Tokyo, Japan" 5

## Project Structure

main.cpp      - Source code
flight.txt    - Flight route dataset
README.md     - Project documentation
