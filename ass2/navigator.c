
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "navigator.h"
#include "RoadMap.h"
#include "time.h"

////////////////////////////////////////////////////////////////////////
// Task 3

struct route navigate(RoadMap map, int fromNode, int toNode,
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

