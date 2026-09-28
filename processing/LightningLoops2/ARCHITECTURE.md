# LightningLoops2 Architecture

This document provides a high-level overview of the `LightningLoops2` Processing project architecture.

## Overview

LightningLoops2 is an interactive, real-time 3D drawing and projection mapping system. It receives 3D stroke data (points, colors, indices) from external clients via OSC or WebSockets, and renders them in a 3D environment. The application supports multiple virtual "projectors" (`FrameProjector`s) which can be independently positioned and rotated, allowing for complex scene compositions. 

The application additionally outputs the drawn strokes to external hardware devices (such as lasers or oscilloscopes) via the XYScope library.

## Core Components

### 1. Main Application & Lifecycle
* **`LightningLoops2.pde`**: The entry point and main loop of the application. It initializes all sub-systems (camera, projectors, networking, post-processing), manages the framerate, and orchestrates the `draw()` loop. It acts as the central coordinator for reading input and rendering output.

### 2. Scene Graph & Rendering
The visual elements are structured hierarchically:
* **`Cam.pde` (Camera)**: A custom 3D camera implemented using Java's `Robot` class for mouselook locking. It handles position, velocity, and orientation (pan/tilt), allowing the user to navigate the 3D space.
* **`FrameProjector.pde`**: Represents a virtual projector in the 3D scene. The application supports multiple projectors (currently two by default). Each projector manages a `strokesBuffer` of incoming strokes, applies its own 3D transformations (position, rotation via `Quaternion.pde`), and generates a `Frame` on a set interval.
* **`Frame.pde`**: A snapshot or collection of `Stroke`s meant to be drawn at a specific time. 
* **`Stroke.pde`**: Represents a single continuous drawn line. It contains a collection of 3D `PVector` points, a color, and a lifespan. Strokes are responsible for drawing themselves onto a `PGraphics3D` texture using standard Processing `vertex()` calls, as well as sending their geometry to XYScope via `xy1.line()`.

### 3. Networking & Input
The system is designed to receive drawing data from external clients (e.g., VR drawing tools).
* **`Osc.pde`**: Listens for OSC messages (such as `/contour` or `/scanline`), decodes byte arrays containing color and 3D point data, and assigns the resulting `Stroke` to the appropriate `FrameProjector` based on the sender's hostname.
* **`Websocket.pde`**: A WebSocket client that polls a central server (`ws://vr.fox-gieg.com:8080`) for frame data encoded in JSON. It decodes the JSON arrays into `Stroke` objects and adds them to the projector buffers.

### 4. Controls & Interaction
* **`Controls.pde`**: Translates keyboard input (WASDQE + Space) into movement commands.
* **`GameMode.java`**: An enum defining the current control state. Using the `TAB` and numeric keys, the user can switch between controlling the main Camera (`CAM`), or the position/rotation of individual FrameProjectors (`DEPTH1_POS`, `DEPTH1_ROT`, `DEPTH2_POS`, `DEPTH2_ROT`).

### 5. Post-Processing & Output
The application applies various effects to the rendered 3D scene before outputting to the screen:
* **`Bloom.pde` & `Sharpen.pde`**: Wrapper classes for applying GLSL shader effects (located in the `data/shaders` directory).
* **XYScope Output**: Baked directly into `Stroke.pde` and `Frame.pde`, sending vector data to audio/laser hardware.
* **`SoundOut.pde`**: Manages audio output.
* **`Settings.pde`**: Reads and writes configuration data (like `settings.txt`).

## Data Flow

1. **Input Reception**: A client sends a stroke via OSC or WebSockets.
2. **Decoding**: `Osc.pde` or `Websocket.pde` decodes the message into a list of 3D `PVector` points and a color.
3. **Buffering**: A new `Stroke` is created and appended to the `strokesBuffer` of a specific `FrameProjector`.
4. **Frame Generation**: Based on the `fps` timing in the main loop, `FrameProjector`s instantiate new `Frame`s containing the current active strokes (discarding those past their `lifespan`).
5. **Rendering**: The main `draw()` loop iterates through the `FrameProjector`s, pushing their transform matrices, and asking them to render their current `Frame`.
6. **Hardware Output**: During rendering, `Stroke`s draw themselves to the screen buffer and push their points to the XYScope library.
7. **Post-Processing**: Shaders (Bloom, Sharpen) are applied to the final screen buffer before display.
