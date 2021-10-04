#include <snake.h>
#include <QDebug>
Snake::Snake(glm::vec2 _origin, glm::vec2 _direction)
{
  m_segments.append(_direction);
  m_origin = _origin;
  m_piecesCount = glm::length(_direction);
}

void Snake::move(glm::vec2 _move)
{
    m_currentDirection  = glm::normalize(m_segments.last());

    if(_move != glm::vec2(0.0,0.0) && _move != m_currentDirection && _move != -m_currentDirection)
    {
        m_segments.append(_move);
    }
    else
    {
        m_segments.last() += m_currentDirection;
    }

    if(!m_isJustEaten)
    {
        glm::vec2 firstSegNorm = glm::normalize(m_segments.first());
        m_segments.first() -= firstSegNorm;
        m_origin += firstSegNorm;
        if(m_segments.first() == glm::vec2(0.0,0.0))
        {
            m_segments.removeFirst();
        }
    }
    else
    {
        m_piecesCount ++;
        m_isJustEaten = false;
    }
}

void Snake::eat()
{
    m_isJustEaten = true;
}


SnakeRecord Snake::getRecord(glm::vec2 _boardHalfVector, glm::vec2 _boardOrigin, glm::vec2 _foodPos)
{
    SnakeRecord record {false, false};
    glm::vec2 segmentChain = m_segments.last();

    if(segmentChain.length()>1)
    {
        QList<glm::vec2>::const_iterator i;
        for (i = m_segments.constEnd()-2; i != m_segments.constBegin()-1; --i)
        {

            glm::vec2 tempSegmentChain = segmentChain + *i;

            if(tempSegmentChain.x == 0.0 || tempSegmentChain.y == 0.0)
            {
                if(tempSegmentChain != glm::vec2(0.0,0.0))
                {
                    if((segmentChain.x == 0.0 && tempSegmentChain.x == 0.0)
                            || (segmentChain.y == 0.0 && tempSegmentChain.y == 0.0))
                    {
                        if((segmentChain.x > 0.0 && tempSegmentChain.x < 0.0)
                                || (segmentChain.y > 0.0 && tempSegmentChain.y < 0.0)
                                || (segmentChain.x < 0.0 && tempSegmentChain.x > 0.0)
                                || (segmentChain.y < 0.0 && tempSegmentChain.y > 0.0))
                        {
                            record.isColliding = true;
                            return record;
                        }

                    }
                }
                else
                {
                    record.isColliding = true;
                    return record;
                }
            }

            segmentChain += *i;
        }
    }

    if((m_origin.x + segmentChain.x > _boardHalfVector.x + _boardOrigin.x) || (m_origin.y + segmentChain.y > _boardHalfVector.y + _boardOrigin.y)
            || (m_origin.x + segmentChain.x < -_boardHalfVector.x + _boardOrigin.x) || (m_origin.y + segmentChain.y < -_boardHalfVector.y + _boardOrigin.y))
    {
        record.isColliding = true;
        return record;
    }

    if(m_origin + segmentChain==_foodPos)
    {
        record.isFed = true;
    }

    return record;
}
