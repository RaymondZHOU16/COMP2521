
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "navigator.h"
#include "RoadMap.h"
#include "time.h"

#define MINUTES_PER_HOUR 60

static int RoadMapGetTravelTime(RoadMap map, int fromNode, int toNode);
static struct time TimeAddMinutes(struct time time, int minutes);
static struct time NormaliseTime(struct time time);

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
    int *dist = malloc(sizeof(int) * RoadMapNumNodes(map));
    if (dist == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    int *prev = malloc(sizeof(int) * RoadMapNumNodes(map));
    if (prev == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    bool *visited = malloc(sizeof(bool) * RoadMapNumNodes(map));
    if (visited == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < RoadMapNumNodes(map); i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
        visited[i] = false;
    }

    dist[fromNode] = 0;
    prev[fromNode] = fromNode;
    visited[fromNode] = true;

    // find shortest path
    int currNode = fromNode;
    while (currNode != toNode) {
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
        int minDist = INT_MAX;
        for (int i = 0; i < RoadMapNumNodes(map); i++) {
            if (visited[i]) {
                continue;
            }
            if (dist[i] < minDist) {
                minDist = dist[i];
                currNode = i;
            }
        }

        free(fromRoads);
    }

    // store and reverse tha path
	int *path = malloc(sizeof(int) * RoadMapNumNodes(map));
	if (path == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}
	int pathLength = 0;
	currNode = toNode;
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
	route.numLegs = pathLength - 1;
	route.legs = malloc(sizeof(struct leg) * route.numLegs);
	if (route.legs == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}
	struct time currTime = time;
	for (int i = 0; i < route.numLegs; i++) {
		route.legs[i].fromNode = path[i];
		route.legs[i].toNode = path[i + 1];
		route.legs[i].duration = RoadMapGetTravelTime(map, path[i], path[i + 1]);
		// find travel time, and remove traffic light wait time
		if (RoadMapHasTrafficLights(map, path[i]) &&
			path[i] != fromNode) {
			route.legs[i].start = TimeAddMinutes(currTime, TRAFFIC_LIGHT_WAIT);
			currTime = route.legs[i].start;
		} else {
			route.legs[i].start = currTime;
		}
		route.legs[i].end = TimeAddMinutes(currTime, route.legs[i].duration);
		currTime = route.legs[i].end;
	}
	route.start = route.legs[0].start;
	route.end = route.legs[route.numLegs - 1].end;
	route.totalDuration = dist[toNode];

	// // print path
    // printf("path: ");
    // for (int i = 0; i < pathLength - 1; i++) {
    //     printf(" %d ->", path[i]);
    // }
    // printf("%d\n", path[pathLength - 1]);
	
    free(dist);
    free(prev);
    free(visited);
    free(path);
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

	return route;
}

////////////////////////////////////////////////////////////////////////
// Helper functions

// get travel time between two nodes
static int RoadMapGetTravelTime(RoadMap map, int fromNode, int toNode) {
	struct road *roads = malloc(sizeof(struct road) * MAX_ROADS_PER_NODE);
	if (roads == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}
	int numRoads = RoadMapGetRoadsFrom(map, fromNode, roads);
	for (int i = 0; i < numRoads; i++) {
		if (roads[i].toNode == toNode) {
			int tmp = roads[i].travelMinutes;
			free(roads);
			return tmp;
		}
	}
	free(roads);
	return -1;
}

// add minutes to time
static struct time TimeAddMinutes(struct time time, int minutes) {
	struct time newTime = {
		.hour = time.hour,
		.minute = time.minute + minutes,
	};
	newTime = NormaliseTime(newTime);
	return newTime;
}

// normalise time
static struct time NormaliseTime(struct time time) {
	struct time newTime = {
		.hour = time.hour,
		.minute = time.minute,
	};
	while (newTime.minute >= MINUTES_PER_HOUR) {
		newTime.hour++;
		newTime.minute -= MINUTES_PER_HOUR;
	}
	while (newTime.minute < 0) {
		newTime.hour--;
		newTime.minute += MINUTES_PER_HOUR;
	}
	while (newTime.hour >= HOURS_PER_DAY) {
		newTime.hour -= HOURS_PER_DAY;
	}
	while (newTime.hour < 0) {
		newTime.hour += HOURS_PER_DAY;
	}
	return newTime;
}
