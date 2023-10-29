// BFS maze solver

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "matrix.h"
#include "Maze.h"
#include "Queue.h"

// Check if the cell is in the bound of the maze
// return true if it is in the bound, false otherwise
static bool validCell(Maze m, struct cell c) {
    return c.row >= 0 && c.row < MazeHeight(m) && c.col >= 0 && c.col < MazeWidth(m);
}

// Transverse the surrounding cells of the given cell
static void TransverseSurroundingCells(Maze m, Queue q, bool **visited, struct cell **path, struct cell tmp) {
    struct cell up = {tmp.row - 1, tmp.col};
    struct cell down = {tmp.row + 1, tmp.col};
    struct cell left = {tmp.row, tmp.col - 1};
    struct cell right = {tmp.row, tmp.col + 1};
    if (validCell(m, up) && !MazeIsWall(m, up) && !visited[up.row][up.col]) {
        QueueEnqueue(q, up);
        visited[up.row][up.col] = true;
        path[up.row][up.col] = tmp;
    }
    if (validCell(m, down) && !MazeIsWall(m, down) && !visited[down.row][down.col]) {
        QueueEnqueue(q, down);
        visited[down.row][down.col] = true;
        path[down.row][down.col] = tmp;
    }
    if (validCell(m, left) && !MazeIsWall(m, left) && !visited[left.row][left.col]) {
        QueueEnqueue(q, left);
        visited[left.row][left.col] = true;
        path[left.row][left.col] = tmp;
    }
    if (validCell(m, right) && !MazeIsWall(m, right) && !visited[right.row][right.col]) {
        QueueEnqueue(q, right);
        visited[right.row][right.col] = true;
        path[right.row][right.col] = tmp;
    }
}

// Trace the path from the exit to the start cell
void tracePath(Maze m, struct cell **path, struct cell start, struct cell exit) {
    struct cell tmp = exit;
    while (tmp.row != start.row || tmp.col != start.col) {
        MazeMarkPath(m, tmp);
        tmp = path[tmp.row][tmp.col];
    }
    MazeMarkPath(m, start);
}

bool solve(Maze m) {
    // TODO: Complete this function
    //       Feel free to add helper functions
    
    // Initialise the visited matrix and path matrix
    int height = MazeHeight(m);
    int width = MazeWidth(m);
    bool **visited = createBoolMatrix(height, width);
    struct cell **path = createCellMatrix(height, width);

    // Initialise the queue
    Queue q = QueueNew();

    // Start from the start cell
    struct cell start = MazeGetStart(m);
    QueueEnqueue(q, start);
    visited[start.row][start.col] = true;
    path[start.row][start.col] = start;

    bool found = false;
    while (QueueIsEmpty(q) != true && found == false) {
        struct cell tmp = QueueDequeue(q);
        if (MazeVisit(m, tmp)) {
            found = true;
            tracePath(m, path, start, tmp);

            // Free the memory
            freeBoolMatrix(visited);
            freeCellMatrix(path);
            QueueFree(q);

            return true;
        } else {
            TransverseSurroundingCells(m, q, visited, path, tmp);
        }
    }
    // Free the memory
    freeBoolMatrix(visited);
    freeCellMatrix(path);
    QueueFree(q);

    return false;
}

