# Square vs Triangle Game

A simple 2D OpenGL game where you control a square to catch a triangle before time runs out.

## Features
- WASD controls for square movement
- Mouse-controlled triangle with smooth movement
- Score tracking with high scores
- 30-second timer
- Collision detection
- Game over screen with restart option

## How to Play
1. Use **WASD** to move the gray square
2. Move your mouse to control the orange triangle
3. Catch the triangle with the square to win
4. Press **R** to restart after game over

## Installation
```bash
# Clone the repository
# Compile
g++ -o game main.cpp -lglut -lGLU -lGL

# Run
./game
