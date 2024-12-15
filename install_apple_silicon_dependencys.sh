#!/bin/bash

echo "Updating Homebrew..."
brew update

echo "Installing SDL2 and extensions..."
brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf meson ninja cmake

echo "Cleaning up unused files..."
brew cleanup

echo "Dependencies installed successfully!"