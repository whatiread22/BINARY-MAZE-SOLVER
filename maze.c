#include <stdio.h>
#include <stdlib.h>

#define N 5  // Maze size
// A utility function to check if a given cell (x, y) is valid and can be visited
int isValidMove(int maze[N][N], int visited[N][N], int x, int y) {
    return (x >= 0 && x < N && y >= 0 && y < N && maze[x][y] == 0 && visited[x][y] == 0);
}

// Utility function to print the maze with the solution path
void printMazeWithSolution(int maze[N][N], int visited[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (visited[i][j] == 1) {
                printf("* ");
            } else {
                printf("%d ", maze[i][j]);
            }
        }
        printf("\n");
    }
}

// ---- DFS Approach ----
// Recursive function to solve the maze using Depth-First Search (DFS)
int solveMazeDFS(int maze[N][N], int visited[N][N], int x, int y) {
    // Base case: destination is reached
    if (x == N-1 && y == N-1) {
        visited[x][y] = 1;  // Mark the destination
        return 1;
    }
    
    // Check if the current cell is valid
    if (isValidMove(maze, visited, x, y)) {
        visited[x][y] = 1;  // Mark current cell as visited

        // Move in all four directions: right, down, left, up
        if (solveMazeDFS(maze, visited, x + 1, y)) return 1;  // Right
        if (solveMazeDFS(maze, visited, x, y + 1)) return 1;  // Down
        if (solveMazeDFS(maze, visited, x - 1, y)) return 1;  // Left
        if (solveMazeDFS(maze, visited, x, y - 1)) return 1;  // Up

        visited[x][y] = 0;  // Backtrack if no path found
    }
    
    return 0;
}

// ---- BFS Approach ----
// Define a structure to represent a point in the maze
typedef struct {
    int x, y;
} Point;

// Define a structure for queue nodes used in BFS
typedef struct Node {
    Point point;
    struct Node* next;
} Node;

// Queue structure
typedef struct {
    Node *front, *rear;
} Queue;

// Initialize the queue
void initializeQueue(Queue* queue) {
    queue->front = queue->rear = NULL;
}

// Enqueue a point into the queue
void enqueue(Queue* queue, Point p) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->point = p;
    temp->next = NULL;
    if (queue->rear == NULL) {
        queue->front = queue->rear = temp;
        return;
    }
    queue->rear->next = temp;
    queue->rear = temp;
}

// Dequeue a point from the queue
Point dequeue(Queue* queue) {
    if (queue->front == NULL) {
        Point p = {-1, -1};  // Empty queue
        return p;
    }
    Node* temp = queue->front;
    queue->front = queue->front->next;
    if (queue->front == NULL) queue->rear = NULL;
    Point p = temp->point;
    free(temp);
    return p;
}

// Check if the queue is empty
int isQueueEmpty(Queue* queue) {
    return queue->front == NULL;
}

// BFS function to solve the maze
void solveMazeBFS(int maze[N][N], int visited[N][N]) {
    Queue queue;
    initializeQueue(&queue);
    
    // Start from the top-left corner
    Point start = {0, 0};
    enqueue(&queue, start);
    visited[0][0] = 1;

    // Directions to move in the maze (right, down, left, up)
    Point directions[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    while (!isQueueEmpty(&queue)) {
        Point current = dequeue(&queue);

        // If destination is reached
        if (current.x == N-1 && current.y == N-1) {
            visited[current.x][current.y] = 1;
            printf("Path found using BFS:\n");
            printMazeWithSolution(maze, visited);
            return;
        }

        // Try all four possible movements from the current point
        for (int i = 0; i < 4; i++) {
            int newX = current.x + directions[i].x;
            int newY = current.y + directions[i].y;

            if (isValidMove(maze, visited, newX, newY)) {
                Point newPoint = {newX, newY};
                enqueue(&queue, newPoint);
                visited[newX][newY] = 1;
            }
        }
    }

    printf("No path found using BFS.\n");
}

// Driver function to test both DFS and BFS solvers
int main() {
    int maze[N][N] = {
        {0, 1, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {1, 1, 0, 1, 0},
        {0, 0, 0, 0, 0}
    };

    // For DFS
    int visitedDFS[N][N] = {0};
    printf("Solving maze using DFS:\n");
    if (solveMazeDFS(maze, visitedDFS, 0, 0)) {
        printMazeWithSolution(maze, visitedDFS);
    } else {
        printf("No path found using DFS.\n");
    }

    // For BFS
    int visitedBFS[N][N] = {0};
    printf("\nSolving maze using BFS:\n");
    solveMazeBFS(maze, visitedBFS);

    return 0;
}