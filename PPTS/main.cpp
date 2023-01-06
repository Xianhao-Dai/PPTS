//
//  main.cpp
//  PPTS
//
//  Created by ios on 2022/11/30.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Trajectory.hpp"
#include "Compressor.hpp"
using namespace std;
int main(int argc, const char * argv[]){
    ifstream infile;
    ofstream outfile;
    infile.open("./taxi_log_2008_by_id/1.txt");
    outfile.open("./taxi_log_2008_by_id/1_s.txt");
    string str;
    vector<Trajectory> T;
    while(getline(infile, str)) {
        if (str.empty()) {
            continue;
        }
        short taxiId = 1;
        string date;
        double longitude;
        double latitude;
        int start = 0;
        int end = 0;
        for(;str[end] and str[end] != ',';end++);
        taxiId = stoi(str.substr(start, end - start));
        for(end++, start = end;str[end] and str[end] != ',';end++);
        date = str.substr(start, end - start);
        for(end++, start = end;str[end] and str[end] != ',';end++);
        longitude = stod(str.substr(start, end - start));
        for(end++, start = end;str[end];end++);
        latitude = stod(str.substr(start, end - start));
        Trajectory t(taxiId, date, longitude, latitude);
        T.push_back(t);
    }
    Compressor compressor(T);
    vector<Trajectory> _T;
    _T = compressor.compress(MIN_ERROR_EXACT, 100, 3);
    if(_T.size() == 0){
        outfile << "Storage overflow!" << endl;
    }
    else{
        for(auto i: _T){
            outfile << i.getTaxiId() << "," << i.getDate() << "," << i.getLongitude() << "," << i.getLatitude() << endl;
        }
        outfile << endl << "Closest Euclidean Distance is " << compressor.getClosestEuclideanDistance() << " meters" << endl;
        outfile << "Direction Deviation is " << compressor.getDirectionDeviation() / PI << "π" << endl;
    }
    infile.close();
    outfile.close();
    return 0;
}
