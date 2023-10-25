
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RoadMap.h"
#include "time.h"

struct roadMap {
	// TODO
};

////////////////////////////////////////////////////////////////////////
// Task 1

RoadMap RoadMapNew(int numNodes) {
	// TODO
	return NULL;
}

void RoadMapFree(RoadMap map) {
	// TODO
}

int RoadMapNumNodes(RoadMap map) {
	// TODO
	return 0;
}

int RoadMapNumRoads(RoadMap map) {
	// TODO
	return 0;
}

bool RoadMapAddRoad(RoadMap map, int node1, int node2,
                    bool isTwoWay, int travelMinutes) {
	// TODO
	return false;
}

int RoadMapGetRoadsFrom(RoadMap map, int node, struct road roads[]) {
	// TODO
	return 0;
}

int RoadMapGetRoadsTo(RoadMap map, int node, struct road roads[]) {
	// TODO
	return 0;
}

////////////////////////////////////////////////////////////////////////
// Task 2

void RoadMapProcessIslands(RoadMap map) {
	// TODO
}

int RoadMapNumIslands(RoadMap map) {
	// TODO
	return 0;
}

bool RoadMapOnSameIsland(RoadMap map, int node1, int node2) {
	// TODO
	return 0;
}

void RoadMapSetIslandName(RoadMap map, int node, char *name) {
	// TODO
}

char *RoadMapGetIslandName(RoadMap map, int node) {
	// TODO
	return "(unnamed)";
}

////////////////////////////////////////////////////////////////////////
// Task 3

void RoadMapAddTrafficLights(RoadMap map, int node) {
	// TODO
}

bool RoadMapHasTrafficLights(RoadMap map, int node) {
	// TODO
	return false;
}

////////////////////////////////////////////////////////////////////////
// Task 4

void RoadMapSetClosedTimes(RoadMap map, int node1, int node2,
                           struct time from, struct time until) {
	// TODO
}

