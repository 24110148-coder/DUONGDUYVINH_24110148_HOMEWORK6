#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <queue>

using namespace std;

struct Edge {
    int u, v, weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class Graph {
    map<string, int> nameToId;
    vector<string> idToName;
    vector<Edge> edges;
    vector<vector<pair<int, int>>> adj;
    int nodeCount = 0;

public:
    void readFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            exit(1);
        }

        string uName, vName;
        int w;

        while (file >> uName >> vName >> w) {
            if (w < 0) {
                cerr << "Error: Negative weight detected between " << uName << " and " << vName << endl;
                exit(1);
            }
            int u = getId(uName);
            int v = getId(vName);
            edges.push_back({ u, v, w });
        }

        if (edges.empty()) {
            cerr << "Error: No edges found in the input file\n";
            exit(1);
        }

        adj.resize(nodeCount);
        for (const auto& e : edges) {
            adj[e.u].push_back({ e.v, e.weight });
            adj[e.v].push_back({ e.u, e.weight });
        }
    }

    void primMST() {
        if (nodeCount == 0) {
            cout << "Graph is empty.\n";
            return;
        }
        cout << "\n Prim's MST:\n";
        vector<bool> visited(nodeCount, false);
        vector<int> key(nodeCount, INT_MAX);
        vector<int> parent(nodeCount, -1);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

        key[0] = 0;
        pq.push({0, 0});
        int totalWeight = 0;
        int visitedCount = 0;

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (visited[u]) continue;
            visited[u] = true;
            visitedCount++;

            if (parent[u] != -1) {
                cout << idToName[parent[u]] << " - " << idToName[u] << " : " << key[u] << "\n";
                totalWeight += key[u];
            }

            for (auto& [v, w] : adj[u]) {
                if (!visited[v] && w < key[v]) {
                    parent[v] = u;
                    key[v] = w;
                    pq.push({w, v});
                }
            }
        }

        if (visitedCount != nodeCount) {
            cout << "Warning: Graph is not connected! MST does not cover all vertices.\n";
        } else {
            cout << "Total Weight: " << totalWeight << "\n";
        }
    }
    void kruskalMST() {
        cout << "\n Kruskal's MST:\n";
        sort(edges.begin(), edges.end());
        vector<int> parent(nodeCount);
        vector<int> rank(nodeCount, 0);
        for (int i = 0; i < nodeCount; ++i) parent[i] = i;
        auto find = [&](int u) {
            while (parent[u] != u) {
                parent[u] = parent[parent[u]]; 
                u = parent[u];
            }
            return u;
        };
        auto unite = [&](int u, int v) {
            u = find(u), v = find(v);
            if (u == v) return false;
            if (rank[u] < rank[v]) swap(u, v);
            parent[v] = u;
            if (rank[u] == rank[v]) rank[u]++;
            return true;
        };

        int totalWeight = 0;
        int edgesUsed = 0;
        for (auto& e : edges) {
            if (unite(e.u, e.v)) {
                edgesUsed++;
                totalWeight += e.weight;
                cout << idToName[e.u] << " - " << idToName[e.v] << " : " << e.weight << "\n";
            }
        }

        if (edgesUsed != nodeCount - 1) {
            cout << "Warning: Graph is not connected!\n";
        } else {
            cout << "Total Weight: " << totalWeight << "\n";
        }
    }

private:
    int getId(const string& name) {
        if (nameToId.count(name)) return nameToId[name];
        nameToId[name] = nodeCount++;
        idToName.push_back(name);
        return nameToId[name];
    }
};

int main() {
    Graph g;
    g.readFromFile("graph.txt");
    g.primMST();
    g.kruskalMST();
    return 0;
}