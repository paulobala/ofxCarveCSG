#ifndef ofxCarveCSGTriangleTriangle_h
#define ofxCarveCSGTriangle_h

#include "ofVec3f.h"

class ofxCarveCSGTriangle
{
public:
    ofVec3f vert1, vert2, vert3;
    
    ofxCarveCSGTriangle()
    {
    }
    
    ofxCarveCSGTriangle(ofVec3f a, ofVec3f b, ofVec3f c) :
    vert1(a), vert2(b), vert3(c)
    {
    }
};

#endif
