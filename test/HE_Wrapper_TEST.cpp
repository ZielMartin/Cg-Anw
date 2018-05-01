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
                    HE_vert(-1.0, -1.0, 1.0),
                    HE_vert(1.0, -1.0, 1.0),
                    HE_vert(-1.0, 1.0, 1.0),
                    HE_vert(1.0, 1.0, 1.0),
                    HE_vert(-1.0, 1.0, -1.0),
                    HE_vert(1.0, 1.0, -1.0),
                    HE_vert(-1.0, -1.0, -1.0),
                    HE_vert(1.0, -1.0, -1.0)
            };

            std::vector<std::vector<int>> faces = {
                    {0, 1, 2},
                    {2, 1, 3},

                    {2, 3, 4},
                    {4, 3, 5},

                    {4, 5, 6},
                    {6, 5, 7},

                    {6, 7, 0},
                    {0, 7, 1},

                    {1, 7, 3},
                    {3, 7, 5},

                    {6, 0, 4},
                    {4, 0, 2}
            };

            halfEdgeStruct.verts = VertList();

            for (HE_vert &vert : vertsExample) {
                halfEdgeStruct.verts.push_back(VertPointer(new HE_vert(vert)));
            }

            EdgeList beschleunigungsListe = EdgeList();


            for (std::vector<int> &faceIndices : faces) {
                FacePointer newFace = FacePointer(new HE_face);
                EdgePointer prev = nullptr;

                for (int &num : faceIndices) {
                    EdgePointer curr = EdgePointer(new HE_edge);
                    VertPointer edgeStartingFrom = halfEdgeStruct.verts[num];

                    halfEdgeStruct.edges.push_back(curr);
                    curr->vert = edgeStartingFrom;

                    if(edgeStartingFrom->edge == nullptr)
                        edgeStartingFrom->edge = curr;

                    if (newFace->edge == nullptr)
                        newFace->edge = curr;

                    if (prev != nullptr)
                        prev->next = curr;

                    prev = curr;
                }
                prev->next = newFace->edge;
            }

            for (EdgePointer currentEdge : halfEdgeStruct.edges) {
                if (currentEdge->pair == nullptr) {

                    VertPointer pointingTo = currentEdge->next->vert;
                    bool matchFound = false;
                    for (EdgePointer edgeWithoutPair : beschleunigungsListe) {
                        if (edgeWithoutPair->vert == pointingTo &&
                            currentEdge->vert == edgeWithoutPair->next->vert) {
                            matchFound = true;
                            currentEdge->pair = edgeWithoutPair;
                            edgeWithoutPair->pair = currentEdge;
                        }
                    }
                    if (!matchFound) {
                        beschleunigungsListe.push_back(currentEdge);
                    }
                }
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
        for (EdgePointer edge : halfEdgeStruct.edges) {
            EdgePointer e = edge;
            do {
                //vergleiche ziel-vertex von e mit start-vertex von seinem pair & start-vertex von e mit ziel-vertex seines pairs
                if ((e->next->vert != e->pair->vert) || (e->pair->next->vert != e->vert)) {
                    failed++;
                }
                e = e->next;
            } while (e.get() != e.get());
        }
        ASSERT_EQ(0, failed) << failed << " pairs sind inkonsistent: es gibt: " << halfEdgeStruct.edges.size()
                             << " halfedges";
    }


    TEST_F(HE_WrapperTest, TestFaces) {
        //key: azahl der kanten des faces / val: anzahl der faces mit dieser kantenanzahl
        std::map<int, int> facevalenz;

        int failed = 0;

        for (FacePointer f : halfEdgeStruct.faces) {
            int edgeCounter = 0;
            EdgePointer e = f->edge;
            do {
                if ((e->vert->edge->vert != e->vert) || e->face != f) {
                    failed++;
                }

                edgeCounter++;

                e = e->next;
            } while (e.get() != f->edge.get());

            facevalenz[edgeCounter]++;

        }
        ASSERT_EQ(0, failed) << failed << " faces sind inkonsistent: es gibt: " << halfEdgeStruct.faces.size()
                             << " faces" << std::endl;

        for (std::map<int, int>::iterator iter = facevalenz.begin(); iter != facevalenz.end(); ++iter) {
            std::cout << "Facevalenz: " << iter->first << " Face-Anzahl: " << iter->second << std::endl;
        }
        std::cout << std::endl;
    }

    TEST_F(HE_WrapperTest, TestVertices) {
        //key: anzahl der ausgehenden kanten, val: anzahl der vertices mit dieser anzahl(key) der ausgehenden kanten
        std::map<int, int> vertexvalenz;

        int failed = 0;

        for (VertPointer v : halfEdgeStruct.verts) {

            //vertexvalenz
            //vertexvalenz[v->alledges.size()]++;


            EdgePointer e = v->edge;
            do {
                ASSERT_NE(nullptr, e) << "IST NULL";
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