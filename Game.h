// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#ifndef GAME_H
#define GAME_H

#include <memory>
#include "Window\Window.h"

class Game
{
public:
  Game();
  ~Game();

  int Run();

  void Initialize();

  void Update(float dt);

  void Draw(float dt);

private:
  bool Initialized = false;

  std::unique_ptr<Window> mWindow;
};

#endif // GAME_H
