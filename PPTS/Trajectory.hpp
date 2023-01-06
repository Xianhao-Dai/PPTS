//
//  Trajectory.hpp
//  PPTS
//
//  Created by ios on 2022/11/30.
//

#ifndef Trajectory_hpp
#define Trajectory_hpp

#include <stdio.h>
#include <string>
using namespace std;


/// 轨迹数据：时间、经度、纬度
class Trajectory {
    
    short taxiId;
    //  TODO: interpret date into specified data(e.g. Year-Month-Day Hour-Minute-Second)
    string date;
    /*
     * short year;
     * short month;
     * short day;
     * short hour;
     * short minute;
     * short second;
     */
    double longitude;
    double latitude;
public:
    Trajectory();
    Trajectory(short taxiId, const string date, double longitude, double latitude);
    short getTaxiId();
    string getDate();
    double getLongitude();
    double getLatitude();
};

#endif /* Trajectory_hpp */
