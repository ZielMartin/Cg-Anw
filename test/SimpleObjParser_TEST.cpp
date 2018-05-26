//
// Created by ZielM on 04.05.2018.
//

#include <SimpleObjParser.h>
#include "gtest/gtest.h"

namespace cg {
// The fixture for testing class Foo.
    class SimpleObjParserTest : public ::testing::Test {
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        SimpleObjParserTest() {
            // You can do set-up work for each test here.
        }

        virtual ~SimpleObjParserTest() {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        virtual void SetUp() {
            // Code here will be called immediately after the constructor (right
            // before each test).

            setupCube();
        }

        void setupCube() {
            std::vector<glm::vec3> vertsExample = {
                    {glm::vec3(-1.0, -1.0, 1.0)},
                    {glm::vec3(1.0, -1.0, 1.0)},
                    {glm::vec3(-1.0, 1.0, 1.0)},
                    {glm::vec3(1.0, 1.0, 1.0)},
                    {glm::vec3(-1.0, 1.0, -1.0)},
                    {glm::vec3(1.0, 1.0, -1.0)},
                    {glm::vec3(-1.0, -1.0, -1.0)},
                    {glm::vec3(1.0, -1.0, -1.0)}
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

            this->he_wrapper = this->simpleObjParser.create(vertsExample, faces);


        }

        virtual void TearDown() {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }

        // Objects declared here can be used by all tests in the test case for Foo.
        std::shared_ptr<HE_Wrapper> he_wrapper = nullptr;
        SimpleObjParser simpleObjParser;
    };

    TEST_F(SimpleObjParserTest, TestPairs) {
        int failed = 0;
        for (EdgePointer edge : he_wrapper->getEdges()) {
            EdgePointer e = edge;
            do {
                ASSERT_NE(e->next, nullptr);
                ASSERT_NE(e->pair, nullptr);

                //vergleiche ziel-vertex von e mit start-vertex von seinem pair & start-vertex von e mit ziel-vertex seines pairs
                ASSERT_EQ(e->next->vert, e->pair->vert);
                ASSERT_EQ(e->pair->next->vert, e->vert);

                e = e->next;
            } while (e.get() != e.get());
        }
    }


    TEST_F(SimpleObjParserTest, TestFaces) {
        for (FacePointer f : he_wrapper->getFaces()) {
            EdgePointer e = f->edge;
            do {
                ASSERT_EQ(e->vert->edge->vert.get(), e->vert.get());
                ASSERT_EQ(e->face.get(), f.get());

                e = e->next;
            } while (e.get() != f->edge.get());

        }
        std::cout << std::endl;
    }

    TEST_F(SimpleObjParserTest, TestVertices) {
//key: anzahl der ausgehenden kanten, val: anzahl der vertices mit dieser anzahl(key) der ausgehenden kanten
        for (VertPointer v : he_wrapper->getVerts()) {

            EdgePointer e = v->edge;
            do {
                ASSERT_NE(e, nullptr) << "IST NULL";
                ASSERT_NE(e->pair, nullptr);
                e = e->pair->next;
            } while (e != v->edge);
        }

    }


}