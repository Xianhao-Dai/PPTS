//
//  Compressor.cpp
//  PPTS
//
//  Created by ios on 2022/11/30.
//

#include "Compressor.hpp"

double minError(vector<double> directions, double theta);

void insertInOrder(vector<double>& directions, double theta);

Compressor::Compressor(vector<Trajectory> &T){
    this->T = T;
    this->directionDeviation = 0;
    this->closestEuclideanDistance = 0;
    this->synchronousEuclideanDistance = 0;
}

double Compressor::getClosestEuclideanDistance(){
    return closestEuclideanDistance;
}

double Compressor::getSynchronousEuclideanDistance(){
    return synchronousEuclideanDistance;
}

double Compressor::getDirectionDeviation(){
    return directionDeviation;
}

vector<Trajectory> Compressor::compress(algorithm al, unsigned int W, double errorLimit) {
    vector<Trajectory> res;
    switch (al) {
        case MIN_ERROR_EXACT:
            res = minErrorCompressExact(W, errorLimit);
            break;
        case MIN_ERROR_APPROXIMATE:
            res = minErrorCompressApproximate(W, errorLimit);
            break;
        default:
            break;
    }
    return res;
}

vector<Trajectory> Compressor::minErrorCompressExact(unsigned int W, double errorLimit) {
    //  construction of E
    vector<double> E;
    vector<double> directions;
    for (int i = 0; i < T.size() - 1; i++) {
        for (int j = i + 2; j < T.size(); j++) {
            //  direction angle of PiPj
            double theta = atan2(T[j].getLatitude() - T[i].getLatitude(), T[j].getLongitude() - T[i].getLongitude());
            //  direction angle of Pj-1Pj
            double thetaJ = atan2(T[j].getLatitude() - T[j - 1].getLatitude(),T[j].getLongitude() - T[j - 1].getLongitude());
            insertInOrder(directions, thetaJ);
            double error = fabs(PI - minError(directions, PI + theta));
            insertInOrder(E, error);
        }
        directions.clear();
    }
    //  Min-Size problem
    vector<Trajectory> _T;
    int right = (int)(lower_bound(E.begin(), E.end(), errorLimit) - E.begin());
    /*误差要求很小的情况下直接看原始数据是否符合存储限额要求*/
    if (right == 0) {
        _T = minSizeCompress(errorLimit);
        if (_T.size() > W) {
            _T.resize(0);
        }
        return _T;
    }
    int left = 0;
    while(left <= right){
        int mid = (left + right) / 2;
        _T = minSizeCompress(E[mid]);
        /*最差结果都超出了存储限额*/
        if(_T.size() > W){
            left = mid + 1;
        }
        else if (_T.size() < W){
            right = mid - 1;
        }
        else{
            break;
        }
    }
    if(_T.size() > W){
        if (E[right + 1] < errorLimit){
            _T = minSizeCompress(E[++right]);
        }
        else {
            _T.resize(0);
        }
    }
    directionDeviation = E[right];
    return _T;
}

vector<Trajectory> Compressor::minSizeCompress(double errorLimit) {
    vector<double> directions;
    // construction of G
    Graph* G = new Graph((int)T.size(), 0);
    for (int i = 0; i < T.size() - 1; i++) {
        double _weight = euclideanDistance(i, i + 1);
        double _theta = atan2(T[i + 1].getLatitude() - T[i].getLatitude(), T[i + 1].getLongitude() - T[i].getLongitude());
        insertInOrder(directions, _theta);
        G->insertEdge(i, i+1, _weight);
        for (int j = i + 2; j < T.size(); j++) {
            //  direction angle of PiPj
            double theta = atan2(T[j].getLatitude() - T[i].getLatitude(), T[j].getLongitude() - T[i].getLongitude());
            //  direction angle of Pj-1Pj
            double thetaJ = atan2(T[j].getLatitude() - T[j - 1].getLatitude(),
                                  T[j].getLongitude() - T[j - 1].getLongitude());
            insertInOrder(directions, thetaJ);
            double temp = PI + theta;
            if(temp > PI){
                temp -= 2 * PI;
            }
            assert(minError(directions, temp) >= 0 and minError(directions, temp) <= PI);
            double error = fabs(PI - minError(directions, temp));
            //  insert edge into G
            if (error <= errorLimit) {
                double weight = euclideanDistance(i, j);
                G->insertEdge(i, j, weight);
            }
        }
        directions.clear();
    }
    //  find the shortest path, using Dijkstra algorithm
    vector<bool> visit(T.size(), false);
    vector<unsigned int> dis(T.size(), INT_MAX);
    vector<int> path(T.size());
    visit[0] = true;
    dis[0] = 0;
    path[0] = -1;
    int p = G->firstNeighbor(0);
    while(p != -1){
        dis[p] = G->getWeight(0, p);
        path[p] = 0;
        p = G->nextNeighbor(0,p);
    }
    for (int i = 0; i < T.size(); i++) {
        int min = INT_MAX;
        int k = 0;
        for (int m = 0; m < T.size(); m++) {
            if (!visit[m] and dis[m] < min) {
                min = dis[m];
                k = m;
            }
        }
        visit[k] = true;
        int j = G->firstNeighbor(k);
        while (j != -1) {
            if (!visit[j] and dis[j] > dis[k] + G->getWeight(k, j)) {
                dis[j] = dis[k] + G->getWeight(k, j);
                path[j] = k;
            }
            j = G->nextNeighbor(k, j);
        }
    }
    stack<int> st;
    vector<Trajectory> _T;
    int rear = (int)T.size() - 1;
    while(rear >= 0) {
        st.push(rear);
        rear = path[rear];
    }
    int pre = st.top();
    _T.push_back(T[pre]);
    st.pop();
    int cur;
    double maxDis = 0;
    while(!st.empty()){
        cur = st.top();
        _T.push_back(T[cur]);
        for(int i = pre + 1;i < cur;i++){
            double dis = verticalDistance(pre, cur, i);
            if(dis > maxDis){
                maxDis = dis;
            }
        }
        st.pop();
        pre = cur;
    }
    closestEuclideanDistance = maxDis;
    return _T;
}

vector<Trajectory> Compressor::minErrorCompressApproximate(unsigned int w, double errorLimit) {
    //  construction of S
    vector<double> directions;
    for(int i = 0;i < T.size() - 1;i++){
        //  direction angle of PiPi+1
        double theta = atan2(T[i + 1].getLatitude() - T[i].getLatitude(), T[i + 1].getLongitude() - T[i].getLongitude());
        insertInOrder(directions, theta);
    }
    vector<vector<double>> S(directions.size());
    for(auto& line: S){
        line.resize(directions.size());
    }
    for(int i = 0;i < directions.size();i++){
        S[i][i] = 0;
        for(int j = i + 1;j < directions.size();j++){
            S[i][j] = directions[j] - directions[i];
            S[j][i] = 2 * PI - S[i][j];
        }
    }
    return vector<Trajectory>();
}

double Compressor::verticalDistance(int p1, int p2, int q) {
    pair<double, double> point1(T[p1].getLatitude(), T[p1].getLongitude());
    pair<double, double> point2(T[p2].getLatitude(), T[p2].getLongitude());
    pair<double, double> point(T[q].getLatitude(), T[q].getLongitude());
    double A = point2.second - point1.second;
    double B = point1.first - point2.first;
    double C = point2.first * point1.second - point1.first * point2.second;
    return fabs(A * point.first + B * point.second + C) / sqrt((A * A) + (B * B)) * 100000;
}

double Compressor::euclideanDistance(int p1, int p2) {
    pair<double, double> point1(T[p1].getLatitude(), T[p1].getLongitude());
    pair<double, double> point2(T[p2].getLatitude(), T[p2].getLongitude());
    double horizonDis = fabs(point1.first - point2.first) * 100000;
    double verticalDis = fabs(point1.second - point2.second) * 111320;
    return sqrt((horizonDis * horizonDis) + (verticalDis * verticalDis));
}

double minError(vector<double> directions, double theta) {
    if(directions.empty()) {
        return PI;
    }
    auto pos = lower_bound(directions.begin(), directions.end(), theta);
    if (pos == directions.begin()) {
        return fmin(fabs(*pos - theta), 2 * PI - fabs(*pos - theta));
    }
    auto pre = pos - 1;
    if (pos == directions.end()) {
        return fmin(fabs(*pre - theta), 2 * PI - fabs(*pre - theta));
    }
    return fmin(fmin(fabs(*pos - theta), 2 * PI - fabs(*pos - theta)), fmin(fabs(*pre - theta), 2 * PI - fabs(*pre - theta)));
}

void insertInOrder(vector<double>& directions, double theta) {
    auto pos = lower_bound(directions.begin(), directions.end(), theta);
    if(directions.empty() or *pos != theta){
        directions.insert(pos, theta);
    }
}
