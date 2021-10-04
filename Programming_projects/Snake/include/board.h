#ifndef BOARD_H
#define BOARD_H
#include <glm/vec2.hpp>
#include <ngl/Transformation.h>

class Board
{

public:
    Board(glm::vec2 _halfVector, glm::vec2 _origin);
    inline glm::vec2 getHalfVector() const {return m_halfVector;}
    inline glm::vec2 getOrigin() const {return m_origin;}
    ngl::Mat4 getPosM();
private:
    glm::vec2 m_halfVector;
    glm::vec2 m_origin;

};

#endif // BOARD_H
