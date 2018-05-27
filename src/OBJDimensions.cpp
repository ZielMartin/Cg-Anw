//
// Created by Johannes on 15.05.2018.
//

#include <iostream>
#include <algorithm>
#include "OBJDimensions.h"

namespace cg {

    void OBJDimensions::initData() {

        std::cout << "maxY: " << maxYValue << std::endl;
        std::cout << "minY: " << minYValue << std::endl;


        lightPosition = glm::vec3(0.0, maxYValue + 100, 0.0);
        gridPosition = glm::vec3(0.0, minYValue, 0.0);

        if(vertexCounter < 100){
            sphereRadius = 0.1;
        }else if(vertexCounter < 1000){
            sphereRadius = 0.01;
        }else if(vertexCounter < 10000){
            sphereRadius = 0.001;
        }else{
            sphereRadius = 0.00001;
        }

        sphereSlices = sphereRadius*1000;
        sphereStacks = sphereSlices/2;
        moveStepSize = sphereRadius;

        gridSize = std::max(maxXValue-minXValue, maxYValue-minYValue) * 3;

        cameraPosition = glm::vec3(gridSize/2, gridSize/4, gridSize/4);




    }

    void OBJDimensions::incrementVertexCounter() {
        vertexCounter++;
    }


    float OBJDimensions::getMaxXValue() const {
        return maxXValue;
    }

    void OBJDimensions::setMaxXValue(float maxXValue) {
        OBJDimensions::maxXValue = maxXValue;
    }

    float OBJDimensions::getMaxYValue() const {
        return maxYValue;
    }

    void OBJDimensions::setMaxYValue(float maxYValue) {
        OBJDimensions::maxYValue = maxYValue;
    }

    float OBJDimensions::getMaxZValue() const {
        return maxZValue;
    }

    void OBJDimensions::setMaxZValue(float maxZValue) {
        OBJDimensions::maxZValue = maxZValue;
    }

    float OBJDimensions::getMinXValue() const {
        return minXValue;
    }

    void OBJDimensions::setMinXValue(float minXValue) {
        OBJDimensions::minXValue = minXValue;
    }

    float OBJDimensions::getMinYValue() const {
        return minYValue;
    }

    void OBJDimensions::setMinYValue(float minYValue) {
        OBJDimensions::minYValue = minYValue;
    }

    float OBJDimensions::getMinZValue() const {
        return minZValue;
    }

    void OBJDimensions::setMinZValue(float minZValue) {
        OBJDimensions::minZValue = minZValue;
    }

    float OBJDimensions::getSphereRadius() const {
        return sphereRadius;
    }

    float OBJDimensions::getSphereSlices() const {
        return sphereSlices;
    }

    float OBJDimensions::getSphereStacks() const {
        return sphereStacks;
    }

    float OBJDimensions::getMoveStepSize() const {
        return moveStepSize;
    }

    const glm::vec3 &OBJDimensions::getLightPosition() const {
        return lightPosition;
    }

    const glm::vec3 &OBJDimensions::getGridPosition() const {
        return gridPosition;
    }

    int OBJDimensions::getGridSize() const {
        return gridSize;
    }

    const glm::vec3 &OBJDimensions::getCameraPosition() const {
        return cameraPosition;
    }


}