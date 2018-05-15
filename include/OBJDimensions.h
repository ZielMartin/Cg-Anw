//
// Created by Johannes on 15.05.2018.
//

#ifndef CG_ANW_OBJDIMENSIONS_H
#define CG_ANW_OBJDIMENSIONS_H


#include <glm/vec3.hpp>

namespace cg {

    class OBJDimensions {

    public:


        void initData();

        void incrementVertexCounter();


        float getMaxXValue() const;

        void setMaxXValue(float maxXValue);

        float getMaxYValue() const;

        void setMaxYValue(float maxYValue);

        float getMaxZValue() const;

        void setMaxZValue(float maxZValue);

        float getMinXValue() const;

        void setMinXValue(float minXValue);

        float getMinYValue() const;

        void setMinYValue(float minYValue);

        float getMinZValue() const;

        void setMinZValue(float minZValue);

        float getSphereRadius() const;

        float getSphereSlices() const;

        float getSphereStacks() const;

        float getMoveStepSize() const;

        const glm::vec3 &getLightPosition() const;

        const glm::vec3 &getGridPosition() const;

        int getGridSize() const;


    private:
        float minXValue;
        float minYValue;
        float minZValue;

        float maxXValue;
        float maxYValue;
        float maxZValue;

        int vertexCounter;

        float sphereRadius;
        float sphereSlices;
        float sphereStacks;
        float moveStepSize;
        glm::vec3 lightPosition;
        glm::vec3 gridPosition;

        int gridSize;


    };

}

#endif //CG_ANW_OBJDIMENSIONS_H
