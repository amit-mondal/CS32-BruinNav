// The main.cpp you can use for testing will replace this file soon.

#include "provided.h"
#include "MyMap.h"
#include <iostream>
#include <cassert>
using namespace std;


int main()
{
    /*
    MyMap<int, string> map;
    
    map.associate(35, "this should be thirty-five");
    map.associate(20, "this should be twenty");
    map.associate(35, "this string for thirty-five has been changed");
    
    cout << "35: " << *map.find(35) << endl;
    cout << "20: " << *map.find(20) << endl;
    
    map.clear();
    
    for (int i = 0;i<100;i++) {
        map.associate(i, to_string(i));
    }
    
    for (int i = 0;i<100;i++) {
        cout << *map.find(i) << endl;
    }
    
    for (int i = 0;i<100;i++) {
        map.associate(100-i, to_string(i));
    }
    
    for (int i = 0;i<100;i++) {
        cout << *map.find(i) << endl;
    }
    
    MapLoader ml;
    ml.load("mapdata.txt");
    AttractionMapper am;
    am.init(ml);
    GeoCoord gc;
    am.getGeoCoord("Diddy Riese", gc);
    cout << gc.latitudeText << " " << gc.longitudeText << endl;
    SegmentMapper sm;
    sm.init(ml);
    vector<StreetSegment> segs = sm.getSegments(gc);
    for (auto s: segs)
    {
        cout << "Segment’s street: " << s.streetName << endl;
        cout << "Segment’s start lat/long: " << s.segment.start.latitudeText << ", " <<
        s.segment.start.longitudeText << endl;
        cout << "Segment’s end lat/long: " << s.segment.end.latitudeText << ", " <<
        s.segment.end.longitudeText << endl;
        cout << "This segment has " << s.attractions.size() <<
        " attractions on it." << endl;
    }
     
    */
//    SegmentMapper sm;
//    sm.init(ml);
//    GeoCoord coord = GeoCoord("34.0608001", "-118.4457307");
//    vector<StreetSegment> segs = sm.getSegments(coord);
//    for (int i = 0;i<ml.getNumSegments();i++) {
//        StreetSegment seg;
//        ml.getSegment(i, seg);
//        //cout << "MAIN CURRENT SEGMENT NAME: " << seg.streetName << " " << seg.segment.start.latitudeText << " " << seg.segment.start.longitudeText << endl;
//        for (int k = 0;k<seg.attractions.size();k++) {
//            vector<StreetSegment> fsegs = sm.getSegments(seg.attractions[k].geocoordinates);
//            for (int j = 0;j<fsegs.size();j++) {
//                for (int p = 0;p<fsegs[j].attractions.size();p++) {
//                    cout << fsegs[j].attractions[p].name << endl;
//                }
//            }
//        }
//    }
    
     Navigator nav;
     nav.loadMapData("mapdata.txt");

     vector<NavSegment> directions;
     // NavResult nav_return =
     //    nav.navigate("Diddy Riese", "Theta Delta Chi Fraternity", directions);
     NavResult nav_return =
     nav.navigate("Beverly Hills Plaza Hotel & Spa", "UCLA Guest House", directions);
     //nav.navigate("Harvard-Westlake Middle School", "Greystone Mansion", directions);
     cout << "Navigation returned " << nav_return << endl;
    for (int i = 0;i<directions.size();i++) {
        if (directions[i].m_command == NavSegment::PROCEED) {
            cout << "type: PROCEED" << endl;
            cout << "start: " << directions.at(i).m_geoSegment.start.latitudeText << ", " << directions.at(i).m_geoSegment.start.longitudeText << endl;
            cout << "end: " << directions.at(i).m_geoSegment.end.latitudeText << ", " << directions.at(i).m_geoSegment.end.longitudeText << endl;
            cout << "direction: " << directions.at(i).m_direction << endl;
            cout << "distance: " << directions.at(i).m_distance << endl;
            cout << "street: " << directions.at(i).m_streetName << endl;
        }
        else {
            cout << "type: TURN" << endl;
            cout << "direction: " << directions.at(i).m_direction << endl;
            cout << "street: " << directions.at(i).m_streetName << endl;
        }
        cout << endl;
    }
//     for (int i = 0; i < directions.size(); i++) {
//        string command = directions.at(i).m_command == NavSegment::PROCEED ? "PROCEED" : "TURN";
//         cout << " type: " << command << endl;
//     cout << " direction: " << directions.at(i).m_direction
//     << " street name: " << directions.at(i).m_streetName
//     << " distance: " << directions.at(i).m_distance
//     << " start: " << directions.at(i).m_geoSegment.start.latitudeText
//     << ", " << directions.at(i).m_geoSegment.start.longitudeText
//     << "   end: " << directions.at(i).m_geoSegment.end.latitudeText << ", "
//     << directions.at(i).m_geoSegment.end.longitudeText << endl;
    
     
}

