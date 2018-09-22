//
//  application.cpp
//  EnTT Example
//
//  Created by Indi Kernick on 17/9/18.
//  Copyright © 2018 Indi Kernick. All rights reserved.
//

#include "application.hpp"

#include "game.hpp"
#include "dimensions.hpp"
#include <Simpleton/SDL/texture.hpp>
#include <Simpleton/SDL/library.hpp>
#include <Simpleton/Time/synchronizer.hpp>

SDL::Window::Desc getWinDesc() {
  SDL::Window::Desc desc;
  desc.title = "Pacman";
  desc.size = tiles * tileSize * scale;
  desc.resizable = false;
  desc.openGL = false;
  return desc;
}

void runGame() {
  SDL::Window window = SDL::makeWindow(getWinDesc());
  SDL::Renderer renderer = SDL::makeRenderer(window, false);
  SDL::Texture maze = renderer.texture("sprites.png");
  maze.blend(SDL_BLENDMODE_BLEND);
  Sprite::Sheet sheet = Sprite::makeSheet("sprites.atlas");
  CHECK_SDL_ERROR(SDL_RenderSetScale(renderer.get(), scale, scale));
  SDL::QuadWriter writer{renderer, sheet, maze};
  Game game;

  game.init(sheet);

  int frame = 0;
  bool quit = false;
  while (!quit) {
    Time::Synchronizer sync{std::chrono::nanoseconds{1000'000'000 / 10}};

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
      	quit = true;
      	break;
      } else if (e.type == SDL_KEYDOWN) {
      	game.input(e.key.keysym.scancode);
      }
    }

    if (frame == tileSize) {
      frame = 0;
      if (!game.logic()) {
        quit = true;
      }
    }

    renderer.clear();
    game.render(writer, frame);
    renderer.present();
    ++frame;
  }
}