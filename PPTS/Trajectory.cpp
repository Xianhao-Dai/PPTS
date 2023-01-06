//
//  Trajectory.cpp
//  PPTS
//
//  Created by ios on 2022/11/30.
//

#include "Trajectory.hpp"

short Trajectory::getTaxiId() {
    return taxiId;
}

string Trajectory::getDate() {
    return date;
}

double Trajectory::getLongitude() {
    return longitude;
}

double Trajectory::getLatitude() {
    return latitude;
}

Trajectory::Trajectory() {
    
}

Trajectory::Trajectory(short taxiId, string date, double longitude, double latitude) : taxiId(taxiId), date(date), longitude(longitude), latitude(latitude) {
    
}
