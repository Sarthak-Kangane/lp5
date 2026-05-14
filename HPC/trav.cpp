#include <chrono>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

void dfs(vector<int> visited, vector<vector<int> > graph, int start) {
  visited[start] = 1;
  cout << start << " ";
  for (int neighbour : graph[start]) {
    if (visited[neighbour] != 1) {
      dfs(visited, graph, neighbour);
    }
  }
}

void dfs_parallel(vector<int> visited, vector<vector<int> > graph, int start) {
  visited[start] = 1;
  cout << start << " ";
#pragma omp parallel for
  for (int neighbour : graph[start]) {
    if (visited[neighbour] != 1) {
      dfs(visited, graph, neighbour);
    }
  }
}

void bfs(vector<int> visited, vector<vector<int> > graph, int start) {
  queue<int> q;
  q.push(start);
  visited[start] = 1;
  while (!q.empty()) {
    int node = q.front();
    q.pop();
    cout << node << " ";

    for (int neighbour : graph[node]) {
      if (visited[neighbour] != 1) {
        visited[neighbour] = 1;
        q.push(neighbour);
      }
    }
  }
}

void bfs_parallel(vector<int> visited, vector<vector<int> > graph, int start) {
  queue<int> q;
  q.push(start);
  visited[start] = 1;
  while (!q.empty()) {
    int node = q.front();
    q.pop();
    cout << node << " ";

#pragma omp parallel for
    for (int neighbour : graph[node]) {
#pragma omp critical
      if (visited[neighbour] != 1) {
        visited[neighbour] = 1;
        q.push(neighbour);
      }
    }
  }
}

int main() {
  int n;
  cout << "Enter number of vertices: ";
  cin >> n;
  vector<vector<int> > graph(n);

  int edges;
  cout << "Enter number of edges: ";
  cin >> edges;

  cout << "Enter vertices";
  for (int i = 0; i < edges; i++) {
    int x, y;
    cin >> x >> y;
    graph[x].push_back(y);
    graph[y].push_back(x);
  }

  vector<int> visited(n, 0);
  auto start = chrono::high_resolution_clock::now();
  cout << "Sequential DFS";
  dfs(visited, graph, 0);
  auto end = chrono::high_resolution_clock::now();
  chrono::duration<double> time = end - start;
  cout << time.count() << endl;
  cout << endl;

  vector<int> visited1(n, 0);
  start = chrono::high_resolution_clock::now();
  cout << "Parallel DFS";
  dfs_parallel(visited1, graph, 0);
  end = chrono::high_resolution_clock::now();
  time = end - start;
  cout << time.count() << endl;
  cout << endl;

  vector<int> visited2(n, 0);
  start = chrono::high_resolution_clock::now();
  cout << "Sequential BFS";
  bfs(visited2, graph, 0);
  end = chrono::high_resolution_clock::now();
  time = end - start;
  cout << time.count() << endl;
  cout << endl;

  vector<int> visited3(n, 0);
  start = chrono::high_resolution_clock::now();
  cout << "Parallel BFS";
  bfs_parallel(visited3, graph, 0);
  end = chrono::high_resolution_clock::now();
  time = end - start;
  cout << time.count() << endl;
  cout << endl;
}
