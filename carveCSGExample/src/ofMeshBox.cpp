#include <iostream>
#include "ofMeshBox.h"

void ofMeshBox::addFace(ofVec3f a, ofVec3f b, ofVec3f c)
{
    ofVec3f normal = ((c - a).cross(b - a)).normalize();
    
    addNormal(normal);
    addVertex(a);
    addNormal(normal);
    addVertex(b);
    addNormal(normal);
    addVertex(c);
    
    addIndex(getNumIndices());
    addIndex(getNumIndices());
    addIndex(getNumIndices());
}

void ofMeshBox::addFace(ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d)
{
    addFace(a, b, d);
    addFace(b, c, d);
}

void ofMeshBox::addFace(ofRectangle r)
{
    ofVec2f lt(r.x,r.y);
    ofVec2f rt(r.x+r.width,r.y);
    ofVec2f rb(r.x+r.width,r.y+r.height);
    ofVec2f lb(r.x,r.y+r.height);
    addFace(lt,lb,rb,rt);
}

void ofMeshBox::addBox(ofRectangle r, float height)
{
    
    ofVec3f a(r.x,r.y);
    ofVec3f b(r.x,r.y+r.height);
    ofVec3f c(r.x+r.width,r.y+r.height);
    ofVec3f d(r.x+r.width,r.y);
    
    ofVec3f e = a+ofVec3f(0,0,height);
    ofVec3f f = b+ofVec3f(0,0,height);
    ofVec3f g = c+ofVec3f(0,0,height);
    ofVec3f h = d+ofVec3f(0,0,height);
    
    ofMeshBox mesh;
    
    mesh.addFace(a,b,c,d); //top
    mesh.addFace(h,g,f,e); //bottom
    mesh.addFace(b,f,g,c); //front
    mesh.addFace(c,g,h,d); //right
    mesh.addFace(d,h,e,a); //back
    mesh.addFace(a,e,f,b); //left
    
    addMesh(mesh);
}

ofMeshBox &ofMeshBox::addMesh(ofMesh b)
{
    
    int numVertices = getNumVertices();
    int numIndices = getNumIndices();
    
    //add mesh b
    addVertices(b.getVertices());
    addNormals(b.getNormals());
    addIndices(b.getIndices());
    
    //shift indices
    for (int i=0; i<b.getNumIndices(); i++)
    {
        getIndices()[numIndices+i] += numVertices;
    }
    
    return *this;
}