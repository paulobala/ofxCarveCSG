#ifndef ofMeshBox_h
#define ofMeshBox_h
#include "ofMesh.h"
#include "ofRectangle.h"

class ofMeshBox : public ofMesh
{
public:
    void addFace(ofVec3f a, ofVec3f b, ofVec3f c);
    void addFace(ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d);
    void addFace(ofRectangle r);
    void addBox(ofRectangle r, float height);
    ofMeshBox &addMesh(ofMesh b);
};
#endif
