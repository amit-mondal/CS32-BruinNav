
#include "provided.h"
#include "support.h"
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions);
    struct Node {
        GeoCoord coord;
        Node* parent;
        double g;
        double h;
        double f() const { return g + h; }
        string streetName;
    };
private:
    Node* getNode();
    Node* popLeastNode(const vector<Node*> open_list);
    bool hasFoundEnd(const vector<StreetSegment>& segs, string end, StreetSegment& endSeg);
    bool hasLowerCost(const vector<Node*>& open_list, const vector<Node*>& closed_list, const Node* q, const Node* next);
    vector<NavSegment> getNavSegments(string start, string end, const vector<GeoCoord>& coords, const vector<string>& streetNames) const;
    string proceedAngleString(double angle) const;
    string turnAngleString(double angle) const;
    MapLoader m_mloader;
    SegmentMapper m_smapper;
    AttractionMapper m_amapper;
    vector<Node*> m_nodes;
};


NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
    for (int i = 0;i<m_nodes.size();i++) {
        delete m_nodes[i];
    }
}

NavigatorImpl::Node* NavigatorImpl::getNode() {
    Node* node = new Node;
    this->m_nodes.push_back(node);
    return node;
}

NavigatorImpl::Node* NavigatorImpl::popLeastNode(vector<Node*> open_list) {
    double minIndex = 0;
    for (int i = 0;i<open_list.size();i++) {
        if (open_list[i]->f() < open_list[minIndex]->f()) {
            minIndex = i;
        }
    }
    Node* q = open_list[minIndex];
    open_list.erase(open_list.begin() + minIndex);
    return q;
}

bool NavigatorImpl::hasFoundEnd(const vector<StreetSegment>& segs, string end, StreetSegment& endSeg) {
    for (int i = 0;i<segs.size();i++) {
        //check if this segment is the goal
        for (int j = 0;j<segs[i].attractions.size();j++) {
            if (segs[i].attractions[j].name == end) {
                endSeg = segs[i];
                return true;
            }
        }
    }
    return false;
}

bool NavigatorImpl::hasLowerCost(const vector<Node*>& open_list, const vector<Node*>& closed_list, const Node* q, const Node* next) {
    if (next->coord == q->coord) return true;
    for (int j = 0;j<open_list.size();j++) {
        if (open_list[j]->coord == next->coord && open_list[j]->f() <= next->f()) {
            return true;
        }
    }
    for (int j = 0;j<closed_list.size();j++) {
        if (closed_list[j]->coord == next->coord && closed_list[j]->f() <= next->f()) {
            return true;
        }
    }
    return false;
}


bool NavigatorImpl::loadMapData(string mapFile)
{
    if (!this->m_mloader.load(mapFile)) return false;
    this->m_smapper.init(m_mloader);
    this->m_amapper.init(m_mloader);
    return true;
}

vector<NavSegment> NavigatorImpl::getNavSegments(string start, string end, const vector<GeoCoord>& coords, const vector<string>& streetNames) const {
    vector<NavSegment> res;
    string lastStreetName = streetNames[1];
    GeoSegment lastGeoSeg = GeoSegment(coords[0], coords[1]);
    string proceedDir = this->proceedAngleString(angleOfLine(lastGeoSeg));
    double distance = distanceEarthMiles(coords[0], coords[1]);
    res.push_back(NavSegment(proceedDir, lastStreetName, distance, lastGeoSeg));
    for (int i = 2;i<coords.size();i++) {
        GeoCoord prev = coords[i-1];
        GeoCoord curr = coords[i];
        if (prev == curr) continue;
        cout << "prev: " << prev.latitudeText << ", " << prev.longitudeText << endl;
        cout << "curr: " << curr.latitudeText << ", " << curr.longitudeText << endl;
        //if the street names are the same, do a proceed
        if (lastStreetName == streetNames[i]) {
            lastGeoSeg = GeoSegment(prev, curr);
            lastStreetName = streetNames[i];
            proceedDir = this->proceedAngleString(angleOfLine(lastGeoSeg));
            distance = distanceEarthMiles(prev, curr);
            res.push_back(NavSegment(proceedDir, lastStreetName, distance, lastGeoSeg));
        }
        //otherwise do a turn and a proceed
        else {
            GeoSegment newGeoSeg = GeoSegment(prev, curr);
            string turnDir = this->turnAngleString(angleBetween2Lines(lastGeoSeg, newGeoSeg));
            //push a turn
            res.push_back(NavSegment(turnDir, streetNames[i]));
            lastGeoSeg = newGeoSeg;
            lastStreetName = streetNames[i];
            proceedDir = this->proceedAngleString(angleOfLine(lastGeoSeg));
            distance = distanceEarthMiles(prev, curr);
            //push a proceed
            res.push_back(NavSegment(proceedDir, lastStreetName, distance, lastGeoSeg));
        }
    }
    return res;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions)
{
    vector<Node*> open_list;
    vector<Node*> closed_list;
    GeoCoord startCoord, endCoord;
    if (!m_amapper.getGeoCoord(start, startCoord)) return NAV_BAD_SOURCE;
    if (!m_amapper.getGeoCoord(end, endCoord)) return NAV_BAD_DESTINATION;
    
    Node* startNode = getNode();
    startNode->parent = nullptr;
    startNode->coord = startCoord;
    startNode->g = 0;
    startNode->h = distanceEarthKM(startCoord, endCoord);

    open_list.push_back(startNode);
    
    while(!open_list.empty()) {
        //Node* q = this->popLeastNode(open_list);
        double minIndex = 0;
        for (int i = 0;i<open_list.size();i++) {
            if (open_list[i]->f() < open_list[minIndex]->f()) {
                minIndex = i;
            }
        }
        Node* q = open_list[minIndex];
        open_list.erase(open_list.begin() + minIndex);

        //generate successors
        vector<StreetSegment> cSegs = this->m_smapper.getSegments(q->coord);
        //cout << q->streetName << endl;
        cout << q->coord.latitudeText  << "," << q->coord.longitudeText << endl;

        StreetSegment endSeg;
        if (this->hasFoundEnd(cSegs, end, endSeg)) {
            cerr << "FOUND THE END" << endl;
            vector<GeoCoord> result_coord;
            vector<string> streetNames;
            result_coord.push_back(endCoord);
            streetNames.push_back(endSeg.streetName);
            result_coord.push_back(endSeg.segment.start);
            streetNames.push_back(endSeg.streetName);
            Node* parent = q;
            while (parent != nullptr) {
                result_coord.push_back(parent->coord);
                streetNames.push_back(parent->streetName);
                parent = parent->parent;
            }
            reverse(result_coord.begin(), result_coord.end());
            reverse(streetNames.begin(), streetNames.end());
            directions = this->getNavSegments(start, end, result_coord, streetNames);
            for (int i = 0;i<result_coord.size();i++) {
                cout << result_coord[i].latitudeText  << "," << result_coord[i].longitudeText << endl;
            }
            return NAV_SUCCESS;
        }
        
        for (int i = 0;i<cSegs.size();i++) {
            //cout << cSegs[i].streetName << endl;
            //create a successor from the start
            Node* sNext = getNode();
            sNext->coord = cSegs[i].segment.start;
            sNext->parent = q;
            sNext->g = q->g + distanceEarthKM(q->coord, sNext->coord);
            sNext->h = distanceEarthKM(sNext->coord, endCoord);
            sNext->streetName = cSegs[i].streetName;
            
            //check the open and closed lists
            if (!this->hasLowerCost(open_list, closed_list, q, sNext)) open_list.push_back(sNext);
            
            //create a successor from the end
            Node* eNext = getNode();
            eNext->coord = cSegs[i].segment.end;
            eNext->parent = q;
            eNext->g = q->g + distanceEarthKM(q->coord, eNext->coord);
            eNext->h = distanceEarthKM(eNext->coord, endCoord);
            eNext->streetName = cSegs[i].streetName;
            
            //check the open and closed lists
            if (!this->hasLowerCost(open_list, closed_list, q, eNext)) open_list.push_back(eNext);
        }
        closed_list.push_back(q);
    }
    return NAV_NO_ROUTE;
}

string NavigatorImpl::proceedAngleString(double angle) const{
    angle = fmod(angle, 360.0);
    
    if (angle < 0 || angle >= 360) return "***INVALID ANGLE***";
    else if (angle <= 22.5) return "east";
    else if (angle <= 67.5) return "northeast";
    else if (angle <= 112.5) return "north";
    else if (angle <= 157.5) return "northwest";
    else if (angle <= 202.5) return "west";
    else if (angle <= 247.5) return "southwest";
    else if (angle <= 292.5) return "south";
    else if (angle <= 337.5) return "southeast";
    else if (angle < 360) return "east";
     return "***INVALID ANGLE***";
}

string NavigatorImpl::turnAngleString(double angle) const{
    if (angle < 0 || angle >= 360) return "***INVALID ANGLE***";
    if (angle < 180) return "left";
    return "right";
}




//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}



