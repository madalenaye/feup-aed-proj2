//
// Created by Madalena Ye on 27/12/2022.
//


#include "graph.h"

// Constructor: nr nodes and direction (default: undirected)
Graph::Graph(int size) : nodes(size+1){
    this->size = size;
}

Graph::Graph() {
   this->size = 3019;
}

// Add edge from source to destination with a certain weight
void Graph::addEdge(string src, string dest, string airline) {
    //if (src<1 || src>size || dest<1 || dest>size) return;
    nodes[0].adj.push_back({dest,airline,0});
}