//
//  Graph.cpp
//  PPTS
//
//  Created by ios on 2022/11/30.
//

#include "Graph.hpp"

Graph::Graph(int vexNum, int arcNum) {
    this->vexNum = vexNum;
    this->arcNum = arcNum;
    this->adjList.clear();
    this->adjList.resize(vexNum);
}

bool Graph::insertEdge(int x, int y, double weight){
    if(this->vexNum < x or this->vexNum < y){
        return false;
    }
    auto rear = new ArcNode(y, weight, NULL);
    auto ptr = this->adjList[x].firstEdge;
    if(ptr == NULL){
        this->adjList[x].firstEdge = rear;
    } else {
        for (; ptr->next; ptr = ptr->next);
        ptr->next = rear;
    }
    rear = new ArcNode(x, weight, NULL);
    ptr = this->adjList[y].firstEdge;
    if(ptr == NULL){
        this->adjList[y].firstEdge = rear;
    } else {
        for (; ptr->next; ptr = ptr->next);
        ptr->next = rear;
    }
    return true;
}

int Graph::firstNeighbor(int x) {
    if(this->vexNum < x or this->adjList[x].firstEdge == NULL){
        return -1;
    }
    return this->adjList[x].firstEdge->vertexNo;
}

int Graph::nextNeighbor(int x, int y) {
    if(this->vexNum < x or this->vexNum < y) {
        return -1;
    }
    auto ptr = this->adjList[x].firstEdge;
    for(;ptr and ptr->vertexNo != y;ptr = ptr->next);
    if(!ptr){
        return -1;
    }
    if(!ptr->next){
        return -1;
    }
    return ptr->next->vertexNo;
}

int Graph::getWeight(int x, int y){
    if(this->vexNum < x or this->vexNum < y) {
        return -1;
    }
    auto ptr = this->adjList[x].firstEdge;
    for(;ptr and ptr->vertexNo != y;ptr = ptr->next);
    if(!ptr){
        return -1;
    }
    return ptr->weight;
}
