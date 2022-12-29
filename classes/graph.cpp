//
// Created by Madalena Ye on 27/12/2022.
//


#include <queue>
#include "graph.h"

// Constructor: nr nodes and direction (default: undirected)
Graph::Graph(int size) : nodes(size+1){
    this->size = size;
}

Graph::Graph() {
   this->size = 3019;
}

// Add edge from source to destination with a certain weight
void Graph::addEdge(int src, int dest, string airline) {
    //if (src<1 || src>size || dest<1 || dest>size) return;
    nodes[src].adj.push_back({dest,airline,0});
}

void Graph::addAirport(int src, Airport airport) {
    nodes[src].airport=airport;
}

void Graph::bfs(int v) {
    for (int i=1; i<=size; i++) nodes[i].visited = false;
    queue<int> q; // queue of unvisited nodes
    q.push(v);
    nodes[v].visited = true;
    while (!q.empty()) { // while there are still unvisited nodes
        int u = q.front(); q.pop();
        // show node order
        //cout << u << " ";
        for (auto e : nodes[u].adj) {
            int w = e.dest;
            if (!nodes[w].visited) {
                q.push(w);
                nodes[w].visited = true;
            }
        }
    }
}

pair<vector<string>,vector<Airport>> Graph::distance(int src, int dest, vector<string> airlines) {
    for (int i=1; i<=size; i++) {
        nodes[i].visited = false;
        nodes[i].dist = 0;
    }
    pair<vector<string>,vector<Airport>> res;
    vector<string> fir;
    queue<int> q; // queue of unvisited nodes
    q.push(src);
    nodes[src].visited = true;
    vector<Airport> init;
    init.push_back(nodes[src].airport);
    nodes[src].visitedAirports = init;

    while (!q.empty()) { // while there are still unvisited nodes
        int u = q.front(); q.pop();

        // show node order
        // cout << u << " ";
        for (auto e : nodes[u].adj) {
            bool tem = false;
            for (auto x : airlines){
                if (x == e.airline){
                    tem = true;
                    break;
                }
            }
            if (!tem) continue;
            int w = e.dest;
            auto why = e.airline;
            if (!nodes[w].visited) {
                q.push(w);
                nodes[w].visited = true;
                nodes[w].dist = nodes[u].dist + 1;
                auto x = nodes[u].visitedAirports;
                x.push_back(nodes[w].airport);
                fir.push_back(e.airline);
                nodes[w].visitedAirports = x;
            }
        }
    }
    res.second = nodes[dest].visitedAirports;
    res.first = fir;
    return res;
}