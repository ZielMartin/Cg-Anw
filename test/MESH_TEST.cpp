//
// Created by ZielM on 30.04.2018.
//

#include "MyMesh.h"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include "gtest/gtest.h"

namespace cg {

// The fixture for testing class Foo.
    class MESH_TEST : public ::testing::Test {
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        MESH_TEST() {
            // You can do set-up work for each test here.
        }

        virtual ~MESH_TEST() {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        virtual void SetUp() {
            // Code here will be called immediately after the constructor (right
            // before each test).



            if (!OpenMesh::IO::read_mesh(mesh, (char*) "../obj/cube.obj")) {
                std::cerr << "Error loading mesh"<< std::endl;
            }
        }



        virtual void TearDown() {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }

        // Objects declared here can be used by all tests in the test case for Foo.
        HE_MESH mesh;
    };


    TEST_F(MESH_TEST, TestPairs){

        for (HE_MESH::HalfedgeIter he_it = mesh.halfedges_begin(); he_it != mesh.halfedges_end(); ++he_it) {
            ASSERT_EQ(mesh.opposite_halfedge_handle(*he_it),*he_it);
        }

    }


    TEST_F(MESH_TEST, TestFaces) {

        for (HE_MESH::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it) {
            HE_MESH::HalfedgeHandle firstHE = mesh.halfedge_handle(*f_it);
            HE_MESH::HalfedgeHandle curr = mesh.halfedge_handle(*f_it);

            do{
                ASSERT_EQ(mesh.face_handle(curr), *f_it);
                curr = mesh.next_halfedge_handle(curr);
            }while(firstHE != curr);
        }

    }

    TEST_F(MESH_TEST, TestVertices) {

        for (HE_MESH::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it) {
            HE_MESH::HalfedgeHandle curr = mesh.halfedge_handle(*v_it);
            do{
                ASSERT_EQ(*v_it, mesh.from_vertex_handle(curr));
                curr = mesh.opposite_halfedge_handle(mesh.next_halfedge_handle(curr));
            }while(curr != mesh.halfedge_handle(*v_it));
        }

    }

}  // namespace

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}