#ifndef SNAKE_H
#define SNAKE_H

#include <QList>
#include <glm/glm.hpp>

struct Directions
{
    glm::vec2 forward;
    glm::vec2 up;
    glm::vec2 down;
    glm::vec2 left;
    glm::vec2 right;
};

const struct Directions c_snakeMoves =
{
    glm::vec2(0.0,0.0),
    glm::vec2(0.0,1.0),
    glm::vec2(0.0,-1.0),
    glm::vec2(-1.0,0.0),
    glm::vec2(1.0,0.0)
};

struct SnakeRecord {
    bool isColliding;
    bool isFed;
};

class Snake
{
public:
  Snake(glm::vec2 _origin, glm::vec2 _direction);
  void move(glm::vec2 _move);
  SnakeRecord getRecord(glm::vec2 _boardHalfVector, glm::vec2 _boardOrigin, glm::vec2 _foodPos);
  void eat();
  QList<glm::vec2> m_segments;
  glm::vec2 m_origin;
  int m_piecesCount;

private:
    bool m_isJustEaten = false;
    glm::vec2 m_currentDirection;
};

#endif // SNAKE_H

