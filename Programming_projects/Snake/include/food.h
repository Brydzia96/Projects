#ifndef FOOD_H
#define FOOD_H
#include <ngl/ShaderLib.h>
#include <glm/vec2.hpp>
#include <ngl/Random.h>
#include <ngl/Obj.h>
#include <ngl/Transformation.h>

class Food
{

public:
    Food(glm::vec2 _boardHalfVector);
    std::unique_ptr<ngl::Obj > m_mesh;
    ngl::Mat4 getPosMatrix();
    void addRandom();
    ngl::Colour getColour();
    glm::vec2 getPos();
private:
    glm::vec2 m_boardHalfVec;
    glm::vec2 m_position;
    ngl::Colour m_colour;

};

#endif // FOOD_H
