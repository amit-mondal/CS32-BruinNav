//
//  support.cpp
//  Project4
//
//  Created by Amit Mondal on 3/14/17.
//  Copyright © 2017 Amit Mondal. All rights reserved.
//

#include "support.h"

bool operator==(const GeoCoord& lhs, const GeoCoord& rhs) {
    return lhs.latitude == rhs.latitude && lhs.longitude == lhs.longitude;
}

bool operator==(const StreetSegment& lhs, const StreetSegment& rhs) {
    return lhs.segment.start == rhs.segment.start && lhs.segment.end == rhs.segment.end;
}
