#include <food.h>
#include <QDebug>
Food::Food(glm::vec2 _boardHalfVector)
{
    m_boardHalfVec = _boardHalfVector;
    ngl::Random *rand=ngl::Random::instance();
    ngl::Vec2 randVect2 = rand->getRandomVec2();
    glm::vec2 floatPosition = glm::vec2(randVect2.m_x, randVect2.m_y) * (_boardHalfVector + glm::vec2(0.5f, 0.5f));
    m_position.x = int(floatPosition.x);
    m_position.y = int(floatPosition.y);
    m_colour = rand->getRandomColour();

    m_mesh.reset( new ngl::Obj("models/foodMesh.obj"));
    m_mesh->createVAO();
}

ngl::Mat4 Food::getPosMatrix()
{
    ngl::Mat4 posM;
    posM.translate(m_position.x, 0.0, -m_position.y);
    return posM;
}

void Food::addRandom()
{
    ngl::Random *rand=ngl::Random::instance();
    ngl::Vec2 randVect2 = rand->getRandomVec2();
    glm::vec2 floatPosition = glm::vec2(randVect2.m_x, randVect2.m_y) * (m_boardHalfVec - glm::vec2(1.0f, 1.0f));
    m_position.x = int(floatPosition.x);
    m_position.y = int(floatPosition.y);
    m_colour = rand->getRandomColour();
}

ngl::Colour Food::getColour()
{
    return m_colour;
}

glm::vec2 Food::getPos()
{
    return m_position;
}
