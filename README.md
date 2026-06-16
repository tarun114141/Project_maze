# Maze Generator & Solver
A real-time maze generation and pathfinding visualizer built from scratch using **C++ and OpenGL (GLUT)** — no game engine, no framework, just raw OpenGL.

Built as a side experiment to explore OpenGL rendering and simple animations while taking a break from game engine work.

---



---

## How It Works

### Maze Generation — Randomized DFS
The maze is built using a **Depth-First Search** algorithm with backtracking:
- Starts at cell (0,0)
- Randomly picks an unvisited neighbour, removes the wall between them
- Backtracks using a stack when no unvisited neighbours are found
- Continues until every cell has been visited — guaranteeing a perfect maze (exactly one path between any two cells)

### Maze Solving — BFS
The solver uses **Breadth-First Search** to find the shortest path from top-left to bottom-right:
- Explores cells layer by layer through accessible (wall-removed) neighbours
- Tracks parent cells to reconstruct the path once the exit is reached
- Animates the explored cells in blue, then reveals the optimal path in red

---

## States
The app runs on a simple state machine:

```
IDLE → GENERATING → GENERATED → SOLVING → SOLVED
```

| State | Description |
|---|---|
| IDLE | Waiting for input |
| GENERATING | DFS building the maze step by step |
| GENERATED | Maze done, waiting for Solve |
| SOLVING | BFS exploring the maze |
| SOLVED | Shortest path revealed |

---

## Controls

| Button | Action | Available When |
|---|---|---|
| Create Maze | Generates a new random maze | IDLE |
| Solve | Runs BFS to find shortest path | GENERATED |
| Reset | Clears everything, back to IDLE | Anytime |

---

## Color Guide

| Color | Meaning |
|---|---|
| Dark grey | Visited cell during generation |
| Green (bright) | Current cell being processed |
| Dark blue | Cell explored by BFS |
| Red | Shortest path |
| Green (walls) | Maze walls |

---

## Project Structure

```
maze/
├── maze.cpp          # Main file — cell, Maze class, OpenGL callbacks
├── Button.h          # Reusable OpenGL button class
├── SimpleRecorder.h  # Frame capture utility
└── frames/           # Auto-created — captured PPM frames go here

```

### Button.h
A small reusable utility written for this project — handles drawing, hover effects, and click detection entirely in OpenGL/GLUT. No external UI library needed. Can be dropped into any GLUT project.

### SimpleRecorder.h
A lightweight frame capture utility that reads raw pixels from the OpenGL framebuffer each tick and saves them as `.ppm` image files into a `frames/` folder. The folder is created automatically on first run.

---

## Recording & Rendering Video

Frames are saved automatically to the `frames/` folder as numbered PPM files (`frame00000.ppm`, `frame00001.ppm`, ...) while the app runs.

To render them into a video using **ffmpeg**:

```bash
# MP4 video at 30fps
ffmpeg -framerate 30 -i frames/frame%05d.ppm -c:v libx264 -pix_fmt yuv420p output.mp4
```

```bash
# GIF (good for GitHub/LinkedIn demo)
ffmpeg -framerate 30 -i frames/frame%05d.ppm -vf "fps=30,scale=700:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse" output.gif
```

> **Tip:** Only record the frames you want. You can toggle recording on/off by commenting out `saveAnimationFrame()` in the timer function in `maze.cpp`.

---

## Build & Run

**Requirements:**
- C++ compiler (g++)
- OpenGL + GLUT (freeglut on Windows)
- ffmpeg (optional, for rendering video)

**Compile:**
```bash
g++ -I./GL -L./lib -o maze.exe maze.cpp -lfreeglut -lopengl32 -lglu32
```

**Run:**
```bash
./maze.exe
```

Frames will be saved to `frames/` automatically while the app is running.

---

## Tech Stack
- **Language:** C++
- **Graphics:** OpenGL (immediate mode) + GLUT
- **Algorithms:** Randomized DFS (generation), BFS (solving)
- **Data Structures:** Stack, Queue, 2D vectors

---

## Why I Built This
I'm primarily a game developer and wanted to get hands-on with raw OpenGL — no engine abstractions, no helper libraries. This project let me explore:
- Viewport management and 2D rendering in OpenGL
- Building a simple GUI (buttons, hover states) from scratch
- Animating algorithms step by step using a timer loop
- State machine architecture for interactive apps
- Frame capture and video rendering pipeline with ffmpeg

Simple project, but everything is built from the ground up.

---
