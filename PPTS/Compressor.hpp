//
//  Compressor.hpp
//  PPTS
//
//  Created by ios on 2022/11/30.
//

#ifndef Compressor_hpp
#define Compressor_hpp

#include <stdio.h>
#include <vector>
#include <stack>
#include <cmath>
#include "Graph.hpp"
#include "Trajectory.hpp"
using namespace std;

#define PI 3.1415926535879932384

enum algorithm {
    MIN_ERROR_EXACT,
    MIN_ERROR_APPROXIMATE
};

/// 轨迹压缩数据结构
class Compressor {
private:
    vector<Trajectory> T;
    double directionDeviation;
    double closestEuclideanDistance;
    double synchronousEuclideanDistance;
public:
    Compressor(vector<Trajectory> &T);
    double getClosestEuclideanDistance();
    double getSynchronousEuclideanDistance();
    double getDirectionDeviation();
    
    /// 按照给定的算法压缩轨迹数据，满足存储以及误差限额
    /// @param al 算法
    /// @param W 存储限额
    /// @param errorLimit 误差限额
    vector<Trajectory> compress(algorithm al, unsigned int W, double errorLimit);
    
    /// 使用Min-Error Approximate算法压缩轨迹数据，满足存储及误差限额
    /// @param w 存储限额
    /// @param errorLimit 误差限额
    vector<Trajectory> minErrorCompressApproximate(unsigned int w, double errorLimit);
    
    /// 使用Min-Error Exact算法压缩轨迹数据，满足存储及误差限额
    /// @param W 存储限额
    /// @param errorLimit 误差限额
    vector<Trajectory> minErrorCompressExact(unsigned int W, double errorLimit);
private:
    
    /// 压缩轨迹数据，找出满足误差限额的最小简化轨迹，主要用于Min-Error Exact压缩算法
    /// @param errorLimit 误差限额
    vector<Trajectory> minSizeCompress(double errorLimit);
    
    
    /// 计算被简化的轨迹点q到其对应边p1p2的欧式距离
    /// @param p1 简化后轨迹点1下标
    /// @param p2 简化后轨迹点2下标
    /// @param q 被简化的轨迹点下标
    double verticalDistance(int p1, int p2, int q);
    
    
    /// 计算两轨迹点之间的直线距离
    /// @param p1 轨迹点下标1
    /// @param p2 轨迹点下标2
    double euclideanDistance(int p1, int p2);
};

#endif /* Compressor_hpp */
