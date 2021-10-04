#include <QMouseEvent>
#include <QGuiApplication>

#include "Scene.h"
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/Transformation.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <QDebug>

Scene::Scene()
{
  setTitle("Snake");
}


Scene::~Scene()
{
}

void Scene::resizeGL( int _w, int _h )
{
  m_cam.setShape( 45.0f, static_cast<float>( _w ) / _h, 0.05f, 350.0f );
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
  m_text->setScreenSize(width(),height());
  m_score->setScreenSize(width(),height());
}

void Scene::initializeGL()
{
  ngl::NGLInit::instance();

  glClearColor(0.1f, 0.1f, 0.2f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from(0,20,30);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);

  // first we create a mesh from an obj passing in the obj file and texture
  m_snakeMesh.reset( new ngl::Obj("models/snakeMesh.obj"));
  m_snakeMesh->createVAO();

  m_cam.set(from,to,up);
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam.setShape(45.0f,(float)720.0/576.0f,0.5f,150.0f);
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  shader->createShaderProgram("Phong");

  shader->attachShader("PhongVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("PhongFragment",ngl::ShaderType::FRAGMENT);
  shader->loadShaderSource("PhongVertex","shaders/PhongVert.glsl");
  shader->loadShaderSource("PhongFragment","shaders/PhongFrag.glsl");

  shader->compileShader("PhongVertex");
  shader->compileShader("PhongFragment");
  shader->attachShaderToProgram("Phong","PhongVertex");
  shader->attachShaderToProgram("Phong","PhongFragment");

  shader->linkProgramObject("Phong");
  (*shader)["Phong"]->use();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=m_mouseGlobalTX*m_transform.getMatrix();
  MV=m_cam.getViewMatrix()*M ;
  MVP=m_cam.getProjectionMatrix()*MV ;
  normalMatrix=MV;
  normalMatrix.inverse().transpose();
  shader->setUniform("MV",MV);
  shader->setUniform("MVP",MVP);
  shader->setUniform("normalMatrix",normalMatrix);
  shader->setUniform("M",M);

  m_board = new Board(glm::vec2(14.0,14.0), glm::vec2(0.0,0.0));
  m_snake = new Snake(glm::vec2(0.0,0.0), glm::vec2(0.0,1.0));
  m_food = new Food(m_board->getHalfVector());

  QVector<glm::mat4> transforms(m_snake->m_piecesCount);
  int currentTransform = 0;
  for(auto &seg : m_snake->m_segments)
  {
    glm::vec2 segNorm = glm::normalize(seg);

    int segLen = glm::length(seg);

    for(int i=0; i<segLen;i++)
    {
        glm::vec2 currentSeg = segNorm * glm::vec2(i+1.0);
        transforms[currentTransform] = glm::translate(glm::mat4(), glm::vec3(currentSeg.x,0.0,currentSeg.y));
        currentTransform++;
    }
  }

  createTBO(m_tboID, m_textureID, GL_TEXTURE0, transforms);

  // now pass the modelView and projection values to the shader
  shader->setUniform("Normalize",1);
  shader->setUniform("viewerPos",m_cam.getEye().toVec3());

  // now set the material and light values
  m_snakeMaterial.set(ngl::STDMAT::POLISHEDSILVER);
  m_snakeMaterial.loadToShader("material");
  // create the lights
  createLights();

  m_text->setScreenSize(width(),height());
  m_score->setScreenSize(width(),height());
  m_score.reset( new ngl::Text(QFont("Helvetica",60)));
  m_score->setColour(0.3,0.3,0.4);
}


void Scene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  QVector<glm::mat4> transforms(m_snake->m_piecesCount);
  int currentTransform = 0;
  glm::vec2 currentOrigin = m_snake->m_origin;
  for(auto &seg : m_snake->m_segments)
  {
    glm::vec2 segNorm = glm::normalize(seg);

    int segLen = glm::length(seg);
    for(int i=0; i<segLen;i++)
    {
        glm::vec2 currentSeg = segNorm * glm::vec2(i + m_timestep) + currentOrigin;
        transforms[currentTransform] = glm::translate(glm::mat4(), glm::vec3(currentSeg.x,0.0,-currentSeg.y));
        currentTransform++;
    }

    currentOrigin += seg;
  }

  if(m_gameState != GAME_OVER && m_gameState != PAUSE && m_gameState != START)
  {
      if(m_timestep < 1.0)
      {
        m_timestep+=m_step;
      }
      else
      {
          m_timestep = 0.0;

          switch (m_gameState) {
            case FORWARD: m_snake->move(c_snakeMoves.forward); break;
            case TURN_UP:  m_snake->move(c_snakeMoves.up); m_gameState = FORWARD; break;
            case TURN_DOWN:  m_snake->move(c_snakeMoves.down); m_gameState = FORWARD; break;
            case TURN_RIGHT:  m_snake->move(c_snakeMoves.right); m_gameState = FORWARD; break;
            case TURN_LEFT:  m_snake->move(c_snakeMoves.left); m_gameState = FORWARD; break;
            case GAME_OVER: break;
            case PAUSE: break;
            case START: break;
          }

      }
  }

  SnakeRecord tempRecord = m_snake->getRecord(m_board->getHalfVector(), m_board->getOrigin(), m_food->getPos());
  if(tempRecord.isColliding)
  {
      m_gameState = GAME_OVER;
  }
  else if(tempRecord.isFed)
  {
      m_snake->eat();
      m_food->addRandom();
  }
  // bind and fill TBO
  glBindBuffer(GL_TEXTURE_BUFFER, m_tboID);
  glBufferData(GL_TEXTURE_BUFFER, transforms.size()*sizeof(glm::mat4), &transforms[0][0][0], GL_STATIC_DRAW);
  // attatch to texture ( Texture unit 0 in this case as using not others)
  glActiveTexture( GL_TEXTURE0 );
  glBindTexture(GL_TEXTURE_BUFFER,m_textureID);
  // Note GL_RGBA32F as using Mat4 -> 4* vec4 in size
  glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_tboID);
}

void Scene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ngl::Transformation trans;
  // Rotation based on the mouse position for our global
  // transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX(m_win.spinXFace);
  rotY.rotateY(m_win.spinYFace);
  // multiply the rotations
  m_mouseGlobalTX=rotY*rotX;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use("Phong");

  // now set this value in the shader for the current ModelMatrix
  m_snakeMesh->bindVAO();
  shader->setUniform("meshType", SNAKE);
  shader->setUniform("colour", ngl::Colour(0.24,0.6,0.24,1.0));

  loadMatricesToShader();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_BUFFER, m_tboID);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D,m_textureID);

  glDrawArraysInstanced(GL_TRIANGLES,0,m_snakeMesh->getMeshSize(),m_snake->m_piecesCount);
  m_snakeMesh->unbindVAO();

  shader->setUniform("meshType", FOOD);
  shader->setUniform("colour", m_food->getColour());
  shader->setUniform("foodPosM", m_food->getPosMatrix());
  m_food->m_mesh->draw();
  ngl::VAOPrimitives *smallBoard=ngl::VAOPrimitives::instance();
  smallBoard->createLineGrid("smallBoard", 2, 2, 2);
  smallBoard->draw("smallBoard");

  shader->setUniform("meshType", GRID);
  shader->setUniform("gridPosM", m_board->getPosM());
  shader->setUniform("colour", ngl::Colour(0.2, 0.2,0.3,1.0));

  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->createLineGrid("board", m_board->getHalfVector().x*2, m_board->getHalfVector().y*2, int(m_board->getHalfVector().x)*2);
  prim->draw("board");


  if(m_gameState != START && m_gameState != GAME_OVER)
  {
      QString scoreText=QString("- %1 -").arg(m_snake->m_piecesCount-1);
      if(m_snake->m_piecesCount < 11)
      {
          m_score->renderText(width()/2-50,50,scoreText);
      }
      else
      {
          m_score->renderText(width()/2-70,50,scoreText);
      }

  }

  if(m_gameState == START)
  {
      m_text.reset( new ngl::Text(QFont("Arial",80)));
      m_text->setColour(0.2,0.2,0.3);
      QString text = QString("S N A K E");
      //QString text=QString("%1 instances %2 fps").arg(c_numTrees).arg(m_fps);
      m_text->renderText(width()/2-250,height()/2-300,text);

      m_text.reset( new ngl::Text(QFont("Helvetica",60)));
      m_text->setColour(0.8,0.8,1.0);
      text = QString("START");
      m_text->renderText(width()/2-130,height()/2 +30,text);

      m_text.reset( new ngl::Text(QFont("Helvetica",20)));
      m_text->setColour(0.5,0.5,0.6);
      text = QString("PRESS ARROWS TO");
      m_text->renderText(width()/2-130,height()/2 -60,text);
  }
  else if(m_gameState == GAME_OVER)
  {
      m_text.reset( new ngl::Text(QFont("Arial",80)));
      m_text->setColour(0.3,0.3,0.4);
      QString text = QString("GAME OVER");
      m_text->renderText(width()/2-300,height()/2 - 270,text);
      m_text.reset( new ngl::Text(QFont("Arial",30)));
      m_text->setColour(0.3,0.3,0.4);
      text=QString("YOUR SCORE: %1").arg(m_snake->m_piecesCount-1);
      m_text->renderText(width()/2-150, 50,text);
  }
  else if(m_gameState == PAUSE)
  {
      m_text.reset( new ngl::Text(QFont("Helvetica",60)));
      m_text->setColour(0.3,0.3,0.4);
      QString text = QString("PAUSE");
      m_text->renderText(width()/2-130,height()/2-10,text);
  }

  update();
}


//----------------------------------------------------------------------------------------------------------------------

void Scene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  // show full screen
  case Qt::Key_F : showFullScreen(); break;
  // show windowed
  case Qt::Key_N : showNormal(); break;
  case Qt::Key_Up : if(m_gameState==GAME_OVER){break;} m_gameState = TURN_UP; break;
  case Qt::Key_Down : if(m_gameState==GAME_OVER){break;} m_gameState = TURN_DOWN; break;
  case Qt::Key_Right : if(m_gameState==GAME_OVER){break;} m_gameState = TURN_RIGHT; break;
  case Qt::Key_Left : if(m_gameState==GAME_OVER){break;} m_gameState = TURN_LEFT; break;
  case Qt::Key_Space :
      if(m_gameState != PAUSE)
      {
          m_gameState = PAUSE;
      }
      else
      {
          m_gameState = FORWARD;
      }
      break;
  default : break;
  }
    update();
}


void Scene::createLights()
{
  // light position
  ngl::Vec3 pos;
  // light colour
  ngl::Colour col,speccol;
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use("Phong");
  // we need to load the inverse view matrix to the light transform
  // this will give us the correct light positions in eye space
  ngl::Mat4 iv=m_cam.getViewMatrix();
  iv.transpose();
  // loop for the NumLights lights and set the position and colour
  int i=0;
  for(auto &light : m_lightArray)
  {
    light.enable();
    // create an instance of the light and put it in the array
    light.setPosition(ngl::Vec3(15.0,15.0,15.0));
    light.setColour(ngl::Colour(0.1,0.1,0.1,1.0));
    light.setSpecColour(ngl::Colour(0.15,0.15,0.15,1.0));
    QString lightName=QString("light[%1]").arg(i++);
    light.setTransform(iv);
    light.loadToShader(lightName.toStdString());
  }
}

void Scene::createTBO(GLuint & _tboID, GLuint & _texID, GLenum _textureUnit, QVector<glm::mat4>& _transforms)
{
  // create a texture buffer to store the position and scale as a mat4 for each tree
  glGenBuffers(1,&_tboID);

  // bind and fill TBO
  glBindBuffer(GL_TEXTURE_BUFFER, _tboID);
  glBufferData(GL_TEXTURE_BUFFER, _transforms.size()*sizeof(glm::mat4), &_transforms[0][0][0], GL_STATIC_DRAW);
  // attatch to texture ( Texture unit 0 in this case as using not others)
  glGenTextures(1, &_texID);
  glActiveTexture( _textureUnit );
  glBindTexture(GL_TEXTURE_BUFFER,_texID);
  // Note GL_RGBA32F as using Mat4 -> 4* vec4 in size
  glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, _tboID);

}
