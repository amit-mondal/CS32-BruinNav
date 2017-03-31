//#include "provided.h"
//#include <string>
//#include <iostream>
//#include <fstream>
//
//using namespace std;
//
//class MapLoaderImpl
//{
//public:
//	MapLoaderImpl();
//	~MapLoaderImpl();
//	bool load(string mapFile);
//	size_t getNumSegments() const;
//	bool getSegment(size_t segNum, StreetSegment& seg) const;
//private:
//    vector<StreetSegment*> m_segments;
//    vector<Attraction*> m_attractions;
//    static GeoSegment getGeoSegment(string line);
//    static Attraction getAttraction(string line);
//};
//
//MapLoaderImpl::MapLoaderImpl()
//{
//    m_segments = vector<StreetSegment*>();
//}
//
//MapLoaderImpl::~MapLoaderImpl()
//{
//    while (m_segments.size() > 0) {
//        delete m_segments[0];
//        m_segments.erase(m_segments.begin());
//    }
//    while (m_attractions.size() > 0) {
//        delete m_attractions[0];
//        m_attractions.erase(m_attractions.begin());
//    }
//}
//
//
//GeoSegment MapLoaderImpl::getGeoSegment(string line) {
//    string coord_pairs[2];
//    string coords[4];
//    size_t start = line.find(", ");
//    line.replace(start, 2, ",");
//    size_t middle = line.find(" ");
//    coord_pairs[0] = line.substr(0, middle);
//    coord_pairs[1] = line.substr(middle + 1, (line.length() - middle - 1));
//    for (int i = 0;i<2;i++) {
//        middle = coord_pairs[i].find(",");
//        coords[i*2] = coord_pairs[i].substr(0, middle);
//        coords[i*2 + 1] = coord_pairs[i].substr(middle + 1, (coord_pairs[i].length() - middle - 1));
//    }
//    return GeoSegment(GeoCoord(coords[0], coords[1]), GeoCoord(coords[2], coords[3]));
//}
//
//Attraction MapLoaderImpl::getAttraction(string line) {
//    Attraction attraction;
//    size_t divider = line.find("|");
//    attraction.name = line.substr(0, divider);
//    string coord_pair = line.substr(divider + 1, (line.length() - divider - 1));
//    size_t middle = coord_pair.find(", ");
//    string xLoc, yLoc;
//    xLoc = coord_pair.substr(0, middle);
//    yLoc = coord_pair.substr(middle + 2, (coord_pair.length() - middle - 2));
//    attraction.geocoordinates = GeoCoord(xLoc, yLoc);
//    return attraction;
//
//}
//
//bool MapLoaderImpl::load(string mapFile)
//{
//    ifstream infile(mapFile);
//    if (!infile) {
//        cerr << "Error: cannot open " << mapFile << endl;
//        return false;
//    }
//    string line;
//    //loop foreeeeever
//    while (true) {
//        if (!getline(infile, line)) break;
//        StreetSegment* ssegment = new StreetSegment;
//        ssegment->streetName = line;
//        if (!getline(infile, line)) return false;
//        ssegment->segment = MapLoaderImpl::getGeoSegment(line);
//        if (!getline(infile, line)) return false;
//        int num_attractions = stoi(line);
//        Attraction* attraction = nullptr;
//        for (int i = 0;i<num_attractions;i++) {
//            if (!getline(infile, line)) return false;
//            attraction = new Attraction;
//            size_t divider = line.find("|");
//            attraction->name = line.substr(0, divider);
//            //cout << attraction->name << endl;
//            string coord_pair = line.substr(divider + 1, (line.length() - divider - 1));
//            size_t middle = coord_pair.find(", ");
//            string xLoc, yLoc;
//            xLoc = coord_pair.substr(0, middle);
//            yLoc = coord_pair.substr(middle + 2, (coord_pair.length() - middle - 2));
//            attraction->geocoordinates = GeoCoord(xLoc, yLoc);
//            this->m_attractions.push_back(attraction);
//            ssegment->attractions.push_back(*attraction);
//        }
//        this->m_segments.push_back(ssegment);
//    }
//    return this->m_segments.size() > 0;
//}
//
//size_t MapLoaderImpl::getNumSegments() const
//{
//    return m_segments.size();
//}
//
//bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
//{
//    if (segNum < 0 || segNum >= m_segments.size()) return false;
//    seg = *m_segments[segNum];
//    return true;
//}
//
////******************** MapLoader functions ************************************
//
//// These functions simply delegate to MapLoaderImpl's functions.
//// You probably don't want to change any of this code.
//
//MapLoader::MapLoader()
//{
//	m_impl = new MapLoaderImpl;
//}
//
//MapLoader::~MapLoader()
//{
//	delete m_impl;
//}
//
//bool MapLoader::load(string mapFile)
//{
//	return m_impl->load(mapFile);
//}
//
//size_t MapLoader::getNumSegments() const
//{
//	return m_impl->getNumSegments();
//}
//
//bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
//{
//   return m_impl->getSegment(segNum, seg);
//}
//
//
//
//
//

#include "provided.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class MapLoaderImpl
{
public:
    MapLoaderImpl();
    ~MapLoaderImpl();
    bool load(string mapFile);
    size_t getNumSegments() const;
    bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
    vector<StreetSegment*> streetSegments;
    
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
    for (int i = 0 ; i < streetSegments.size() ; i++)
    {
        delete streetSegments[i]; //de-allocate
        streetSegments[i] = nullptr;
    }
}

bool MapLoaderImpl::load(string mapFile)
{
    ifstream infile(mapFile);
    if ( !infile )                    // Did opening the file fail?
    {
        cerr << "Error: Cannot open data.txt!" << endl;
        return false;
    }
    else
    {
        enum readStates {READING_STREET_NAME, READING_COORDINATES, READING_ATTRACTIONS_COUNT, READING_ATTRACTION_NAME};
        
        //opening the file did not fail
        
        string line;
        readStates current_state = readStates::READING_STREET_NAME;
        int attrLeftToRead = 0;
        
        
        while (getline(infile, line ))
        {
            switch (current_state)
            {
                case readStates::READING_STREET_NAME:
                {
                    //we are reading the street name
                    streetSegments.push_back(new StreetSegment);
                    
                    StreetSegment* cur_segment = streetSegments[streetSegments.size() -1];
                    cur_segment->streetName = line;
                    
                    current_state = READING_COORDINATES;
                    
                }break;
                case readStates::READING_COORDINATES:
                {
                    StreetSegment* cur_segment = streetSegments[streetSegments.size() - 1];
                    
                    //we are reading the start and end coordinate information
                    
                    //get start and end coordinates
                    
                    string startLatString;
                    string startLonString;
                    
                    string endLatString;
                    string endLonString;
                    
                    size_t pos = 0;
                    
                    while (line[pos] != ',')
                    {
                        startLatString += line[pos];
                        pos++;
                    }
                    
                    while (line[pos] == ',' || line[pos] == ' ')
                    {
                        pos++;
                    }
                    
                    while (line[pos] != ' ')
                    {
                        startLonString += line[pos];
                        pos++;
                    }
                    
                    while (line[pos] == ',' || line[pos] == ' ')
                    {
                        pos++;
                    }
                    
                    while (line[pos]!= ',')
                    {
                        endLatString += line[pos];
                        pos++;
                    }
                    
                    while (line[pos] == ',' || line[pos] == ' ')
                    {
                        pos++;
                    }
                    
                    while (pos < line.size() && !isspace(line[pos]))
                    {
                        endLonString += line[pos];
                        pos++;
                    }
                    
                    GeoCoord startCoord(startLatString, startLonString); //the GeoCoord object representing the start coordinate
                    GeoCoord endCoord(endLatString, endLonString); //the GeoCoord object representing the end coordinate
                    
                    GeoSegment streetSegment(startCoord, endCoord); //the GeoSegment object for this segment
                    
                    //put the GeoSegment in the StreetSegment
                    
                    cur_segment->segment = streetSegment;
                    
                    current_state = READING_ATTRACTIONS_COUNT;
                    
                }break;
                case readStates::READING_ATTRACTIONS_COUNT:
                {
                    int attractionsCount = stoi(line);
                    
                    if (attractionsCount == 0)
                    {
                        current_state = READING_STREET_NAME;
                    }
                    else
                    {
                        current_state = READING_ATTRACTION_NAME;
                        
                        //attractionsCount > 0
                        
                        attrLeftToRead = attractionsCount;
                    }
                    
                    
                }break;
                case readStates::READING_ATTRACTION_NAME:
                {
                    StreetSegment* cur_segment = streetSegments[streetSegments.size() - 1];
                    attrLeftToRead--;
                    
                    
                    string attractionName;
                    
                    string attractionLat;
                    string attractionLon;
                    
                    size_t pos = 0;
                    
                    while (line[pos] != '|')
                    {
                        attractionName += line[pos];
                        pos++;
                    }
                    
                    
                    while (line[pos] == '|' || line[pos] == ',' || line[pos] == ' ')
                    {
                        pos++;
                    }
                    
                    while (line[pos] != ',')
                    {
                        attractionLat += line[pos];
                        pos++;
                    }
                    
                    while (line[pos] == ',' || line[pos] == ' ')
                    {
                        pos++;
                    }
                    
                    while (pos < line.size() && !isspace(line[pos]))
                    {
                        attractionLon += line[pos];
                        pos++;
                    }
                    
                    GeoCoord attractionCoord(attractionLat, attractionLon);
                    
                    Attraction curAttr;
                    
                    curAttr = {attractionName, attractionCoord};
                    
                    cur_segment->attractions.push_back(curAttr);
                    
                    if (attrLeftToRead == 0)
                    {
                        current_state = readStates::READING_STREET_NAME;
                    } //else leave the system in this state
                    
                }break;
                default:
                    break;
            }
        }
        
        infile.close();
    }
    
    return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
    return streetSegments.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    if (segNum < 0 || segNum >= streetSegments.size())
    {
        return false;
    }
    else
    {
        //segNum is in bounds
        
        seg = *streetSegments[segNum];
        
        return true;
    }
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
    m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
    delete m_impl;
}

bool MapLoader::load(string mapFile)
{
    return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
    return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
    return m_impl->getSegment(segNum, seg);
}
