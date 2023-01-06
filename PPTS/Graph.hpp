//
//  Graph.hpp
//  PPTS
//
//  Created by ios on 2022/11/30.
//

#ifndef Graph_hpp
#define Graph_hpp

#include <stdio.h>
#include <vector>
using namespace std;

///  邻接边：邻接顶点号、权重、下一条邻接边
struct ArcNode
{
    int vertexNo;
    double weight;
    ArcNode* next;
    ArcNode(int vertexNo, double weight, ArcNode* next) {
        this->vertexNo = vertexNo;
        this->weight = weight;
        this->next = next;
    }
};

///  顶点：顶点号、第一条邻接边
struct VNode
{
    int vertexNo;
    ArcNode* firstEdge;
};

///  图
class Graph {
private:
    int vexNum;
    int arcNum;
    vector<VNode> adjList;
public:
    Graph(int vexNum, int arcNum);
    bool insertEdge(int x, int y, double weight);
    int firstNeighbor(int x);
    int nextNeighbor(int x, int y);
    int getWeight(int x, int y);
};
#endif /* Graph_hpp */
