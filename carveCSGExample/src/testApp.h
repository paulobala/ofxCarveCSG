#pragma once

#include "ofMain.h"
#include "ofMeshBox.h"
#include "ofxCarveCSG.h"

class testApp : public ofBaseApp{
	public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofMeshBox objectA, objectB;
    //other ofMesh objects could be used; the more complex the mesh, the more time it will take to make the boolean

    ofEasyCam cam;
    ofMesh result; //result of boolean operation
    int count;//current operation
};
