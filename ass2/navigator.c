
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RoadMap.h"
#include "navigator.h"
#include "time.h"

#define MINUTES_PER_HOUR 60

static bool findShortestPath(RoadMap map, int fromNode, int toNode, int *dist,
                             int *prev, bool *visited);
static void getRoute(RoadMap map, int fromNode, int toNode, struct time time,
                     int *dist, int *prev, bool *visited, struct route *route);
static int minDist(int dist[], bool visited[], int numNodes);
static int RoadMapGetTravelTime(RoadMap map, int fromNode, int toNode);
static struct time TimeAddMinutes(struct time time, int minutes);
static int compareTime(struct time t1, struct time t2);
static int minDistance(struct time dist[], bool visited[], int numNodes);
static bool RoadMapIsClosed(RoadMap map, struct road road, struct time time);
static int getMinutes(struct time start, struct time end);

////////////////////////////////////////////////////////////////////////
// Task 3

struct route navigate(RoadMap map, int fromNode, int toNode, struct time time) {
    struct route route = {
        .fromNode = fromNode,
        .toNode = toNode,
        .numLegs = 0,
        .legs = NULL,
    };

    // use djikstra's algorithm to find shortest path
    // initialise
    int *dist = malloc(sizeof(int) * RoadMapNumNodes(map));
    int *prev = malloc(sizeof(int) * RoadMapNumNodes(map));
    bool *visited = malloc(sizeof(bool) * RoadMapNumNodes(map));
    if (dist == NULL || prev == NULL || visited == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < RoadMapNumNodes(map); i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
        visited[i] = false;
    }

    // find shortest path
    bool isPath = findShortestPath(map, fromNode, toNode, dist, prev, visited);

    // Reconstruct and store the path, if there is path
    if (isPath) {
        getRoute(map, fromNode, toNode, time, dist, prev, visited, &route);
    }

    free(dist);
    free(prev);
    free(visited);
    return route;
}

////////////////////////////////////////////////////////////////////////
// Task 4

struct route advancedNavigate(RoadMap map, int fromNode, int toNode,
                              struct time time) {
    struct route route = {
        .fromNode = fromNode,
        .toNode = toNode,
        .numLegs = 0,
        .legs = NULL,
    };

    int numNodes = RoadMapNumNodes(map);
    // Distance values are now time values.
    struct time *dist = malloc(sizeof(struct time) * numNodes);
    int *prev = malloc(sizeof(int) * numNodes);
    bool *visited = malloc(sizeof(bool) * numNodes);
    if (dist == NULL || prev == NULL || visited == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    // Initialize distances as infinity and visited array as false
    for (int i = 0; i < numNodes; i++) {
        dist[i] = (struct time){.hour = 99,
                                .minute = 99}; // Assuming 24:60 as infinity
        prev[i] = -1;
        visited[i] = false;
    }

    // Distance to source is the departure time
    dist[fromNode] = time;
    prev[fromNode] = fromNode;

    int u = fromNode;
    for (int count = 0; count < numNodes - 1 && u != -1; count++) {

        // Mark the picked vertex as processed
        visited[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex
        struct road *fromRoads =
            malloc(sizeof(struct road) * MAX_ROADS_PER_NODE);
        if (fromRoads == NULL) {
            fprintf(stderr, "error: out of memory\n");
            exit(EXIT_FAILURE);
        }
        int numFromRoads = RoadMapGetRoadsFrom(map, u, fromRoads);
        for (int i = 0; i < numFromRoads; i++) {
            if (visited[fromRoads[i].toNode]) {
                continue;
            }

            struct time travelTime = dist[u];

            if (RoadMapHasTrafficLights(map, fromRoads[i].toNode) &&
                fromRoads[i].toNode != toNode) {
                fromRoads[i].travelMinutes += TRAFFIC_LIGHT_WAIT;
            }

            travelTime = TimeAddMinutes(dist[u], fromRoads[i].travelMinutes);

            // Check for road closure and wait until it opens
            struct time roadOpenTime = fromRoads[i].closedUntil;
            if (RoadMapIsClosed(map, fromRoads[i], travelTime)) {
                travelTime = roadOpenTime;
                travelTime =
                    TimeAddMinutes(travelTime, fromRoads[i].travelMinutes);
            }

            if (compareTime(travelTime, dist[fromRoads[i].toNode]) < 0) {
                dist[fromRoads[i].toNode] = travelTime;
                prev[fromRoads[i].toNode] = u;
            }
        }
        free(fromRoads);
        // printf("\t\t\t\tcurrNode<%d>\n", u);
        // // print dist and prev
        // printf("node: ");
        // for (int i = 0; i < numNodes; i++) { printf("%5d ", i); }
        // printf("\n");
        // printf("dist: ");
        // for (int i = 0; i < numNodes; i++) { printf("%2d:%2d ", dist[i].hour,
        // dist[i].minute); } printf("\n"); printf("prev: "); for (int i = 0; i
        // < numNodes; i++) { printf("%5d ", prev[i]); } printf("\n");

        // Find the minimum distance node from the set of vertices not yet
        // processed
        u = minDistance(dist, visited, numNodes);
    }

    // store and reverse tha path
    int *path = malloc(sizeof(int) * RoadMapNumNodes(map));
    if (path == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    int pathLength = 0;
    int currNode = toNode;
    while (currNode != fromNode) {
        path[pathLength] = currNode;
        pathLength++;
        currNode = prev[currNode];
    }
    path[pathLength] = fromNode;
    pathLength++;
    // reverse path
    for (int i = 0; i < pathLength / 2; i++) {
        int temp = path[i];
        path[i] = path[pathLength - i - 1];
        path[pathLength - i - 1] = temp;
    }

    // // print path
    // printf("path: ");
    // for (int i = 0; i < pathLength-1; i++) { printf("%d -> ", path[i]); }
    // printf("%d", path[pathLength-1]);
    // printf(" with pathLength: %d\n", pathLength);

    // store route and legs
    route.numLegs = pathLength - 1;
    route.legs = malloc(sizeof(struct leg) * route.numLegs);
    if (route.legs == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    // reverse get route
    int indexPath = pathLength - 1;
    struct time currTime = dist[toNode];
    for (int i = route.numLegs - 1; i >= 0; i--) {
        // printf("i: %d; indexPath :%d\n", i, indexPath);

        route.legs[i].fromNode = path[indexPath - 1];
        route.legs[i].toNode = path[indexPath];
        route.legs[i].duration =
            RoadMapGetTravelTime(map, path[indexPath - 1], path[indexPath]);

        route.legs[i].end = currTime;
        route.legs[i].start = TimeAddMinutes(currTime, -route.legs[i].duration);
        indexPath--;
        currTime = route.legs[i].start;
    }
    route.start = route.legs[0].start;
    route.end = route.legs[route.numLegs - 1].end;
    route.totalDuration = getMinutes(route.start, route.end);

    free(dist);
    free(prev);
    free(visited);
    free(path);

    return route;
}

////////////////////////////////////////////////////////////////////////
// Helper functions

// task 3 find shortest path by djikstra's algorithm
static bool findShortestPath(RoadMap map, int fromNode, int toNode, int *dist,
                             int *prev, bool *visited) {
    dist[fromNode] = 0;
    prev[fromNode] = fromNode;
    visited[fromNode] = true;

    int currNode = fromNode;
    for (int count = 0; count < RoadMapNumNodes(map) - 1; count++) {
        // update distances
        struct road *fromRoads =
            malloc(sizeof(struct road) * MAX_ROADS_PER_NODE);
        if (fromRoads == NULL) {
            fprintf(stderr, "error: out of memory\n");
            exit(EXIT_FAILURE);
        }
        int numFromRoads = RoadMapGetRoadsFrom(map, currNode, fromRoads);
        for (int i = 0; i < numFromRoads; i++) {
            if (visited[fromRoads[i].toNode]) {
                continue;
            }
            // add traffic light wait time
            if (RoadMapHasTrafficLights(map, fromRoads[i].toNode) &&
                fromRoads[i].toNode != toNode) {
                fromRoads[i].travelMinutes += TRAFFIC_LIGHT_WAIT;
            }
            if (dist[fromRoads[i].toNode] >
                dist[currNode] + fromRoads[i].travelMinutes) {
                dist[fromRoads[i].toNode] =
                    dist[currNode] + fromRoads[i].travelMinutes;
                prev[fromRoads[i].toNode] = currNode;
            }
        }

        // mark node as visited
        visited[currNode] = true;

        // find next node to visit
        currNode = minDist(dist, visited, RoadMapNumNodes(map));

        // no accessible node was found
        if (currNode == -1) {
            free(fromRoads);
            return false;
        }

        free(fromRoads);
    }
    return true;
}

// task 3 get route
static void getRoute(RoadMap map, int fromNode, int toNode, struct time time,
                     int *dist, int *prev, bool *visited, struct route *route) {
    // store and reverse tha path
    int *path = malloc(sizeof(int) * RoadMapNumNodes(map));
    if (path == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    int pathLength = 0;
    int currNode = toNode;
    while (currNode != fromNode) {
        path[pathLength] = currNode;
        pathLength++;
        currNode = prev[currNode];
    }
    path[pathLength] = fromNode;
    pathLength++;
    for (int i = 0; i < pathLength / 2; i++) {
        int temp = path[i];
        path[i] = path[pathLength - i - 1];
        path[pathLength - i - 1] = temp;
    }

    // store route and legs
    route->numLegs = pathLength - 1;
    route->legs = malloc(sizeof(struct leg) * route->numLegs);
    if (route->legs == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    struct time currTime = time;
    for (int i = 0; i < route->numLegs; i++) {
        route->legs[i].fromNode = path[i];
        route->legs[i].toNode = path[i + 1];
        route->legs[i].duration =
            RoadMapGetTravelTime(map, path[i], path[i + 1]);
        // find travel time, and remove traffic light wait time
        if (RoadMapHasTrafficLights(map, path[i]) && path[i] != fromNode) {
            route->legs[i].start = TimeAddMinutes(currTime, TRAFFIC_LIGHT_WAIT);
            currTime = route->legs[i].start;
        } else {
            route->legs[i].start = currTime;
        }
        route->legs[i].end = TimeAddMinutes(currTime, route->legs[i].duration);
        currTime = route->legs[i].end;
    }
    route->start = route->legs[0].start;
    route->end = route->legs[route->numLegs - 1].end;
    route->totalDuration = dist[toNode];
    free(path);
}

// task 3 find min distance
static int minDist(int dist[], bool visited[], int numNodes) {
    // Initialize min value as the maximum possible time
    int min = INT_MAX;
    int min_index = -1;

    for (int v = 0; v < numNodes; v++) {
        if (!visited[v] && dist[v] < min) {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

// task 3 get travel time
static int RoadMapGetTravelTime(RoadMap map, int fromNode, int toNode) {
    struct road *fromRoads = malloc(sizeof(struct road) * MAX_ROADS_PER_NODE);
    if (fromRoads == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    int numFromRoads = RoadMapGetRoadsFrom(map, fromNode, fromRoads);
    for (int i = 0; i < numFromRoads; i++) {
        if (fromRoads[i].toNode == toNode) {
            int tmp = fromRoads[i].travelMinutes;
            free(fromRoads);
            return tmp;
        }
    }
    free(fromRoads);
    return -1;
}

// add minutes to time
struct time TimeAddMinutes(struct time t, int minutes) {
    t.minute += minutes;
    while (t.minute >= 60) {
        t.hour += 1;
        t.minute -= 60;
    }
    // Make sure to handle overflow if hour becomes >= 24
    return t;
}

// compare two times, return -1 if t1 < t2, 0 if t1 == t2, 1 if t1 > t2
static int compareTime(struct time t1, struct time t2) {
    if (t1.hour < t2.hour || (t1.hour == t2.hour && t1.minute < t2.minute)) {
        return -1;
    } else if (t1.hour == t2.hour && t1.minute == t2.minute) {
        return 0;
    } else {
        return 1;
    }
}

// task 4 find min distance
static int minDistance(struct time dist[], bool visited[], int numNodes) {
    // Initialize min value as the maximum possible time
    struct time min = (struct time){.hour = 24, .minute = 0};
    int min_index = -1;

    for (int v = 0; v < numNodes; v++) {
        if (!visited[v] && compareTime(dist[v], min) < 0) {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

// task 4 check if road is closed
static bool RoadMapIsClosed(RoadMap map, struct road road, struct time time) {
    // check if there is a road closure
    if (road.closedFrom.hour == -1) {
        return false;
    }
    // check if road is closed overnight
    if (compareTime(road.closedFrom, road.closedUntil) > 0) {
        road.closedUntil.hour += 24;
    }
    // check if road is closed
    if (compareTime(time, road.closedFrom) > 0 &&
        compareTime(time, road.closedUntil) <= 0) {
        return true;
    }
    return false;
}

// task 4 get minutes from time
static int getMinutes(struct time start, struct time end) {
    int minutes = 0;
    minutes += (end.hour - start.hour) * MINUTES_PER_HOUR;
    minutes += end.minute - start.minute;
    return minutes;
}