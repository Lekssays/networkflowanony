#include <bits/stdc++.h> 

using namespace std; 

const int N = 450000; 
  
vector<int> graph[N]; 
vector<int> cycles[N]; 
vector<int> vertices(N);

int deficit[N], labeled[N];

int getDegree(int x) {
    return graph[x].size();
}

void addEdge(int u, int v) { 
    graph[u].push_back(v); 
    graph[v].push_back(u); 
} 

void dfs(int u, int p, int color[], int mark[], int par[], int& cyclenumber) { 
    if (color[u] == 2) { 
        return; 
    } 
    if (color[u] == 1) { 
        cyclenumber++; 
        int cur = p; 
        mark[cur] = cyclenumber; 
        while (cur != u) { 
            cur = par[cur]; 
            mark[cur] = cyclenumber; 
        } 
        return; 
    } 
    par[u] = p; 
  
    color[u] = 1; 
  
    for (int v : graph[u]) { 
        if (v == par[u]) { 
            continue; 
        } 
        dfs(v, u, color, mark, par, cyclenumber); 
    } 

    color[u] = 2; 
} 
  
int assignLevels(int V, int x) {      
    bool marked[V]; 
    queue<int> que; 
    int level[V];  
    que.push(x); 
    level[x] = 0; 
    marked[x] = true; 

    while (!que.empty()) { 
        x = que.front(); 
        que.pop(); 
        for (int i = 0; i < graph[x].size(); i++) { 
            int b = graph[x][i]; 
            if (!marked[b]) { 
                que.push(b); 
                level[b] = level[x] + 1; 
                marked[b] = true; 
            } 
        } 
    } 

    int max = -1;
    for (int i = 0; i < V; i++) {
        if(graph[i].size() > 0 || level[i] > INT32_MAX) {
            if(level[i] >= max) {
                max = level[i];
            }
        } else {
            level[i] = -1;
        }

    } 

    unordered_set<int> nodes;
    for(int i = 0; i < V; i++) {
        if(level[i] >= max - 3) {
            nodes.insert(i);
        }
    }

    for(auto n: nodes) {
        vertices.push_back(n);
    }

    return max;
} 

bool firstCheck(int V, int v) {
    vector<int> adj(V);
    for(int i = 0; i < graph[v].size(); i++) {
        adj.push_back(graph[v][i]);
    }
    
    bool hasVertx = false, hasOne = false;
    for(int i = 0; i < adj.size() - 1; i++) {
        if(getDegree(adj[i]) >= 2) {
            hasVertx = true;
        }
        if(getDegree(adj[i]) == 1) {
            hasOne = true;
        }   
    } 
    
    if(graph[v].size() >= 2 && hasOne && hasVertx) {
        labeled[v] = 1;
        return true;
    } else {
        return false;
    }
}

void thirdCheck(int V, int v) {
    vector<int> adjV;
    
    for(int i = 0; i < graph[v].size(); i++) {
        adjV.push_back(graph[v][i]);
    }

    int degreeOne = 0, degreeTwo = 0, y;
    for(int i = 0; i < adjV.size(); i++) {
        int degree = getDegree(adjV[i]);
        if(degree == 1) degreeOne++;
        if(degree == 2){
            y = adjV[i];
            degreeTwo++;
        }
    }

    if(degreeOne > 1 && degreeTwo == 1) {
        labeled[y] = 1;
        deficit[y] = 1;
    }
}

void matchCycles(int edges, int mark[], int& cyclenumber) {
    for (int i = 1; i <= edges; i++) { 
        if (mark[i] != 0) 
            cycles[mark[i]].push_back(i); 
    } 
  
    for (int i = 1; i <= cyclenumber; i++) { 
        for (int x : cycles[i]){
            if(cycles[i].size() == 4) {
                labeled[x] = 1;
                deficit[x] = 1;
            }
        }
    } 
} 

void assignDeficit(int V){
    for(int i = 1; i <= V; i++) {
        if(labeled[i] != 1 && getDegree(i) <= 2) {
            if(firstCheck(V, i)) {
                deficit[i] = 1;
            }
            thirdCheck(V, i);
        }

    }
}

void matchDeficit(int V) {
    vector<int> nodes;
    for(int i = 1; i <= V; i++) {
        if(deficit[i]) nodes.push_back(i);
    }
    
    if(nodes.size() == 0){
        cout << "Graph is already anonymized" << endl;
        return;
    } else if(nodes.size() == 1) {
        cout << nodes[0] << " random node" << endl; // this needs to be random
    } else {
        cout << "Suggested Edges: " << endl;
        for(int i = 0, j = nodes.size() - 1; i <= nodes.size() / 2, j >= nodes.size() / 2; i++, j--) {
            if(i == j) break;
            cout << nodes[i] << " " << nodes[j] << endl;
        }

        if(nodes.size() % 2 != 0) {
            cout << nodes[(nodes.size() / 2) + 1] << " " << nodes[nodes.size() - 1] << endl; 
        }
    }

}

void printDeficit(int V) {
    for(int i = 1; i <= V; i++) {
        cout << deficit[i] << " ";
    }    
    cout << endl;
}

int main() {
    int V, E;

    cin >> V >> E;

    for(int i = 0; i < E; i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);
    }

    int color[N]; 
    int par[N];
    int mark[N]; 
    int cyclenumber = 0;  

    dfs(1, 0, color, mark, par, cyclenumber); 
    matchCycles(E, mark, cyclenumber);

    assignDeficit(V);
    matchDeficit(V);
} 