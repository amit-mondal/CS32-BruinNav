//#include "provided.h"
//#include "MyMap.h"
//#include <string>
//using namespace std;
//
//class AttractionMapperImpl
//{
//public:
//	AttractionMapperImpl();
//	~AttractionMapperImpl();
//	void init(const MapLoader& ml);
//	bool getGeoCoord(string attraction, GeoCoord& gc) const;
//private:
//    //associates the name of each attraction with its GeoCoordinate
//    MyMap<string, GeoCoord> m_map;
//};
//
//AttractionMapperImpl::AttractionMapperImpl()
//{
//}
//
//AttractionMapperImpl::~AttractionMapperImpl()
//{
//}
//
//void AttractionMapperImpl::init(const MapLoader& ml)
//{
//    StreetSegment seg;
//    for (int i = 0;i<ml.getNumSegments();i++) {
//        ml.getSegment(i, seg);
//        for (int j = 0;j<seg.attractions.size();j++) this->m_map.associate(seg.attractions[j].name, seg.attractions[j].geocoordinates);
//    }
//}
//
//bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
//{
//    const GeoCoord* res = this->m_map.find(attraction);
//    if (res == nullptr) return false;
//    gc = *res;
//    return true;
//}
//
////******************** AttractionMapper functions *****************************
//
//// These functions simply delegate to AttractionMapperImpl's functions.
//// You probably don't want to change any of this code.
//
//AttractionMapper::AttractionMapper()
//{
//	m_impl = new AttractionMapperImpl;
//}
//
//AttractionMapper::~AttractionMapper()
//{
//	delete m_impl;
//}
//
//void AttractionMapper::init(const MapLoader& ml)
//{
//	m_impl->init(ml);
//}
//
//bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
//{
//	return m_impl->getGeoCoord(attraction, gc);
//}
//
//
//
//

#include "provided.h"
#include <string>
#include "MyMap.h"
//#include "substituteMyMap.h"
using namespace std;

class AttractionMapperImpl
{
public:
    AttractionMapperImpl();
    ~AttractionMapperImpl();
    void init(const MapLoader& ml);
    bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
    MyMap<string, GeoCoord> attrMap;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    for (int i = 0 ; i < ml.getNumSegments(); i++)
    {
        StreetSegment ss;
        ml.getSegment(i, ss);
        
        for (int j = 0 ; j < ss.attractions.size(); j++) // i is which attraction in the attraction vector is being checked
        {
            string attractionName_lowercase;
            
            for (size_t k = 0 ; k < ss.attractions[j].name.size() ; k++) //j is the index of the character in the attraction name
            {
                attractionName_lowercase += tolower(ss.attractions[j].name[k]); //attractions are case insensitive, so we just consider the lowercase version
            }
            
            attrMap.associate(attractionName_lowercase, ss.attractions[j].geocoordinates); //we associate all lowercase attraction names so that the case-insensitive attraction name comparison later works.
        }
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    string attraction_lowercase;
    
    for (size_t i = 0 ; i < attraction.size() ; i++)
    {
        attraction_lowercase += tolower(attraction[i]); //attractions are case insensitive, so we just consider the lowercase version
    }
    
    //the following code gets the GeoCoord pointer that is associated with the given attraction string key
    //if the key is not found in the MyMap, then we do not modify the GeoCoord gc that is passed as a parameter to the getGeoCoord method and return false;
    //else, we modify the GeoCoord gc and return true;
    const GeoCoord* vtPtr = attrMap.find(attraction_lowercase);
    
    if (vtPtr != nullptr)
    {
        gc = *vtPtr; //set gc to the value from the MyMap
        return true;
    }
    else
    {
        //vtPtr == nullptr, so we leave gc unchanged...
        return false;
    }
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
    m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
    delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
    m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
    return m_impl->getGeoCoord(attraction, gc);
}

