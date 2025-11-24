#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

typedef struct {
    int x;
    int y;
} Node;

typedef struct {
    int f;
    int g;
    int x;
    int y;
    int parent_x;
    int parent_y;
} HeapNode;

typedef struct {
    HeapNode* data;
    int size;
    int capacity;
} MinHeap;

MinHeap* create_heap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->data = (HeapNode*)malloc(capacity * sizeof(HeapNode));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void heap_insert(MinHeap* heap, HeapNode element) {
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->data = realloc(heap->data, heap->capacity * sizeof(HeapNode));
    }
    
    int i = heap->size;
    heap->data[i] = element;
    
    while (i > 0 && heap->data[(i-1)/2].f > heap->data[i].f) {
        HeapNode temp = heap->data[(i-1)/2];
        heap->data[(i-1)/2] = heap->data[i];
        heap->data[i] = temp;
        i = (i-1)/2;
    }
    heap->size++;
}

HeapNode heap_extract(MinHeap* heap) {
    HeapNode min = heap->data[0];
    heap->data[0] = heap->data[--heap->size];
    
    int i = 0;
    while (1) {
        int left = 2*i + 1;
        int right = 2*i + 2;
        int smallest = i;
        
        if (left < heap->size && heap->data[left].f < heap->data[smallest].f)
            smallest = left;
        if (right < heap->size && heap->data[right].f < heap->data[smallest].f)
            smallest = right;
            
        if (smallest != i) {
            HeapNode temp = heap->data[i];
            heap->data[i] = heap->data[smallest];
            heap->data[smallest] = temp;
            i = smallest;
        } else {
            break;
        }
    }
    return min;
}

int manhattan_distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int** generate_grid(int rows, int cols, int obstacle_percent, Node* entrance, Node* exit) {
    int** grid = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        grid[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++)
            grid[i][j] = 0;
    }
    entrance->x = 0; entrance->y = 0;
    exit->x = rows-1; exit->y = cols-1;
    grid[0][0] = 2;     
    grid[rows-1][cols-1] = 3;
    int obstacle_count = (rows * cols * obstacle_percent) / 100 - 2;
    for (int k = 0; k < obstacle_count; k++) {
        int i = rand() % rows;
        int j = rand() % cols;
        if (grid[i][j] == 0)
            grid[i][j] = 1;
        else
            k--;
    }
    return grid;
}

Node* a_star(int** grid, int rows, int cols, Node start, Node end, int* path_length) {
    MinHeap* heap = create_heap(100);
    bool** visited = (bool**)malloc(rows * sizeof(bool*));
    Node** parent = (Node**)malloc(rows * sizeof(Node*));
    
    for (int i = 0; i < rows; i++) {
        visited[i] = (bool*)malloc(cols * sizeof(bool));
        parent[i] = (Node*)malloc(cols * sizeof(Node));
        for (int j = 0; j < cols; j++) {
            visited[i][j] = false;
            parent[i][j] = (Node){-1, -1};
        }
    }
    HeapNode initial = {
        .f = manhattan_distance(start.x, start.y, end.x, end.y),
        .g = 0,
        .x = start.x,
        .y = start.y,
        .parent_x = -1,
        .parent_y = -1
    };
    heap_insert(heap, initial);
    
    while (heap->size > 0) {
        HeapNode current = heap_extract(heap);
        if (visited[current.x][current.y]) continue;
        
        visited[current.x][current.y] = true;
        parent[current.x][current.y] = (Node){current.parent_x, current.parent_y};
        if (current.x == end.x && current.y == end.y) {
            Node* path = (Node*)malloc(rows * cols * sizeof(Node));
            Node current_node = end;
            int length = 0;
            
            while (current_node.x != -1) {
                path[length++] = current_node;
                current_node = parent[current_node.x][current_node.y];
            }
            for (int i = 0; i < rows; i++) {
                free(visited[i]);
                free(parent[i]);
            }
            free(visited);
            free(parent);
            free(heap->data);
            free(heap);
            
            *path_length = length;
            return path;
        }
        
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};
        
        for (int i = 0; i < 4; i++) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];
            
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && 
                grid[nx][ny] != 1 && !visited[nx][ny]) {
                
                HeapNode neighbor = {
                    .g = current.g + 1,
                    .x = nx,
                    .y = ny,
                    .parent_x = current.x,
                    .parent_y = current.y
                };
                neighbor.f = neighbor.g + manhattan_distance(nx, ny, end.x, end.y);
                
                heap_insert(heap, neighbor);
            }
        }
    }
    for (int i = 0; i < rows; i++) {
        free(visited[i]);
        free(parent[i]);
    }
    free(visited);
    free(parent);
    free(heap->data);
    free(heap);
    
    return NULL;
}

void print_grid(int** grid, int rows, int cols, Node* path, int path_length) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            bool in_path = false;
            for (int k = 0; k < path_length; k++) {
                if (path[k].x == i && path[k].y == j) {
                    in_path = true;
                    break;
                }
            }
            
            if (grid[i][j] == 2) printf("S ");
            else if (grid[i][j] == 3) printf("E ");
            else if (grid[i][j] == 1) printf("# ");
            else if (in_path) printf("* ");
            else printf(". ");
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));
    
    int rows = 10 + rand() % 11;  // 10-20
    int cols = rows;
    int obstacle_percent = 10 + rand() % 81;  // 10-90%
    
    Node entrance, exit;
    int** grid = generate_grid(rows, cols, obstacle_percent, &entrance, &exit);
    
    int path_length;
    Node* path = a_star(grid, rows, cols, entrance, exit, &path_length);
    
    if (path) {
        printf("Path found! Length: %d\n", path_length-1);
        print_grid(grid, rows, cols, path, path_length);
    } else {
        printf("No path found!\n");
        print_grid(grid, rows, cols, NULL, 0);
    } 
    for (int i = 0; i < rows; i++) free(grid[i]);
    free(grid);
    if (path) free(path);
    
    return 0;
}