// ============================================================================
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
#include "core/Entity.h"
#include "tools/FpsCounter.h"
#include "Render/MeshManager.h"


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

  MeshManager mMeshManager;
  mMeshManager.LoadDirectory("data\\meshes\\");
  mMeshManager.Compile();

  auto texture = mTextureManager.GetTexture("data\\textures\\test_texture.png");

  Shader shader;
  shader.LoadFromFile("data\\basic.glsl");
  shader.Compile();

  UniformBasic uniform;
  uniform.AttachShader(shader);

  Camera camera;

  auto uv = mTextureManager.GetTextureUV("data\\textures\\test_texture.png");

  auto vao = mMeshManager.GetMesh("data\\meshes\\cube.obj");

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
    if (action == GLFW_PRESS) action = GLFW_REPEAT;
    events_state.keys[key] = action;
  });

  Entity player;

  FpsCounter fps;
  auto currTime = static_cast<float>(glfwGetTime());
  while (!mWindow->WindowShouldClose())
  {
    fps.Update();
    mWindow->SetTitle(std::to_string(fps.GetCount()) + " fps");
    auto lastTime = currTime;
    currTime = static_cast<float>(glfwGetTime());
    
    Update(currTime - lastTime);
    Draw(currTime - lastTime);

    if (events_state.keys[GLFW_KEY_W] == GLFW_REPEAT) player.Move({ 0, 0, -0.001 });
    if (events_state.keys[GLFW_KEY_S] == GLFW_REPEAT) player.Move({ 0, 0, 0.001 });
    if (events_state.keys[GLFW_KEY_A] == GLFW_REPEAT) player.Move({ -0.001, 0, 0 });
    if (events_state.keys[GLFW_KEY_D] == GLFW_REPEAT) player.Move({ 0.001, 0, 0 });

    if (events_state.keys[GLFW_KEY_UP] == GLFW_REPEAT) player.Rotate({ -0.001, 0, 0 });
    if (events_state.keys[GLFW_KEY_DOWN] == GLFW_REPEAT) player.Rotate({ 0.001, 0, 0 });
    if (events_state.keys[GLFW_KEY_LEFT] == GLFW_REPEAT) player.Rotate({ 0, -0.001, 0 });
    if (events_state.keys[GLFW_KEY_RIGHT] == GLFW_REPEAT) player.Rotate({ 0, 0.001, 0 });

    player.Update();
    camera.SetPos(player.GetPos());
    camera.SetQuat(player.GetQuat());
    camera.Update();

    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));     // Очистка экрана

    texture->Set(TEXTURE_SLOT_0);
    shader.Use();

    //shader->SetUniform(TEXTURE_SLOT_0, "atlas");
    //shader->SetUniform(camera->GetViewProject(), "transform_VP");
    uniform.atlas(TEXTURE_SLOT_0);
    uniform.mat_vp(camera.GetProject() * camera.GetView());
    uniform.Bind();

    vao->Draw();
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

