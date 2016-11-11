﻿// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "Game.h"

#include <gl/glew.h>
#include <iostream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <fstream>
#include <memory>
#include <vector>
#include "Render/TextureManager.h"
#include "Render/Render.h"
#include "Render/Shader.h"
#include "Render/Camera.h"
#include "Render/RenderMeshVao.h"
#include "Render/OpenGLCall.h"



Game::Game()
{
  Window::WindowSystemInitialize();

  mWindow = std::make_unique<Window>(glm::uvec2(600, 600));
  mWindow->SetCurrentContext();

  Initialized = true;
}

Game::~Game()
{
  mWindow.reset();
  Window::WindowSystemFinally();
}

static glm::vec3 vertexCube[] =
{
  { 0.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f }, // front
  { 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 1.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f, 0.0f }, // right 
  { 1.0f, 1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f, 1.0f },{ 0.0f, 1.0f, 0.0f }, // back
  { 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 1.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f, 0.0f }, // left
  { 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f, 1.0f }, // top
  { 0.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f, 0.0f }  // bot
};

static glm::vec3 normalCube[] =
{
  { 0.0f, -1.0f,  0.0f }, // front
  { 1.0f,  0.0f,  0.0f }, // right 
  { 0.0f,  1.0f,  0.0f }, // back
  { -1.0f,  0.0f,  0.0f }, // left
  { 0.0f,  0.0f,  1.0f }, // top
  { 0.0f,  0.0f, -1.0f }, // bot
};


static size_t indexCubeSide[] =
{
  0, 1, 2, 2, 3, 0
};

static glm::vec2 textureCube[] =
{
  { 0.0f, 0.0f },{ 0.0f, 1.0f },{ 1.0f, 1.0f },{ 1.0f, 0.0f }
};


int Game::Run()
{
  if (!Initialized)
  {
    system("pause");
    return -1;
  }

  Render::Initialize();

  Render render;

  TextureManager::Get().LoadDirectory("data\\textures\\");
  TextureManager::Get().Compile();

  auto texture = TextureManager::Get().GetTexture("test_texture");

  auto &men = TextureManager::Get();

  auto shader = std::make_shared<Shader>();
  shader->BuildBody("data\\basic.glsl");
  shader->BuildType(GL_FRAGMENT_SHADER);
  shader->BuildType(GL_VERTEX_SHADER);
  shader->Link();

  auto camera = std::make_shared<Camera>();
  //camera->Move({});

  const auto &txtPos = std::get<glm::uvec4>(texture);

  glm::vec2 txtCoord[] =
  {
    { txtPos.x,            txtPos.y },
    { txtPos.x,            txtPos.y + txtPos.w },
    { txtPos.x + txtPos.z, txtPos.y + txtPos.w },
    { txtPos.x + txtPos.z, txtPos.y }
  };

  glm::vec2 scale(1.0f / (static_cast<glm::vec2>(std::get<0>(texture)->GetSize())));
  txtCoord[0] *= scale;
  txtCoord[1] *= scale;
  txtCoord[2] *= scale;
  txtCoord[3] *= scale;

  glm::vec2 txtScale((txtCoord[2].x - txtCoord[0].x), (txtCoord[2].y - txtCoord[0].y));

  glm::vec2 test[] =
  {
    textureCube[0] * txtScale + txtCoord[0],
    textureCube[1] * txtScale + txtCoord[0],
    textureCube[2] * txtScale + txtCoord[0],
    textureCube[3] * txtScale + txtCoord[0],
  };

  std::vector<VertexVT> vertexs;
  vertexs.push_back({ vertexCube[0],test[0] });
  vertexs.push_back({ vertexCube[1],test[3] });
  vertexs.push_back({ vertexCube[2],test[2] });
  vertexs.push_back({ vertexCube[3],test[1] });


  RenderMeshVao mesh;
  auto mAttribute = VertexVT::Get();
  auto locations = shader->GetAttributeLocation(mAttribute);
  mesh.SetAttribute(mAttribute, locations);
  mesh.Compile(reinterpret_cast<float *>(vertexs.data()), vertexs.size(), indexCubeSide, 6);

  //camera->Move({ 0, 0, 10 });
  //camera->Update();

  auto currTime = static_cast<float>(glfwGetTime());
  while (!mWindow->WindowShouldClose())
  {
    auto lastTime = currTime;
    currTime = static_cast<float>(glfwGetTime());
    
    Update(currTime - lastTime);
    Draw(currTime - lastTime);

    if (mWindow->GetKeyboard().IsKeyDown(GLFW_KEY_W))
    {
      //camera->Move({});
    }


    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));     // Очистка экрана

    std::get<0>(texture)->Set(TEXTURE_SLOT_0);
    shader->Use();
    shader->SetUniform(TEXTURE_SLOT_0, "atlas");

    shader->SetUniform(camera->GetViewProject(), "transform_VP");

    mesh.Draw();

    mWindow->Update();
	  //std::this_thread::sleep_for(std::chrono::milliseconds(1)); ?!
  }

  return 0;
}


void Game::Update(float dt)
{
  if (mWindow->GetKeyboard().IsKeyPress(GLFW_KEY_TAB))
  {
    mWindow->GetMouse().SetCentring(!mWindow->GetMouse().GetCentring());
  }

  if (mWindow->GetKeyboard().IsKeyDown(GLFW_KEY_F5))
  {

  }
}


void Game::Draw(float dt)
{

}

