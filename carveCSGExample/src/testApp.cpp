#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    glEnable(GL_DEPTH_TEST);
    objectA.addBox(ofRectangle(0,0,200,200), 200);
    objectB.addBox(ofRectangle(-100,-100,200,200), 200);
    count = 0;//No Boolean operation
}

//--------------------------------------------------------------
void testApp::update()
{
    
}

//--------------------------------------------------------------
void testApp::draw()
{
    
    cam.begin();
    if(count == 0)
    {
        //No Boolean Operation
        ofSetLineWidth(10);
        
        ofSetColor(ofColor::white);
        objectA.draw();
        ofSetColor(ofColor::red);
        objectA.drawWireframe();
        
        ofSetColor(ofColor::white);
        objectB.draw();
        ofSetColor(ofColor::green);
        objectB.drawWireframe();
    }
    else
    {
        //Boolean Operation
        ofSetLineWidth(10);
        ofSetColor(ofColor::black);
        result.drawWireframe();
        ofSetColor(ofColor::white);
        result.draw();
    }
    cam.end();
    
    ofSetColor(ofColor::black);
    switch(count)
    {
        case 0:
            ofDrawBitmapString("No Boolean Operation", 30,30);
            break;
        case 1:
            ofDrawBitmapString("INTERSECTION", 30,30);
            break;
        case 2:
            ofDrawBitmapString("UNION", 30,30);
            break;
        case 3:
            ofDrawBitmapString("A - B", 30,30);
            break;
        case 4:
            ofDrawBitmapString("B - A", 30,30);
            break;
        case 5:
            ofDrawBitmapString("SYMETRIC DIFFERENCE", 30,30);
            break;
        default:
            break;
    }
    ofDrawBitmapString("Press 1 for Intersection; Press 2 for Union; Press 3 for A - B; Press 4 for B - A; Press 5 for Symetric Difference.", 30,ofGetHeight()-30);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    if(key == '1')
    {
        //Intersection
        count = 1;
        ofxCarveCSG booleanOperator;
        result = booleanOperator.meshSetToOfMesh(booleanOperator.booleanOperation(ofxCarveCSG::INTERSECTION, booleanOperator.ofMeshToMeshSet(objectA), booleanOperator.ofMeshToMeshSet(objectB)));
    }
    else if(key == '2')
    {
        //Union
        count = 2;
        ofxCarveCSG booleanOperator;
        result = booleanOperator.meshSetToOfMesh(booleanOperator.booleanOperation(ofxCarveCSG::UNION, booleanOperator.ofMeshToMeshSet(objectA), booleanOperator.ofMeshToMeshSet(objectB)));
    }
    else if(key == '3')
    {
        //A - B
        count = 3;
        ofxCarveCSG booleanOperator;
        result = booleanOperator.meshSetToOfMesh(booleanOperator.booleanOperation(ofxCarveCSG::A_MINUS_B, booleanOperator.ofMeshToMeshSet(objectA), booleanOperator.ofMeshToMeshSet(objectB)));
    }
    else if(key == '4')
    {
        //B - A
        count = 4;
        ofxCarveCSG booleanOperator;
        result = booleanOperator.meshSetToOfMesh(booleanOperator.booleanOperation(ofxCarveCSG::B_MINUS_A, booleanOperator.ofMeshToMeshSet(objectA), booleanOperator.ofMeshToMeshSet(objectB)));
    }
    else if(key == '5')
    {
        //Symetric Difference
        count = 5;
        ofxCarveCSG booleanOperator;
        result = booleanOperator.meshSetToOfMesh(booleanOperator.booleanOperation(ofxCarveCSG::SYMMETRIC_DIFFERENCE, booleanOperator.ofMeshToMeshSet(objectA), booleanOperator.ofMeshToMeshSet(objectB)));
    }
    else
    {
        //No operation
        count = 0;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{
    
}
