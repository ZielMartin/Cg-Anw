//
// Created by Johannes on 02.06.2018.
//

#include <iostream>
#include <QtWidgets/QInputDialog>
#include "MeshWrapper.h"

MeshWrapper::MeshWrapper() {
    subdivisionLvl = 0;
    limit = false;
}


void MeshWrapper::loadMesh(const char *path) {

    if(path != nullptr) {
        if (!OpenMesh::IO::read_mesh(mesh, path, opt)) {
            std::cerr << "Error loading mesh from file " << path << std::endl;
        }
    }else{
        mesh.clear();
    }

    smoothMesh(false);
}

void MeshWrapper::writeMesh(const char *path){
    if (!OpenMesh::IO::write_mesh(mesh, path))    {
        std::cerr << "Error writing mesh to file " << path << std::endl;
    }
}



//gets vertices in triangle order, if mesh should not be triangulated, an index array must be implemented with triangulated indices
void MeshWrapper::getVerticesAndNormalsTriangulated(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals) {

    mesh.request_face_normals();
    mesh.update_normals();
    mesh.request_vertex_normals();

    HE_MESH meshTriangulated = HE_MESH(mesh);
    //triangulation needed for DRAW_TRIANGLES
    meshTriangulated.triangulate();

    meshTriangulated.request_face_normals();
    meshTriangulated.update_normals();
    meshTriangulated.request_vertex_normals();

    OpenMesh::Vec3f v, n;

    //run throught each face(witch is a triangle)
    for (HE_MESH::FaceIter f_it = meshTriangulated.faces_begin(); f_it != meshTriangulated.faces_end(); ++f_it) {
        //run throught each vertex in this face
        for (HE_MESH::FaceVertexIter fv_it = meshTriangulated.fv_begin(*f_it); fv_it != meshTriangulated.fv_end(*f_it); ++fv_it) {
            v = meshTriangulated.point(*fv_it);
            vertices.push_back(glm::vec3(v[0], v[1], v[2]));
            n = meshTriangulated.normal(*fv_it);
            normals.push_back(glm::vec3(n[0], n[1], n[2]));

        }
    }
}

void MeshWrapper::getVertices(std::vector<glm::vec3> &vertices) {

    for (HE_MESH::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
        OpenMesh::Vec3f v = mesh.point(*v_it);
        vertices.push_back(glm::vec3(v[0], v[1], v[2]));
    }

}

void MeshWrapper::getLimitNormals(std::vector<glm::vec3> &vertices) {

    for (HE_MESH::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
        OpenMesh::Vec3f v = mesh.point(*v_it);
        HE_MESH::Normal limitnormal = this->mesh.property(this->mesh.limitnormal, v_it);//  + mesh.point(*v_it);
        vertices.push_back(glm::vec3(v[0], v[1], v[2]));
        vertices.push_back(glm::vec3(limitnormal[0] * 0.2f +v[0], limitnormal[1]* 0.2f+v[1], limitnormal[2]* 0.2f+v[2] ));
    }

}

void MeshWrapper::getLineVertices(std::vector<glm::vec3> &vertices, std::vector<bool> &selected) {


    for(HE_MESH::EdgeIter e_it = mesh.edges_begin(); e_it != mesh.edges_end(); ++e_it){
        HE_MESH::VertexHandle vh1 = mesh.to_vertex_handle(mesh.halfedge_handle(*e_it, 0));
        HE_MESH::VertexHandle vh2 = mesh.from_vertex_handle(mesh.halfedge_handle(*e_it, 0));

        OpenMesh::Vec3f v1 = mesh.point(vh1);
        OpenMesh::Vec3f v2 = mesh.point(vh2);

        vertices.push_back(glm::vec3(v1[0], v1[1], v1[2]));
        vertices.push_back(glm::vec3(v2[0], v2[1], v2[2]));

        if(checkSelectedHalfEdges(vh1, vh2)){
            selected.push_back(true);
            selected.push_back(true);
        }else{
            selected.push_back(false);
            selected.push_back(false);
        }

    }
}

bool MeshWrapper::checkSelectedHalfEdges(HE_MESH::VertexHandle v1, HE_MESH::VertexHandle v2){
    for(HE_MESH::HalfedgeHandle heh : selectedHalfEdges){
        std::pair<HE_MESH::VertexHandle, HE_MESH::VertexHandle> vertices = getVerticesFromHalfEdge(heh);
        if((vertices.first == v1 && vertices.second == v2) || (vertices.first == v2 && vertices.second == v1)){
            return true;
        }
    }
    return false;
}



void MeshWrapper::moveVertex(HE_MESH::VertexHandle v_h, glm::vec3 relativeMovement) {
    OpenMesh::Vec3f newPoint =
            mesh.point(v_h) + HE_MESH::Point(relativeMovement.x, relativeMovement.y, relativeMovement.z);
    mesh.set_point(v_h, newPoint);

    smoothMesh(false);

}

bool MeshWrapper::selectVertex(glm::vec3 pos, float radius) {
    bool isSelected = false;
    OpenMesh::Vec3f vertexPos;
    vertexPos[0] = pos.x;
    vertexPos[1] = pos.y;
    vertexPos[2] = pos.z;


    for (HE_MESH::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
        OpenMesh::Vec3f meshPoint = mesh.point(*v_it);
        float xDif = meshPoint[0] - vertexPos[0];
        float yDif = meshPoint[1] - vertexPos[1];
        float zDif = meshPoint[2] - vertexPos[2];


        if ((xDif < radius && xDif > -radius) && (yDif < radius && yDif > -radius) &&
            (zDif < radius && zDif > -radius)) {

            if (std::find(selectedVertices.begin(), selectedVertices.end(), v_it) != selectedVertices.end()) {
                //deselect
                selectedVertices.erase(std::find(selectedVertices.begin(), selectedVertices.end(), v_it));
                //this->setVertexWeight(*v_it, 1.0);
            } else {
                //select
                selectedVertices.push_back(*v_it);
                //this->setVertexWeight(*v_it, 2.0);
                isSelected = true;
            }
        }

    }

    selectedHalfEdges.clear();
    if(selectedVertices.size() >= 2){
        for(int i = 0; i < selectedVertices.size()-1; i++){
            selectHalfEdge(selectedVertices.at(i), selectedVertices.at(i + 1));
        }
    }
    return isSelected;
}

void MeshWrapper::setVertexWeightAllSelected(float weight) {
    for(HE_MESH::VertexHandle vh : selectedVertices){
        this->setVertexWeight(vh, weight);
    }

}

void MeshWrapper::selectHalfEdge(HE_MESH::VertexHandle v1, HE_MESH::VertexHandle v2){

    for(HE_MESH::VertexOHalfedgeIter voh_it = mesh.voh_iter(v1); voh_it.is_valid(); ++voh_it) {
        // Iterate over all outgoing halfedges...

        HE_MESH::VertexHandle vh = mesh.to_vertex_handle(*voh_it);

        if(vh == v2){
            selectedHalfEdges.push_back(*voh_it);
        }
    }
}

void MeshWrapper::clearSelectedEdges(){
    selectedHalfEdges.clear();
}

std::pair<HE_MESH::VertexHandle, HE_MESH::VertexHandle> MeshWrapper::getVerticesFromHalfEdge(HE_MESH::HalfedgeHandle heh){
    std::pair<HE_MESH::VertexHandle, HE_MESH::VertexHandle> vertices;
    vertices.first =  mesh.from_vertex_handle(heh);
    vertices.second =  mesh.to_vertex_handle(heh);
    return  vertices;
}



void MeshWrapper::deselectAll() {
    selectedVertices.clear();
    selectedHalfEdges.clear();
}


std::vector<glm::vec3> MeshWrapper::getSelectedVertices() {
    std::vector<glm::vec3> selected;

    for (HE_MESH::VertexHandle v_h : selectedVertices) {
        selected.push_back(glm::vec3(mesh.point(v_h)[0], mesh.point(v_h)[1], mesh.point(v_h)[2]));
    }
    return selected;
}

void MeshWrapper::moveSelectedVertices(glm::vec3 relativeMovement) {
    for (HE_MESH::VertexHandle v_h : selectedVertices) {
        moveVertex(v_h, relativeMovement);
    }
    smoothMesh(false);

}

void MeshWrapper::deleteSelectedVertices() {
    mesh.request_face_status();
    mesh.request_edge_status();
    mesh.request_vertex_status();


    for (HE_MESH::VertexHandle v_h : selectedVertices) {
        mesh.delete_vertex(v_h, true);
    }
    mesh.garbage_collection();
    deselectAll();
    smoothMesh(false);

}

void MeshWrapper::addVertex(glm::vec3 vertex) {
    OpenMesh::Vec3f const newPoint = HE_MESH::Point(vertex.x, vertex.y, vertex.z);
    mesh.new_vertex(newPoint);
}

void MeshWrapper::makeSelectedFace() {
    if (selectedVertices.size() == 3) {
        mesh.add_face(selectedVertices);
    }
    smoothMesh(false);
}


void MeshWrapper::subdivision() {
    if(subdivisionLvl == 0) {
        backstack.push_back(mesh);
    }
    HE_MESH newMesh = catmull(backstack.back(), 1);
    subdivisionLvl++;
    backstack.push_back(newMesh);

    HE_MESH::VertexIter v_itr = newMesh.vertices_begin();
    HE_MESH::VertexIter v_end = newMesh.vertices_end();

    for (; v_itr != v_end; ++v_itr) {
        catmull.calcLimitNormal(newMesh, *v_itr);
       // newMesh.property(newMesh.limitnormal, *v_itr) = HE_MESH::Normal(0.0f, 0.0f, 0.0f);
       // std::cout << newMesh.property(newMesh.limitnormal, *v_itr) << std::endl;
    }

    v_itr = newMesh.vertices_begin();
    for (; v_itr != v_end; ++v_itr) {
        HE_MESH::Point VP(0.0, 0.0, 0.0);
        VP = newMesh.point(*v_itr);
        std::vector<HE_MESH::VertexHandle> vertices;

        HE_MESH::HalfedgeHandle start = newMesh.halfedge_handle(*v_itr);
        HE_MESH::HalfedgeHandle he = start;

        do
        {
            he = newMesh.opposite_halfedge_handle(he);
            vertices.push_back(newMesh.from_vertex_handle(he));
            he = newMesh.next_halfedge_handle(he);
        } while (he != start);

        //std::cout << vertices.size() << std::endl;

        HE_MESH::Point LP(0.0, 0.0, 0.0);
        std::vector<HE_MESH::HalfedgeHandle> halfEdges;

        /* HE_MESH::VertexOHalfedgeIter voh_it = _m.voh_iter(*v_itr);
         for(++voh_it; voh_it != newMesh.voh_iter(*v_itr) ; ++voh_it) {
                 // Iterate over all outgoing halfedges...
                 halfEdges.push_back(*voh_it);
         }*/


        start = newMesh.halfedge_handle(*v_itr);
        he = start;

        do
        {
            halfEdges.push_back(he);

            he = newMesh.opposite_halfedge_handle(he);
            he = newMesh.next_halfedge_handle(he);
        } while (he != start);


        int k = (int) vertices.size();

        // weights
        float alpha = 1.0f - 5.0f / (k + 5.0f);
        float beta  = 4.0f / ((k + 5.0f) * k);
        float gamma = 1.0f / ((k + 5.0f) * k);

        // calculate new point
        for (HE_MESH::HalfedgeHandle halfEdge : halfEdges)
        {
            // next half edge
            HE_MESH::HalfedgeHandle nHE = newMesh.next_halfedge_handle(halfEdge);

            // next next half edge
            HE_MESH::HalfedgeHandle nnHE = newMesh.next_halfedge_handle(nHE);

            // beta
            LP += newMesh.point(newMesh.from_vertex_handle(nHE)) * beta;
            //std::cout << newMesh.point(newMesh.from_vertex_handle(nHE)) << std::endl;

            // gamma
            LP += newMesh.point(newMesh.from_vertex_handle(nnHE)) * gamma;
        }

        // alpha
        LP += VP * alpha;
        newMesh.set_point(*v_itr, LP);
        newMesh.property(newMesh.limitpoint, v_itr) = LP;
       // HE_MESH::Point g = newMesh.property(newMesh.limitpoint, vertex);
       // std::cout << g << std::endl;
    }


    backstackLimit.push_back(newMesh);
    /*if(subdivisionLvl > 0) {
        backstackH
    }*/

    this->mesh = limit ? backstackLimit.back() : backstack.back();

    smoothMesh(false);
}

void MeshWrapper::undo() {
    /*if(backstack.size() > 0) {
        mesh = backstack.at(backstack.size() - 1);
        backstack.pop_back();
    }*/
    if(subdivisionLvl == 1) {
        backstack.pop_back();
        backstackLimit.pop_back();
        mesh = backstack.back();
        backstack.pop_back();
        subdivisionLvl--;
    }
    else if(subdivisionLvl > 1) {
        backstack.pop_back();
        backstackLimit.pop_back();
        mesh = backstack.back();
        subdivisionLvl--;
    }

    smoothMesh(false);
}

void MeshWrapper::setVertexWeight(HE_MESH::VertexHandle vertexHandle, float weight) {
    this->mesh.property(this->mesh.vp_fourth, vertexHandle) = weight;
}

float MeshWrapper::getVertexWeight(HE_MESH::VertexHandle vertexHandle) {
    return this->mesh.property(this->mesh.vp_fourth, vertexHandle);
}

void MeshWrapper::getDimensions(glm::vec3 &min, glm::vec3 &max){
    for (HE_MESH::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
        if(mesh.point(*v_it)[0] < min.x) min.x =  mesh.point(*v_it)[0];
        if(mesh.point(*v_it)[1] < min.y) min.y =  mesh.point(*v_it)[1];
        if(mesh.point(*v_it)[2] < min.z) min.z =  mesh.point(*v_it)[2];

        if(mesh.point(*v_it)[0] > max.x) max.x =  mesh.point(*v_it)[0];
        if(mesh.point(*v_it)[1] > max.y) max.y =  mesh.point(*v_it)[1];
        if(mesh.point(*v_it)[2] > max.z) max.z =  mesh.point(*v_it)[2];
    }
}

void MeshWrapper::smoothMesh(bool pushToBackstack) {
    if(pushToBackstack){
        //backstack.push_back(mesh);
    }

    smoothedVertices.clear();
    unSmoothedVertices.clear();

    for (HE_MESH::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
        HE_MESH::Point p(0,0,0);
        int c = 0;
        for(HE_MESH::VertexOHalfedgeIter voh_it = mesh.voh_iter(v_it); voh_it; ++voh_it) {
            // Iterate over all outgoing halfedges..
            p[0] += mesh.point(mesh.to_vertex_handle(*voh_it))[0];
            p[1] += mesh.point(mesh.to_vertex_handle(*voh_it))[1];
            p[2] += mesh.point(mesh.to_vertex_handle(*voh_it))[2];
            c++;
        }
        if(c != 0){
            p[0] = p[0] / c;
            p[1] = p[1] / c;
            p[2] = p[2] / c;
        }else{
            p = mesh.point(*v_it);
        }
        smoothedVertices.push_back(p);
        unSmoothedVertices.push_back(mesh.point(*v_it));
    }
}

void MeshWrapper::applySmoothedVertices(int interpolationValue){
    int i = 0;
    for (OpenMesh::PolyConnectivity::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it){
        //vector from old to new vertex
        glm::vec3 vec = glm::vec3(smoothedVertices.at(i)[0] - unSmoothedVertices.at(i)[0], smoothedVertices.at(i)[1] - unSmoothedVertices.at(i)[1], smoothedVertices.at(i)[2] - unSmoothedVertices.at(i)[2]);
        //float lenght = (vec.length()/99)*interpolationValue;
        mesh.point(*v_it)[0] = unSmoothedVertices.at(i)[0] + (vec.x/99)*interpolationValue;
        mesh.point(*v_it)[1] = unSmoothedVertices.at(i)[1] + (vec.y/99)*interpolationValue;
        mesh.point(*v_it)[2] = unSmoothedVertices.at(i)[2] + (vec.z/99)*interpolationValue;
        i++;
    }
}


std::vector<std::pair<std::string, int>> MeshWrapper::getMeshInfo(){
    std::vector<std::pair<std::string, int>> stats;

    std::pair<std::string, int> faceCount;
    faceCount.first = "Faces";
    faceCount.second = mesh.n_faces();
    stats.push_back(faceCount);

    for (HE_MESH::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it) {
        int faceValence = mesh.valence(*f_it);
        bool found = false;
        for(std::pair<std::string, int> &p : stats){
            std::string s = "FaceValence";
            s += std::to_string(faceValence);
            if(p.first == s){
                p.second = p.second + 1;
                found = true;
            }
        }if(!found){
            std::pair<std::string, int> faceV;
            faceV.first = "FaceValence";
            faceV.first += std::to_string(faceValence);
            faceV.second = 1;
            stats.push_back(faceV);
        }
    }


    std::pair<std::string, int> vertexCount;
    vertexCount.first = "Vertices";
    vertexCount.second = mesh.n_vertices();
    stats.push_back(vertexCount);

    for (HE_MESH::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
        int vertexValence = mesh.valence(*v_it);
        bool found = false;
        for(std::pair<std::string, int> &p : stats){
            std::string s = "VertexValence";
            s += std::to_string(vertexValence);
            if(p.first == s){
                p.second = p.second + 1;
                found = true;
            }
        }if(!found){
            std::pair<std::string, int> vertexV;
            vertexV.first = "VertexValence";
            vertexV.first += std::to_string(vertexValence);
            vertexV.second = 1;
            stats.push_back(vertexV);
        }
    }


    std::pair<std::string, int> halfEdgeCount;
    halfEdgeCount.first = "Half-Edges";
    halfEdgeCount.second = mesh.n_halfedges();
    stats.push_back(halfEdgeCount);



    return stats;


}






