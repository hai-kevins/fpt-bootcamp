#include <stdio.h>
#include <stdlib.h>

// === ONLY CODE WITHIN THE DESIGNATED AREA . DO NOT MODIFY OTHER PARTS ===
int minTurnMazePath(int** maze, int rows, int cols) {
    // maze[r][c] is 0 for road and 1 for wall.
    // Start at (0, 0), finish at (rows - 1, cols - 1).

    if (rows == 1 && cols == 1)
        return 0;

    const int INF = 1000000000;

    int dx[4] = {-1, 1, 0, 0}; // up, down, left, right
    int dy[4] = {0, 0, -1, 1};

    int dist[30][30][4];

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            for (int k = 0; k < 4; k++)
                dist[i][j][k] = INF;

    typedef struct {
        int x;
        int y;
        int dir;
    } Node;

    Node dq[8000];
    int front = 4000;
    int back = 4000;

    // Khởi tạo từ ô (0,0)
    for (int d = 0; d < 4; d++) {
        int nx = dx[d];
        int ny = dy[d];

        if (nx >= 0 && nx < rows &&
            ny >= 0 && ny < cols &&
            maze[nx][ny] == 0) {

            dist[nx][ny][d] = 0;
            dq[back++] = (Node){nx, ny, d};
        }
    }

    while (front < back) {
        Node cur = dq[front++];

        for (int nd = 0; nd < 4; nd++) {
            int nx = cur.x + dx[nd];
            int ny = cur.y + dy[nd];

            if (nx < 0 || nx >= rows || ny < 0 || ny >= cols)
                continue;

            if (maze[nx][ny] == 1)
                continue;

            int cost = (nd == cur.dir) ? 0 : 1;

            if (dist[nx][ny][nd] > dist[cur.x][cur.y][cur.dir] + cost) {
                dist[nx][ny][nd] = dist[cur.x][cur.y][cur.dir] + cost;

                if (cost == 0)
                    dq[--front] = (Node){nx, ny, nd};
                else
                    dq[back++] = (Node){nx, ny, nd};
            }
        }
    }

    int ans = INF;

    for (int d = 0; d < 4; d++)
        if (dist[rows - 1][cols - 1][d] < ans)
            ans = dist[rows - 1][cols - 1][d];

    if (ans == INF)
        return -1;

    return ans;
}
// === DO NOT MODIFY OTHER PARTS ===

int main() {
    int rows, cols;
    if (scanf("%d %d", &rows, &cols) != 2) return 0;
    int** maze = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        maze[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) scanf("%d", &maze[i][j]);
    }
    int result = minTurnMazePath(maze, rows, cols);
    printf("\n__JUDGE_RESULT__\n%d", result);
    for (int i = 0; i < rows; i++) free(maze[i]);
    free(maze);
    return 0;
}