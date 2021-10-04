#include <board.h>

Board::Board(glm::vec2 _halfVector, glm::vec2 _origin)
{
    m_halfVector = _halfVector;
    m_origin = _origin;
}

ngl::Mat4 Board::getPosM()
{
    ngl::Mat4 posM;
    posM.translate(m_origin.x, -0.001, -m_origin.y);
    return posM;
}
