// Interface to navigation algorithms

// !!! DO NOT MODIFY THIS FILE !!!

#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "RoadMap.h"
#include "time.h"

#define TRAFFIC_LIGHT_WAIT 1

struct route {
    int fromNode;
    int toNode;
    struct time start;
    struct time end;
    int numLegs;
    struct leg *legs;
    int totalDuration; // in minutes
};

struct leg {
    int fromNode;
    int toNode;
    struct time start;
    struct time end;
    int duration; // in minutes
};

/**
 * Finds the fastest route from `fromNode` to `toNode` departing at the
 * given time. If there is no route, the function should return a route
 * struct with numLegs set to 0 and legs set to NULL.
 */
struct route navigate(RoadMap map, int fromNode, int toNode,
                      struct time time);

/**
 * Finds the fastest route from `fromNode` to `toNode` that arrives as
 * soon as possible after `time`, and that departs as late as possible.
 * This function is expected to handle closed roads (see the spec).
 */
struct route advancedNavigate(RoadMap map, int fromNode, int toNode,
                              struct time time);

#endif
