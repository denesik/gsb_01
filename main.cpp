﻿// ============================================================================
// ==                   Copyright (c) 2016, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "Game.h"
#include <stdlib.h>
#include <time.h>
#include "tools/Log.h"

int main()
{
  srand(static_cast<unsigned int>(time(0)));

  LogInit();

  Game game;
  return game.Run();
}

