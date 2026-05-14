#include <chrono>
#include <iostream>
#include <omp.h>
#include <queue>
#include <vector>

using namespace std;

// Sequential DFS
void dfs(vector<int>& visited,
         vector<vector<int>>& graph,
         int start)
{
    visited[start] = 1;

    cout << start << " ";

    for (int neighbour : graph[start])
    {
        if (!visited[neighbour])
        {
            dfs(visited, graph, neighbour);
        }
    }
}

// Parallel DFS
void dfs_parallel(vector<int>& visited,
                  vector<vector<int>>& graph,
                  int start)
{
    #pragma omp critical
    {
        if (visited[start])
            return;

        visited[start] = 1;
        cout << start << " ";
    }

    #pragma omp parallel for
    for (int i = 0; i < graph[start].size(); i++)
    {
        int neighbour = graph[start][i];

        if (!visited[neighbour])
        {
            dfs_parallel(visited, graph, neighbour);
        }
    }
}

// Sequential BFS
void bfs(vector<int>& visited,
         vector<vector<int>>& graph,
         int start)
{
    queue<int> q;

    q.push(start);
    visited[start] = 1;

    while (!q.empty())
    {
        int node = q.front();
        q.pop();

        cout << node << " ";

        for (int neighbour : graph[node])
        {
            if (!visited[neighbour])
            {
                visited[neighbour] = 1;
                q.push(neighbour);
            }
        }
    }
}

// Parallel BFS
void bfs_parallel(vector<int>& visited,
                  vector<vector<int>>& graph,
                  int start)
{
    queue<int> q;

    q.push(start);
    visited[start] = 1;

    while (!q.empty())
    {
        int node;

        #pragma omp critical
        {
            if (!q.empty())
            {
                node = q.front();
                q.pop();
            }
        }

        cout << node << " ";

        #pragma omp parallel for
        for (int i = 0; i < graph[node].size(); i++)
        {
            int neighbour = graph[node][i];

            if (!visited[neighbour])
            {
                #pragma omp critical
                {
                    if (!visited[neighbour])
                    {
                        visited[neighbour] = 1;
                        q.push(neighbour);
                    }
                }
            }
        }
    }
}

int main()
{
    int n;

    cout << "Enter number of vertices: ";
    cin >> n;

    vector<vector<int>> graph(n);

    int edges;

    cout << "Enter number of edges: ";
    cin >> edges;

    cout << "Enter edges:\n";

    for (int i = 0; i < edges; i++)
    {
        int x, y;

        cin >> x >> y;

        graph[x].push_back(y);
        graph[y].push_back(x);
    }

    // Sequential DFS
    vector<int> visited1(n, 0);

    auto start = chrono::high_resolution_clock::now();

    cout << "\nSequential DFS: ";
    dfs(visited1, graph, 0);

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> time = end - start;

    cout << "\nTime: " << time.count() << " seconds\n";

    // Parallel DFS
    vector<int> visited2(n, 0);

    start = chrono::high_resolution_clock::now();

    cout << "\nParallel DFS: ";
    dfs_parallel(visited2, graph, 0);

    end = chrono::high_resolution_clock::now();

    time = end - start;

    cout << "\nTime: " << time.count() << " seconds\n";

    // Sequential BFS
    vector<int> visited3(n, 0);

    start = chrono::high_resolution_clock::now();

    cout << "\nSequential BFS: ";
    bfs(visited3, graph, 0);

    end = chrono::high_resolution_clock::now();

    time = end - start;

    cout << "\nTime: " << time.count() << " seconds\n";

    // Parallel BFS
    vector<int> visited4(n, 0);

    start = chrono::high_resolution_clock::now();

    cout << "\nParallel BFS: ";
    bfs_parallel(visited4, graph, 0);

    end = chrono::high_resolution_clock::now();

    time = end - start;

    cout << "\nTime: " << time.count() << " seconds\n";

    return 0;
}


// g++ -fopenmp your_program.cpp -o program.exe
// .\program.exe
