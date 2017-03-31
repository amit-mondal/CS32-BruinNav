//#include "provided.h"
//#include "support.h"
//#include <vector>
//#include <iostream>
//#include "MyMap.h"
//
//using namespace std;
//
//class SegmentMapperImpl
//{
//public:
//	SegmentMapperImpl();
//	~SegmentMapperImpl();
//	void init(const MapLoader& ml);
//	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
//private:
//    void map_assoc(const GeoCoord& coord, const StreetSegment& seg);
//    MyMap< GeoCoord, vector<StreetSegment> > m_map;
//    vector< vector<StreetSegment>* > vectors;
//};
//
//
//SegmentMapperImpl::SegmentMapperImpl()
//{
//}
//
//SegmentMapperImpl::~SegmentMapperImpl()
//{
//    for (int i = 0;i<vectors.size();i++) {
//        delete vectors[i];
//    }
//}
//
//bool operator<(const GeoCoord& lhs, const GeoCoord& rhs) {
//    if (lhs.latitude != rhs.latitude) return lhs.latitude < rhs.latitude;
//    return lhs.longitude < rhs.longitude;
//}
//
//bool operator>(const GeoCoord& lhs, const GeoCoord& rhs) {
//    if (lhs.latitude != rhs.latitude) return lhs.latitude > rhs.latitude;
//    return lhs.longitude > rhs.longitude;
//}
//
//void SegmentMapperImpl::map_assoc(const GeoCoord& coord, const StreetSegment& seg) {
//    vector<StreetSegment>* res = this->m_map.find(coord);
//    if (res == nullptr) {
//        res = new vector<StreetSegment>;
//        res->push_back(seg);
//        this->m_map.associate(coord, *res);
//        this->vectors.push_back(res);
//        return;
//    }
//    res->push_back(seg);
//}
//
//void SegmentMapperImpl::init(const MapLoader& ml)
//{
//    for (int i = 0;i<ml.getNumSegments();i++) {
//        StreetSegment seg;
//        ml.getSegment(i, seg);
//        this->map_assoc(seg.segment.end, seg);
//        this->map_assoc(seg.segment.start, seg);
//        for (int j = 0;j<seg.attractions.size();j++) {
//            this->map_assoc(seg.attractions[j].geocoordinates, seg);
//        }
//    }
//}
//
//vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
//{
//    const vector<StreetSegment>* res = this->m_map.find(gc);
//    vector<StreetSegment> empty;
//    if (res == nullptr) return empty;
//    return *res;
//}
//
////******************** SegmentMapper functions ********************************
//
//// These functions simply delegate to SegmentMapperImpl's functions.
//// You probably don't want to change any of this code.
//
//SegmentMapper::SegmentMapper()
//{
//	m_impl = new SegmentMapperImpl;
//}
//
//SegmentMapper::~SegmentMapper()
//{
//	delete m_impl;
//}
//
//void SegmentMapper::init(const MapLoader& ml)
//{
//	m_impl->init(ml);
//}
//
//vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
//{
//	return m_impl->getSegments(gc);
//}
//
//
//
//
//
//

#include "provided.h"
#include "MyMap.h"
//#include "substituteMyMap.h"
#include <vector>
#include "support.h"
using namespace std;

class SegmentMapperImpl
{
public:
    SegmentMapperImpl();
    ~SegmentMapperImpl();
    void init(const MapLoader& ml);
    vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
    MyMap<GeoCoord, vector<StreetSegment>> segMap;
    void associateCoord(const StreetSegment& ss, const GeoCoord& coord);
    
};

bool operator<(const GeoCoord& lhs, const GeoCoord& rhs) {
    if (lhs.latitude != rhs.latitude) return lhs.latitude < rhs.latitude;
    return lhs.longitude < rhs.longitude;
}

bool operator>(const GeoCoord& lhs, const GeoCoord& rhs) {
    if (lhs.latitude != rhs.latitude) return lhs.latitude > rhs.latitude;
    return lhs.longitude > rhs.longitude;
}

SegmentMapperImpl::SegmentMapperImpl()
{
    
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::associateCoord(const StreetSegment& ss, const GeoCoord& coord)
{
    //passed a coord, this function checks whether the map already has the coord, and associates the street segment appropriately
    
    vector<StreetSegment>* vtPtr = segMap.find(coord);
    
    if (vtPtr == nullptr)
    {
        //if the vector pointer is nullptr, that means the coordinate wasn't found in the MyMap
        
        vector<StreetSegment> sVect;
        
        sVect.push_back(ss);
        
        segMap.associate(coord, sVect);
    }
    else
    {
        //the vector pointer is not nullptr, so the coordinate was found in the MyMap
        
        vtPtr->push_back(ss);
    }
    
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    for (int i = 0 ; i < ml.getNumSegments(); i++)
    {
        StreetSegment ss; //the current StreetSegment being analyzed
        
        ml.getSegment(i, ss);
        
        GeoCoord ss_startingCoord = ss.segment.start;
        GeoCoord ss_endingCoord = ss.segment.end;
        
        associateCoord(ss, ss_startingCoord);
        associateCoord(ss, ss_endingCoord);
        
        for (int j = 0 ; j < ss.attractions.size(); j++)
        {
            associateCoord(ss, ss.attractions[j].geocoordinates);
        }
        
    }
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    
    //    A StreetSegment is associated with a geocoordinate if:
    //        1. The StreetSegment starts at that geocoordinate
    //        2. The StreetSegment ends at that geocoordinate
    //        3. The StreetSegment has an attraction on it with that geocoordinate
    //
    const vector<StreetSegment>* vtPtr = segMap.find(gc);
    
    if (vtPtr != nullptr)
    {
        return *vtPtr;
    }
    else
    {
        vector<StreetSegment> returnVect;
        //vtPtr == nullptr, so we leave gc unchanged...
        return returnVect;
    }
    
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
    m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
    delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
    m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
    return m_impl->getSegments(gc);
}

