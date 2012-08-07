/*
 
 ofxCarveCSG is a wrapper for Carve CSG Library ( http://carve-csg.com/ ), focusing on boolean operations between meshes. The code regarding details of boolean operations is based on a Blender 2.62 file (intern/boolop/intern/BOP_CarveInterface.cpp).
 
 The code in this repository is available under the MIT License.
 
 Copyright (c) 2012- Paulo Bala
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef ofxCarveCSG_h
#define ofxCarveCSG_h
#include <interpolator.hpp>
#include <csg.hpp>
#include <csg_triangulator.hpp>
#include <mesh_simplify.hpp>
#include "ofxCarveCSGTriangle.h"

class ofxCarveCSG
{
public:
    ofxCarveCSG() {};
    ~ofxCarveCSG() {};
    
    //Boolean Operations
    enum OP
    {
        UNION,                  /**< in a or in b. */
        INTERSECTION,           /**< in a and in b. */
        A_MINUS_B,              /**< in a, but not in b. */
        B_MINUS_A,              /**< in b, but not in a. */
        SYMMETRIC_DIFFERENCE,   /**< in a or b, but not both. */
    };
    
    typedef carve::geom::RTreeNode<3, carve::mesh::Face<3> *> face_rtree_t;
    
    carve::mesh::MeshSet<3> * booleanOperation(OP opType, carve::mesh::MeshSet<3> * objA, carve::mesh::MeshSet<3> * objB)
    {
        
        carve::csg::CSG::OP op;
        carve::csg::CSG csg;
        carve::mesh::MeshSet<3> *result = NULL;
        
        carve::mesh::MeshSet<3> *left, *right = NULL;
        carve::geom3d::Vector min, max;
        carve::interpolate::FaceAttr<uint> oface_num;
        uint num_origfaces = 0;
        
        //chose operation
        switch (opType)
        {
            case UNION:
                op = carve::csg::CSG::UNION;
                break;
            case INTERSECTION:
                op = carve::csg::CSG::INTERSECTION;
                break;
            case A_MINUS_B:
                op = carve::csg::CSG::A_MINUS_B;
                break;
            case B_MINUS_A:
                op = carve::csg::CSG::B_MINUS_A;
                break;
            case SYMMETRIC_DIFFERENCE:
                op = carve::csg::CSG::SYMMETRIC_DIFFERENCE;
                break;
            default:
                return NULL;
        }
        
        try
        {
            
            left = Carve_addMesh(objA, oface_num, num_origfaces );
            
            right = Carve_addMesh(objB, oface_num, num_origfaces );
            
            //intersect meshes to prevent wrong tesselations
            Carve_unionIntersections(&left, &right, oface_num);
            
            if(left->meshes.size() == 0 || right->meshes.size()==0)
            {
                delete left;
                delete right;
                
                return NULL;
            }
            
            csg.hooks.registerHook(new carve::csg::CarveTriangulator, carve::csg::CSG::Hooks::PROCESS_OUTPUT_FACE_BIT);
            
            oface_num.installHooks(csg);
            
            //compute the boolean operation
            result = csg.compute(left, right, op, NULL, carve::csg::CSG::CLASSIFY_EDGE);
            
        }
        catch(carve::exception e)
        {
            std::cout << "CSG failed, exception " << e.str() << std::endl;
            return NULL;
        }
        
        return result;
    };
    
    
    std::vector<std::pair<carve::mesh::MeshSet<3> *, ofxCarveCSG::OP> > allOperations(carve::mesh::MeshSet<3> * objA, carve::mesh::MeshSet<3> * objB)
    {
        //use this function in case several boolean operations are needed as it is faster than doing each function separately.
        
        carve::csg::CSG::OP op;
        carve::csg::CSG csg;
        
        carve::mesh::MeshSet<3> * result1 = NULL;
        carve::mesh::MeshSet<3> * result2 = NULL;
        carve::mesh::MeshSet<3> * result3 = NULL;
        carve::mesh::MeshSet<3> * result4 = NULL;
        carve::mesh::MeshSet<3> * result5 = NULL;
        
        std::vector<std::pair<carve::mesh::MeshSet<3> *, ofxCarveCSG::OP> > results ;
        results.clear();
        carve::mesh::MeshSet<3> *left, *right = NULL;
        carve::geom3d::Vector min, max;
        carve::interpolate::FaceAttr<uint> oface_num;
        uint num_origfaces = 0;
        
        try
        {
            
            left = Carve_addMesh(objA, oface_num, num_origfaces);
            
            right = Carve_addMesh(objB, oface_num, num_origfaces);
            
            //intersect meshes to prevent wrong tesselations
            Carve_unionIntersections(&left, &right, oface_num);
            
            if(left->meshes.size() == 0 || right->meshes.size()==0)
            {
                delete left;
                delete right;
                results.clear();
                return results;
            }
            
            csg.hooks.registerHook(new carve::csg::CarveTriangulator, carve::csg::CSG::Hooks::PROCESS_OUTPUT_FACE_BIT);
            
            oface_num.installHooks(csg);
            
            op = carve::csg::CSG::UNION;
            
            result1 = csg.compute(left, right, op, NULL, carve::csg::CSG::CLASSIFY_EDGE);
            
            results.push_back( pair<carve::mesh::MeshSet<3> *, ofxCarveCSG::OP > (result1,ofxCarveCSG::UNION));
            
            op = carve::csg::CSG::INTERSECTION;
            
            result2 = csg.compute(left, right, op, NULL, carve::csg::CSG::CLASSIFY_EDGE);
            
            results.push_back( pair<carve::mesh::MeshSet<3> *, ofxCarveCSG::OP > (result2,ofxCarveCSG::INTERSECTION));
            
            op = carve::csg::CSG::A_MINUS_B;
            
            result3 = csg.compute(left, right, op, NULL, carve::csg::CSG::CLASSIFY_EDGE);
            
            results.push_back( pair<carve::mesh::MeshSet<3> *, ofxCarveCSG::OP > (result3,ofxCarveCSG::A_MINUS_B));
            
            op = carve::csg::CSG::B_MINUS_A;
            
            result4 = csg.compute(left, right, op, NULL, carve::csg::CSG::CLASSIFY_EDGE);
            
            results.push_back( pair<carve::mesh::MeshSet<3> *, ofxCarveCSG::OP > (result4,ofxCarveCSG::B_MINUS_A));
            
            op = carve::csg::CSG::SYMMETRIC_DIFFERENCE;
            
            result5 = csg.compute(left, right, op, NULL, carve::csg::CSG::CLASSIFY_EDGE);
            
            results.push_back( pair<carve::mesh::MeshSet<3> *, ofxCarveCSG::OP > (result5,ofxCarveCSG::SYMMETRIC_DIFFERENCE));
            
        }
        catch(carve::exception e)
        {
            std::cout << "CSG failed, exception " << e.str() << std::endl;
            
            results.clear();
            return results;
        }
        
        return results;
    };
    
    carve::mesh::MeshSet<3> * ofMeshToMeshSet(ofMesh obj) //converts ofMesh to MeshSet needed for Boolean Operations
    {
        
        std::vector< carve::mesh::MeshSet<3>::face_t *> faces = std::vector<carve::mesh::MeshSet<3>::face_t *>();
        
        vector<ofxCarveCSGTriangle> triangles = getTriangles(obj);
        
        for (vector<ofxCarveCSGTriangle>::iterator it=triangles.begin() ; it < triangles.end(); it++ )
        {
            
            std::vector< carve::mesh::MeshSet<3>::vertex_t *> v =  std::vector< carve::mesh::MeshSet<3>::vertex_t *>();
            
            carve::mesh::MeshSet<3>::vertex_t * v1 = new carve::mesh::MeshSet<3>::vertex_t(carve::geom::VECTOR((*it).vert1.x, (*it).vert1.y,(*it).vert1.z));
            carve::mesh::MeshSet<3>::vertex_t * v2 = new carve::mesh::MeshSet<3>::vertex_t(carve::geom::VECTOR((*it).vert2.x, (*it).vert2.y,(*it).vert2.z));
            carve::mesh::MeshSet<3>::vertex_t * v3 = new carve::mesh::MeshSet<3>::vertex_t(carve::geom::VECTOR((*it).vert3.x, (*it).vert3.y,(*it).vert3.z));
            
            v.push_back(v1);
            v.push_back(v2);
            v.push_back(v3);
            
            carve::mesh::MeshSet<3>::face_t * temp = new carve::mesh::MeshSet<3>::face_t::Face(v.begin(), v.end());
            faces.push_back(temp);
            
        }
        
        return new carve::mesh::MeshSet<3>(faces);
        
    }
    
    ofMesh meshSetToOfMesh(carve::mesh::MeshSet<3> * result) //converts MeshSet to OfMesh
    {
        
        ofMesh mesh;
        
        for (carve::mesh::MeshSet<3>::face_iter face_iter = result->faceBegin(); face_iter != result->faceEnd(); ++face_iter)
        {
            carve::mesh::MeshSet<3>::face_t *fa = *face_iter;
            
            std::vector<carve::mesh::MeshSet<3>::vertex_t *> verts;
            fa->getVertices(verts);
            
            
            ofVec3f a = ofVec3f(verts[0]->v.x, verts[0]->v.y, verts[0]->v.z);
            ofVec3f b = ofVec3f( verts[1]->v.x, verts[1]->v.y, verts[1]->v.z);
            ofVec3f c = ofVec3f( verts[2]->v.x, verts[2]->v.y, verts[2]->v.z);
            
            //Add face
            ofVec3f normal = ((c - a).cross(b - a)).normalize();
            
            mesh.addNormal(normal);
            mesh.addVertex(a);
            mesh.addNormal(normal);
            mesh.addVertex(b);
            mesh.addNormal(normal);
            mesh.addVertex(c);
            
            mesh.addIndex( mesh.getNumIndices());
            mesh.addIndex( mesh.getNumIndices());
            mesh.addIndex( mesh.getNumIndices());
        }
        return mesh;
    }
    
private:
    vector<ofxCarveCSGTriangle> getTriangles(ofMesh mesh)
    {
        vector<ofxCarveCSGTriangle> temp = vector<ofxCarveCSGTriangle>();
        
        for(int i = 0; i < mesh.getNumVertices(); i= i+3)
        {
            temp.push_back(ofxCarveCSGTriangle(mesh.getVertices()[i], mesh.getVertices()[i+1], mesh.getVertices()[i+2]));
        }
        
        return temp;
    }
    
    carve::mesh::MeshSet<3> *Carve_unionIntersectingMeshes(carve::mesh::MeshSet<3> *poly,
                                                           carve::mesh::MeshSet<3>::aabb_t &otherAABB,
                                                           carve::interpolate::FaceAttr<uint> &oface_num)
    {
        if(poly->meshes.size()<=1)
            return poly;
        
        carve::csg::CSG csg;
        
        oface_num.installHooks(csg);
        csg.hooks.registerHook(new carve::csg::CarveTriangulator,
                               carve::csg::CSG::Hooks::PROCESS_OUTPUT_FACE_BIT);
        
        std::vector<carve::mesh::MeshSet<3>::mesh_t*> orig_meshes =
        std::vector<carve::mesh::MeshSet<3>::mesh_t*>(poly->meshes.begin(), poly->meshes.end());
        
        carve::mesh::MeshSet<3> *left = Carve_getIntersectedOperand(orig_meshes, otherAABB);
        
        if (!left)
        {
            return poly;
        }
        
        while(orig_meshes.size())
        {
            carve::mesh::MeshSet<3> *right = Carve_getIntersectedOperand(orig_meshes, otherAABB);
            
            if (!right)
            {
                break;
            }
            
            try
            {
                if(left->meshes.size()==0)
                {
                    delete left;
                    
                    left = right;
                }
                else
                {
                    carve::mesh::MeshSet<3> *result = csg.compute(left, right, carve::csg::CSG::UNION, NULL, carve::csg::CSG::CLASSIFY_EDGE);
                    
                    delete left;
                    delete right;
                    
                    left = result;
                }
            }
            catch(carve::exception e)
            {
                std::cerr << "CSG failed, exception " << e.str() << std::endl;
                
                carve::mesh::MeshSet<3> *result = Carve_meshSetFromTwoMeshes(left->meshes, right->meshes);
                
                delete left;
                delete right;
                
                left = result;
            }
            catch(...)
            {
                delete left;
                delete right;
                
                throw "Unknown error in Carve library";
            }
        }
        
        /* append all meshes which doesn't have intersection with another operand as-is */
        if (orig_meshes.size())
        {
            carve::mesh::MeshSet<3> *result = Carve_meshSetFromTwoMeshes(left->meshes, orig_meshes);
            
            delete left;
            
            return result;
        }
        
        return left;
    }
    
    void Carve_unionIntersections(carve::mesh::MeshSet<3> **left_r, carve::mesh::MeshSet<3> **right_r,
                                  carve::interpolate::FaceAttr<uint> &oface_num)
    {
        carve::mesh::MeshSet<3> *left, *right;
        
        carve::mesh::MeshSet<3>::aabb_t leftAABB = (*left_r)->getAABB();
        carve::mesh::MeshSet<3>::aabb_t rightAABB = (*right_r)->getAABB();
        
        left = Carve_unionIntersectingMeshes(*left_r, rightAABB, oface_num);
        right = Carve_unionIntersectingMeshes(*right_r, leftAABB, oface_num);
        
        if(left != *left_r)
            delete *left_r;
        
        if(right != *right_r)
            delete *right_r;
        
        *left_r = left;
        *right_r = right;
    }
    
    void Carve_copyMeshes(std::vector<carve::mesh::MeshSet<3>::mesh_t*> &meshes, std::vector<carve::mesh::MeshSet<3>::mesh_t*> &new_meshes)
    {
        std::vector<carve::mesh::MeshSet<3>::mesh_t*>::iterator it = meshes.begin();
        
        for(; it!=meshes.end(); it++)
        {
            carve::mesh::MeshSet<3>::mesh_t *mesh = *it;
            carve::mesh::MeshSet<3>::mesh_t *new_mesh = new carve::mesh::MeshSet<3>::mesh_t(mesh->faces);
            
            new_meshes.push_back(new_mesh);
        }
    }
    
    carve::mesh::MeshSet<3> *Carve_meshSetFromMeshes(std::vector<carve::mesh::MeshSet<3>::mesh_t*> &meshes)
    {
        std::vector<carve::mesh::MeshSet<3>::mesh_t*> new_meshes;
        
        Carve_copyMeshes(meshes, new_meshes);
        
        return new carve::mesh::MeshSet<3>(new_meshes);
    }
    
    carve::mesh::MeshSet<3> *Carve_meshSetFromTwoMeshes(std::vector<carve::mesh::MeshSet<3>::mesh_t*> &left_meshes,
                                                        std::vector<carve::mesh::MeshSet<3>::mesh_t*> &right_meshes)
    {
        std::vector<carve::mesh::MeshSet<3>::mesh_t*> new_meshes;
        
        Carve_copyMeshes(left_meshes, new_meshes);
        Carve_copyMeshes(right_meshes, new_meshes);
        
        return new carve::mesh::MeshSet<3>(new_meshes);
    }
    
    
    bool Carve_checkMeshSetInterseciton_do(carve::csg::Intersections &intersections,
                                           const carve::geom::RTreeNode<3, carve::mesh::Face<3> *> *a_node,
                                           const carve::geom::RTreeNode<3,  carve::mesh::Face<3> *> *b_node,
                                           bool descend_a = true)
    {
        if(!a_node->bbox.intersects(b_node->bbox))
            return false;
        
        if(a_node->child && (descend_a || !b_node->child))
        {
            for(carve::geom::RTreeNode<3, carve::mesh::Face<3> *> *node = a_node->child; node; node = node->sibling)
            {
                if(Carve_checkMeshSetInterseciton_do(intersections, node, b_node, false))
                    return true;
            }
        }
        else if(b_node->child)
        {
            for(carve::geom::RTreeNode<3, carve::mesh::Face<3> *> *node = b_node->child; node; node = node->sibling)
            {
                if(Carve_checkMeshSetInterseciton_do(intersections, a_node, node, true))
                    return true;
            }
        }
        else
        {
            for(size_t i = 0; i < a_node->data.size(); ++i)
            {
                carve::mesh::MeshSet<3>::face_t *fa = a_node->data[i];
                carve::geom::aabb<3> aabb_a = fa->getAABB();
                if(aabb_a.maxAxisSeparation(b_node->bbox) > carve::EPSILON) continue;
                
                for(size_t j = 0; j < b_node->data.size(); ++j)
                {
                    carve::mesh::MeshSet<3>::face_t *fb = b_node->data[j];
                    carve::geom::aabb<3> aabb_b = fb->getAABB();
                    if(aabb_b.maxAxisSeparation(aabb_a) > carve::EPSILON) continue;
                    
                    std::pair<double, double> a_ra = fa->rangeInDirection(fa->plane.N, fa->edge->vert->v);
                    std::pair<double, double> b_ra = fb->rangeInDirection(fa->plane.N, fa->edge->vert->v);
                    if(carve::rangeSeparation(a_ra, b_ra) > carve::EPSILON) continue;
                    
                    std::pair<double, double> a_rb = fa->rangeInDirection(fb->plane.N, fb->edge->vert->v);
                    std::pair<double, double> b_rb = fb->rangeInDirection(fb->plane.N, fb->edge->vert->v);
                    if(carve::rangeSeparation(a_rb, b_rb) > carve::EPSILON) continue;
                    
                    if(!Carve_facesAreCoplanar(fa, fb))
                    {
                        if(Carve_checkEdgeFaceIntersections(intersections, fa, fb))
                        {
                            return true;
                        }
                    }
                }
            }
        }
        
        return false;
    }
    
    bool Carve_checkMeshSetInterseciton(carve::geom::RTreeNode<3, carve::mesh::Face<3> *> *rtree_a, carve::geom::RTreeNode<3, carve::mesh::Face<3> *> *rtree_b)
    {
        carve::csg::Intersections intersections;
        
        return Carve_checkMeshSetInterseciton_do(intersections, rtree_a, rtree_b);
    }
    
    void Carve_getIntersectedOperandMeshes(std::vector<carve::mesh::MeshSet<3>::mesh_t*> &meshes, carve::mesh::MeshSet<3>::aabb_t &otherAABB,
                                           std::vector<carve::mesh::MeshSet<3>::mesh_t*> &operandMeshes)
    {
        std::vector<carve::mesh::MeshSet<3>::mesh_t*>::iterator it = meshes.begin();
        std::vector< carve::geom::RTreeNode<3, carve::mesh::Face<3> *> *> meshRTree;
        
        while(it != meshes.end())
        {
            carve::mesh::MeshSet<3>::mesh_t *mesh = *it;
            bool isAdded = false;
            
            carve::geom::RTreeNode<3, carve::mesh::Face<3> *> *rtree = carve::geom::RTreeNode<3, carve::mesh::Face<3> *>::construct_STR(mesh->faces.begin(), mesh->faces.end(), 4, 4);
            
            if (rtree->bbox.intersects(otherAABB))
            {
                bool isIntersect = false;
                
                std::vector<carve::mesh::MeshSet<3>::mesh_t*>::iterator operand_it = operandMeshes.begin();
                std::vector<carve::geom::RTreeNode<3, carve::mesh::Face<3> *> *>::iterator tree_it = meshRTree.begin();
                for(; operand_it!=operandMeshes.end(); operand_it++, tree_it++)
                {
                    carve::geom::RTreeNode<3, carve::mesh::Face<3> *> *operandRTree = *tree_it;
                    
                    if(Carve_checkMeshSetInterseciton(rtree, operandRTree))
                    {
                        isIntersect = true;
                        break;
                    }
                }
                
                if(!isIntersect)
                {
                    operandMeshes.push_back(mesh);
                    meshRTree.push_back(rtree);
                    
                    it = meshes.erase(it);
                    isAdded = true;
                }
            }
            
            if (!isAdded)
            {
                delete rtree;
                it++;
            }
        }
        
        std::vector<carve::geom::RTreeNode<3, carve::mesh::Face<3> *> *>::iterator tree_it = meshRTree.begin();
        for(; tree_it != meshRTree.end(); tree_it++)
        {
            delete *tree_it;
        }
        
    }
    
    bool Carve_facesAreCoplanar(const carve::mesh::MeshSet<3>::face_t *a, const carve::mesh::MeshSet<3>::face_t *b)
    {
        carve::geom3d::Ray temp;
        return !carve::geom3d::planeIntersection(a->plane, b->plane, temp);
    }
    
    carve::mesh::MeshSet<3> *Carve_getIntersectedOperand(std::vector<carve::mesh::MeshSet<3>::mesh_t*> &meshes, carve::mesh::MeshSet<3>::aabb_t &otherAABB)
    {
        std::vector<carve::mesh::MeshSet<3>::mesh_t*> operandMeshes;
        Carve_getIntersectedOperandMeshes(meshes, otherAABB, operandMeshes);
        
        if (operandMeshes.size() == 0)
            return NULL;
        
        return Carve_meshSetFromMeshes(operandMeshes);
    }
    
    
    bool Carve_checkEdgeFaceIntersections_do(carve::csg::Intersections &intersections,
                                             carve::mesh::MeshSet<3>::face_t *face_a, carve::mesh::MeshSet<3>::edge_t *edge_b)
    {
        if(intersections.intersects(edge_b, face_a))
            return true;
        
        carve::mesh::MeshSet<3>::vertex_t::vector_t _p;
        if(face_a->simpleLineSegmentIntersection(carve::geom3d::LineSegment(edge_b->v1()->v, edge_b->v2()->v), _p))
            return true;
        
        return false;
    }
    
    bool Carve_checkEdgeFaceIntersections(carve::csg::Intersections &intersections,
                                          carve::mesh::MeshSet<3>::face_t *face_a, carve::mesh::MeshSet<3>::face_t *face_b)
    {
        carve::mesh::MeshSet<3>::edge_t *edge_b;
        
        edge_b = face_b->edge;
        do
        {
            if(Carve_checkEdgeFaceIntersections_do(intersections, face_a, edge_b))
                return true;
            edge_b = edge_b->next;
        }
        while (edge_b != face_b->edge);
        
        return false;
    }
    
    int findVertex(std::vector<carve::geom3d::Vector> vertices, carve::mesh::MeshSet<3>::vertex_t *vert)
    {
        int count = 0;
        for (std::vector<carve::geom3d::Vector>::iterator it = vertices.begin(); it!=vertices.end(); ++it)
        {
            if((*it).x==vert->v.x && (*it).y==vert->v.y && (*it).z==vert->v.z)
            {
                return count;
            }
            count++;
        }
        
    }
    
    bool findVertex2(std::vector<carve::geom3d::Vector> vertices, carve::mesh::MeshSet<3>::vertex_t vert)
    {
        
        for (std::vector<carve::geom3d::Vector>::iterator it = vertices.begin(); it!=vertices.end(); ++it)
        {
            if((*it).x==vert.v.x && (*it).y==vert.v.y && (*it).z==vert.v.z)
            {
                return true;
            }
        }
        return false;
    }
    carve::mesh::MeshSet<3> *Carve_addMesh(carve::mesh::MeshSet<3> *mesh,
                                           carve::interpolate::FaceAttr<uint> &oface_num,
                                           uint &num_origfaces)
    {
        
        std::vector<carve::geom3d::Vector> vertices = std::vector<carve::geom3d::Vector>();
        std::vector<carve::mesh::MeshSet<3>::vertex_t> temp = mesh->vertex_storage;
        
        for (std::vector<carve::mesh::MeshSet<3>::vertex_t>::iterator it = temp.begin(); it!=temp.end(); ++it)
        {
            if(!findVertex2(vertices,(*it)))
            {
                vertices.push_back(carve::geom::VECTOR((*it).v.x,
                                                       (*it).v.y,
                                                       (*it).v.z));
            }
        }
        
        
        //CSG_IFace face;
        std::vector<int> f;
        int numfaces = 0;
        std::vector<int> forig;
        
        int i;
        carve::mesh::MeshSet<3>::face_iter face_iter = mesh->faceBegin();
        for (i = 0; face_iter != mesh->faceEnd(); ++face_iter, ++i)
        {
            carve::mesh::MeshSet<3>::face_t *fa = *face_iter;
            
            std::vector<carve::mesh::MeshSet<3>::vertex_t *> verts;
            fa->getVertices(verts);
            
            f.push_back(3);
            f.push_back(findVertex(vertices,verts[0]));
            f.push_back(findVertex(vertices,verts[1]));
            f.push_back(findVertex(vertices,verts[2]));
            forig.push_back(numfaces);
            ++numfaces;
            ++num_origfaces;
            
        }
        
        carve::mesh::MeshSet<3> *poly = new carve::mesh::MeshSet<3> (vertices, numfaces, f);
        
        uint ii;
        carve::mesh::MeshSet<3>::face_iter face_iter2 = poly->faceBegin();
        for (ii = 0; face_iter2 != poly->faceEnd(); ++face_iter2, ++ii)
        {
            carve::mesh::MeshSet<3>::face_t *face = *face_iter2;
            oface_num.setAttribute(face, forig[ii]);
        }
        
        return poly;
    }
    
};

#endif
