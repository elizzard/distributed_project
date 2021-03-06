#include "assert.h"
#include "kdtree.h"
#include "airport.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <functional>
#include <sstream>
#include <map>
#include <vector>
#include <math.h>
#define pi 3.14159265358979323846

using namespace std;

kdtree *kdt;

static int readonce = 0;

struct Airport{
        string code;
        string state;
        string city;
        double latitude;
        double longitude;
};

struct Result{
        Airport airport;
        double distance;
};

map<string,Airport> airportMap;

//left trim
static inline string &ltrim(string &str) {
        str.erase(str.begin(), find_if(str.begin(), str.end(), not1(ptr_fun<int, int>(isspace))));
        return str;
}

//right trim
static inline string &rtrim(string &str) {
        str.erase(find_if(str.rbegin(), str.rend(), not1(ptr_fun<int, int>(isspace))).base(), str.end());
        return str;
}

//trim function
static inline string &trim(string &str) {
        return ltrim(rtrim(str));
}

//converts degree to radians
static double deg2rad(double deg) {
  return (deg * pi / 180);
}

//converts radians to degree
static double rad2deg(double rad) {
  return (rad * 180 / pi);
}

//compare distances and return true if first distance is smaller than second 
static bool compare(Result a, Result b) {
  return (a.distance < b.distance);
}

static char* StrToChar(string str){
  char *ch=new char[str.size()+1];
  ch[str.size()]=0;
  memcpy(ch,str.c_str(),str.size());
  return ch;
}

static string genKey(double latitude, double longitude){
      ostringstream ostr;
      ostr << latitude << ":" << longitude;
      string s = ostr.str();
      return s;
}

static void readAirportLocationsFile(){
  string line;
  ifstream myairportfile("airport-locations.txt");
  kdt = kd_create(2);
  if (myairportfile.is_open())
  {
      getline (myairportfile,line);//skip first line
    while(getline(myairportfile,line) )
    {
        if(trim(line) == ""){
                continue;
        }
      string code = line.substr(1,3);
      string latitude = line.substr(5,7);
      string longitude = line.substr(11,8);
      string citystate = line.substr(19,60);
      citystate = trim(citystate);
	int count = 0;
        string city, state;
        istringstream istr(citystate);
        string token;
        while(getline(istr, token, ',')) {
                if(count == 0){
                 city = token;
                }else if (count == 1){
                 state = token;
                }
                count++;
        }
      latitude = trim(latitude);
      longitude = trim(longitude);
      code = trim(code);
      double lat = atof(latitude.c_str());
      double lon = atof(longitude.c_str());
      Airport airport = {code,state,city,lat,lon};
      string key = genKey(lat,lon);
      assert(0== kd_insert3(kdt, lat, lon,0.0, NULL));
      airportMap[key] = airport;
    }
    myairportfile.close();
  }else{
    cout << "File Error!!!" << endl;
  }
}

static double dist_sq(double lat1, double lon1, double lat2, double lon2)
{
      double theta, dist;
      theta = lon1 - lon2;
      dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(theta));
      dist = acos(dist);
      dist = rad2deg(dist);
      dist = dist * 60 * 1.1515;
      return(dist);
}

int main(int argc,char *argv[]){
{
        static char * result;

        double arr1[3];
        arr1[0] = 40.0; 
        arr1[1] = -120.0;
        arr1[2] = 0.0;

        double arr2[3];
        arr2[2] =0.0;

        if(readonce == 0){
                readAirportLocationsFile();
                readonce = 1;
        }
        
        kdres *results;
        double radius = 0.0;
        int size = 0;
        while(size < 10){
                radius +=0.1;
                results = kd_nearest_range(kdt, arr1, radius);
                size = kd_res_size(results);
        }

        vector<Result> airportResults;
        int count=0;
        while(!kd_res_end(results)){
                double dist;
                kd_res_item(results,arr2);
                dist = dist_sq(arr1[0],arr1[1],arr2[0],arr2[1]);
                string key = genKey(arr2[0],arr2[1]);
                Result res;
                res.distance = dist;
                res.airport = airportMap[key];
                airportResults.push_back(res);
                kd_res_next(results);
                count++;
        }

        sort (airportResults.begin(), airportResults.end(), compare);

        int count1 = 0;

        string str;

        for( vector<Result>::const_iterator i = airportResults.begin(); i != airportResults.end(); ++i){
                if(count1 == 5){
                        break;
                }
                Result a = *i;
                ostringstream out;
                out << "code="<< a.airport.code  << ", name: " << a.airport.city << ", state: " << a.airport.state << ", Distance: " << a.distance << " miles" << endl;
                str = str.append(out.str());
                count1++;
        }
        result = StrToChar(str);
	printf(result);

        return 0;
}
} 
