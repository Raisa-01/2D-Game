# Square vs Triangle Game

A simple 2D OpenGL game where a square and triangle compete in a timed challenge.

## 🎮 Game Rules

### Objective
- You control the **Gray Square** using keyboard
- The **Orange Triangle** follows your mouse cursor
- Your goal is to **catch the triangle with the square** before time runs out

### Controls
| Key/Action | Function |
|------------|----------|
| **W** | Move Square Up |
| **A** | Move Square Left |
| **S** | Move Square Down |
| **D** | Move Square Right |
| **Mouse Movement** | Triangle follows smoothly |
| **Left Click** | Set Triangle position |
| **R** | Restart Game |

### How to Win
- **Square Wins**: If you catch the triangle with the square
- **Triangle Wins**: If the 30-second timer runs out

### Scoring System
- **Square Wins**: Score = (Time Remaining) × 10
  - Example: If you catch with 25 seconds left → Score = 250
- **Triangle Wins**: Score = 100

### Game Rules Summary
1. Game starts with 30 seconds on the timer
2. Use WASD to move the gray square
3. The orange triangle follows your mouse smoothly
4. Try to collide the square with the triangle
5. Collision = Square Wins!
6. Timer reaches 0 = Triangle Wins!
7. Press 'R' to restart after game over

### Tips for Winning
- Move the square towards the triangle quickly
- Use mouse to keep triangle in a predictable path
- Don't waste time - you only have 30 seconds!
- Corner the triangle near the edges

## 🚀 How to Run

### Prerequisites
- OpenGL library
- GLUT library
- C++ compiler

### Installation & Compilation
```bash
# Clone the repository
git clone https://github.com/Raisa-01/2D-Game.git

# Navigate to project directory
cd square-triangle-game

# Compile the game
g++ -o game main.cpp -lglut -lGLU -lGL

# Run the game
./game
