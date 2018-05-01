//
// Created by ZielM on 30.04.2018.
//

#include "HalfEdge.h"
#include "gtest/gtest.h"

namespace cg {

// The fixture for testing class Foo.
    class HE_WrapperTest : public ::testing::Test {
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        HE_WrapperTest() {
            // You can do set-up work for each test here.
        }

        virtual ~HE_WrapperTest() {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        virtual void SetUp() {
            // Code here will be called immediately after the constructor (right
            // before each test).

            HE_vert vertsExample[] = {
                    HE_vert(-1.0, -1.0, -1.0),
                    HE_vert(-1.0, -1.0, 1.0),
                    HE_vert(-1.0, 1.0, -1.0),
                    HE_vert(-1.0, 1.0, 1.0),
                    HE_vert(1.0, -1.0, -1.0),
                    HE_vert(1.0, -1.0, 1.0),
                    HE_vert(1.0, 1.0, -1.0),
                    HE_vert(1.0, 1.0, 1.0)
            };

            halfEdgeStruct.verts = std::vector<std::shared_ptr<HE_vert>>();

            for(HE_vert &vert : vertsExample) {
                halfEdgeStruct.verts.push_back(std::shared_ptr<HE_vert>(new HE_vert(vert)));
            }

            for(std::shared_ptr<HE_vert> &vert : halfEdgeStruct.verts) {

            }

        }

        virtual void TearDown() {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }

        // Objects declared here can be used by all tests in the test case for Foo.
        HalfEdgeStruct halfEdgeStruct;
    };


    TEST_F(HE_WrapperTest, TestPairs) {
        int failed = 0;
        for (std::shared_ptr<HE_edge> edge : halfEdgeStruct.edges) {
            std::shared_ptr<HE_edge> e = edge;
            do {
                //vergleiche ziel-vertex von e mit start-vertex von seinem pair & start-vertex von e mit ziel-vertex seines pairs
                if ((e->next->vert != e->pair->vert) || (e->pair->next->vert != e->vert)) {
                    failed++;
                }
                e = e->next;
            } while (e != edge);
        }
        std::cout << std::endl << failed << " pairs sind inkonsistent: es gibt: " << halfEdgeStruct.edges.size() << " halfedges" << std::endl;
        std::cout << std::endl;
    }


    TEST_F(HE_WrapperTest, TestFaces) {
        //key: azahl der kanten des faces / val: anzahl der faces mit dieser kantenanzahl
        std::map<int, int> facevalenz;

        int failed = 0;

        for (std::shared_ptr<HE_face> f : halfEdgeStruct.faces) {
            int edgeCounter = 0;
            std::shared_ptr<HE_edge> e = f->edge;
            do {
                if ((e->vert->edge->vert != e->vert) || e->face != f) {
                    failed++;
                }

                edgeCounter++;

                e = e->next;
            } while (e != f->edge);

            facevalenz[edgeCounter]++;

        }
        std::cout << failed << " faces sind inkonsistent: es gibt: " << halfEdgeStruct.faces.size() << " faces" << std::endl;

        for (std::map<int, int>::iterator iter = facevalenz.begin(); iter != facevalenz.end(); ++iter) {
            std::cout << "Facevalenz: " << iter->first << " Face-Anzahl: " << iter->second << std::endl;
        }
        std::cout << std::endl;
    }

    TEST_F(HE_WrapperTest, TestVertices) {
        //key: anzahl der ausgehenden kanten, val: anzahl der vertices mit dieser anzahl(key) der ausgehenden kanten
        std::map<int, int> vertexvalenz;

        int failed = 0;

        for (std::shared_ptr<HE_vert> v : halfEdgeStruct.verts) {

            //vertexvalenz
            //vertexvalenz[v->alledges.size()]++;


            std::shared_ptr<HE_edge> e = v->edge;
            do {
                std::cout << "DO" << std::endl;
                e = e->pair->next;
            } while (e != v->edge);
        }
        std::cout << "es gibt : " << halfEdgeStruct.verts.size() << " vertices" << std::endl;

        for (std::map<int, int>::iterator iter = vertexvalenz.begin(); iter != vertexvalenz.end(); ++iter) {
            std::cout << "vertexvalenz: " << iter->first << " vertexanzahl: " << iter->second << std::endl;
        }
        std::cout << std::endl;
    }

}  // namespace

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}