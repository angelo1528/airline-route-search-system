========================================
World Airline Route Search
CSCE 2110 - Foundation of Data Structures
========================================

OVERVIEW
--------
This program reads a flight route dataset and answers four questions
about shortest airline routes using graph search algorithms.


FILES INCLUDED
--------------
main.cpp        - Full source code
flight.txt      - Airline route dataset
README.txt      - This file
report.docx     - Project report


COMPILATION
-----------
Requires g++ with C++17 support.

    g++ -std=c++17 -o routeSearch main.cpp

Make sure flight.txt is in the same directory as the compiled binary.


RUNNING THE PROGRAM
-------------------
The program takes the question number as the first argument followed
by the required city names. City names with spaces or commas must be
wrapped in double quotes.


QUESTION 1 - Route with connection limit
    Usage:
        ./routeSearch 1 <cityA> <cityB> <num_connections>

    Example:
        ./routeSearch 1 "Moscow, Russia" "Tokyo, Japan" 5

    Output:
        The shortest route from A to B if it has fewer than
        num_connections connections. If no such route exists,
        reports the shortest available and its connection count.


QUESTION 2 - Route through waypoints
    Usage:
        ./routeSearch 2 <cityA> through <cityB> and <cityC> to <cityD>

    Example:
        ./routeSearch 2 "Moscow, Russia" through "Seoul, South Korea" and "Tokyo, Japan" to "London, United Kingdom"

    Output:
        The shortest route from A to D that passes through both
        B and C. Both orderings (A-B-C-D and A-C-B-D) are tried
        and the shorter one is returned.


QUESTION 3 - Visit all cities and return
    Usage:
        ./routeSearch 3 <cityA>

    Example:
        ./routeSearch 3 "Moscow, Russia"

    Output:
        A route that starts at A, visits as many reachable cities
        as possible using DFS, then returns to A using BFS to find
        the shortest return path. If no return path exists, a
        partial tour is printed.


QUESTION 4 - Find optimal meeting city
    Usage:
        ./routeSearch 4 <cityA> <cityB> <cityC>

    Example:
        ./routeSearch 4 "Moscow, Russia" "Seoul, South Korea" "Tokyo, Japan"

    Output:
        The city that minimizes the total number of connections
        for all three people to travel to. Prints the route and
        connection count for each person separately.


NOTES
-----
- City names must match exactly as they appear in flight.txt
- Use double quotes around any city name containing spaces or commas
- The graph is directed, so flights are one-way unless both directions
  appear in flight.txt
- Question 3 may report no return path for some starting cities due
  to the directed nature of the graph


ALGORITHMS USED
---------------
Q1  - BFS from source city, checks distance against limit
Q2  - BFS run on each leg, both orderings compared
Q3  - DFS to visit cities, BFS to find return path
Q4  - BFS from all three cities, minimum total distance found

Time complexity for all questions: O(V + E)
where V = number of cities, E = number of flight routes
