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
#include "Render/VertexArray.h"
#include "Render/UniformBase.h"


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


static uint32_t indexCubeSide[] =
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

  TextureManager mTextureManager;

  mTextureManager.LoadDirectory("data\\textures\\");
  mTextureManager.Compile();

  auto texture = mTextureManager.GetTexture("data\\textures\\test_texture.png");

  auto shader = std::make_shared<Shader>();
  shader->BuildBody("data\\basic.glsl");
  shader->BuildType(GL_FRAGMENT_SHADER);
  shader->BuildType(GL_VERTEX_SHADER);
  shader->Link();

  UniformBasic uniform;
  uniform.AttachShader(*shader);

  auto camera = std::make_shared<Camera>();
  //camera->Move({});


  auto uv = mTextureManager.GetTextureUV("data\\textures\\test_texture.png");

  std::vector<VertexVT> vertexs;
  vertexs.push_back({ vertexCube[0],{ uv.x, uv.y } });
  vertexs.push_back({ vertexCube[1],{ uv.z, uv.y } });
  vertexs.push_back({ vertexCube[2],{ uv.z, uv.w } });
  vertexs.push_back({ vertexCube[3],{ uv.x, uv.w } });


  VertexArray vao;
  {
    auto &buf = vao.CreateBuffer(vertexs.size() * sizeof(VertexVT));
    memcpy(buf.Data(), vertexs.data(), buf.Size());
    for (auto &attibute : VertexVT::Get())
    {
      buf.SetAttibute(attibute);
    }
  }
  {
    auto &buf = vao.CreateIndexBuffer(6 * sizeof(uint32_t));
    memcpy(buf.Data(), indexCubeSide, buf.Size());
  }
  vao.Compile();

  struct
  {
    std::array<int, GLFW_KEY_LAST + 1> keys;
  } events_state;

  for (auto &key : events_state.keys)
  {
    key = GLFW_RELEASE;
  }

  mWindow->SetKeyboardCallback([&events_state](int key, int, int action, int)
  {
    events_state.keys[key] = action;
  });

  //camera->Move({ 0, 0, 10 });
  //camera->Update();

  auto currTime = static_cast<float>(glfwGetTime());
  while (!mWindow->WindowShouldClose())
  {
    auto lastTime = currTime;
    currTime = static_cast<float>(glfwGetTime());
    
    Update(currTime - lastTime);
    Draw(currTime - lastTime);

    if (events_state.keys[GLFW_KEY_W] == GLFW_REPEAT)
    {
      //camera->Move({});
    }


    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));     // Очистка экрана

    texture->Set(TEXTURE_SLOT_0);
    shader->Use();

    //shader->SetUniform(TEXTURE_SLOT_0, "atlas");
    //shader->SetUniform(camera->GetViewProject(), "transform_VP");
    uniform.atlas(TEXTURE_SLOT_0);
    uniform.mat_vp(camera->GetProject() * camera->GetView());
    uniform.Bind();

    vao.Draw();
    //mesh.Draw();

    mWindow->Update();
	  //std::this_thread::sleep_for(std::chrono::milliseconds(1)); ?!
  }
  return 0;
}


void Game::Update(float dt)
{

}


void Game::Draw(float dt)
{

}

