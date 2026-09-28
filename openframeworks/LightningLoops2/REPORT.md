# LightningLoops2 openFrameworks Port: Report

The openFrameworks port of the Processing sketch (`processing/LightningLoops2/`) is complete. It builds with no warnings from the project's own code. It was run on a Raspberry Pi 4 under a virtual display (Xvfb) with real OSC input: strokes land within 1–2 px of the expected screen positions, and the oscilloscope audio traces the right shapes at 50 Hz. Nothing has been committed yet.

## Location

`/home/pi/openFrameworks/of_v0.12.1_linuxaarch64_release/LightningLoops2/openframeworks/LightningLoops2/`

- **Source:** `src/`
- **Data:** `bin/data/` (`settings.txt` and `shaders/`)
- **Built app:** `bin/LightningLoops2`
- **Build config:** `Makefile` and `addons.make`

## Structure

Each Processing file has a C++ counterpart:

| Processing | openFrameworks |
|---|---|
| `LightningLoops2.pde`, `Controls.pde`, `Osc.pde`, `Websocket.pde` | `ofApp.h/.cpp` |
| `Cam.pde` | `Cam.h/.cpp` |
| `FrameProjector.pde`, `Quaternion.pde` | `FrameProjector.h/.cpp` |
| `Frame.pde` | `Frame.h/.cpp` |
| `Stroke.pde` | `Stroke.h/.cpp` |
| `Settings.pde` | `Settings.h/.cpp` |
| `GameMode.java` | `GameMode.h` |
| `Bloom.pde` | `Bloom.h/.cpp`, `shaders/bloom_blur.frag`, `shaders/bloom_merge.frag` |
| `Sharpen.pde` | `Sharpen.h/.cpp`, `shaders/sharpen.frag` |
| `SoundOut.pde` (XYscope) | `XYScope.h/.cpp` |
| — | `PassShader.h` (loads the shaders on GL2, GL3 or GLES) |

Three Java libraries needed replacing:

- **XYscope/Minim** → `XYScope`, an `ofSoundStream` output (left = X, right = Y) that follows XYscope's waveform algorithm.
- **PixelFlow bloom** → a GLSL blur-and-add effect driven by the same `Bloom Amount` setting.
- **Processing websockets** → ofxLws, with JSON parsing done on its own thread.

## Build changes

- `addons.make` is now `ofxOsc` + `ofxLws`. OF's built-in JSON replaces ofxJSON.
- The Makefile's `OF_ROOT` default pointed at `/home/pi`; it is now `../../..`, matching the `yellowtail` project at the same depth.
- `settings.txt` was copied into `bin/data/`, and the shaders live in `bin/data/shaders/`.
- `main.cpp` opens a fullscreen GL 2.1 window (GLES2 on `TARGET_OPENGLES`), following `PiLatkPlayer`.

## What was tested

- OSC in PiLatkPlayer's `/contour` format, including `/scanline` and malformed or NaN messages (ignored, no crash).
- The real `settings.txt`: both projectors render at their saved poses with the glow.
- Keyboard controls, mouselook, and saving with `o`. Values round-trip without precision loss; zeros now save as `0` instead of `0.0`.
- The WebSocket path against a local server, with sound off.
- Strokes expiring after a client stops sending.
- The sharpen pass (temporarily switched on, then back off).

## Bugs found and fixed during testing

- **WebSocket crash:** ofxLws segfaulted when asked for no subprotocol (as the Java client does), so the port uses ofxLws's default protocol. A server that doesn't reply with a subprotocol still accepts it.
- **Mouselook jump:** the first mouse move after pressing `c` could spin the view a full turn, so that first move is now ignored.

## Behaviour changes to review

Each of these fixes an apparent bug in the original:

1. **The scope now draws strokes from both projectors.** The original cleared it once per projector, so only the last projector reached it; with a single Pi connected it drew nothing.
2. **`Activity Threshold` now takes effect.** The key has a trailing space in the file, so Processing ignored it and used 1. The port reads the configured 10, so scope output starts at more than 10 strokes on projector 1.
3. **Frames are real 12 fps snapshots, and expired strokes are dropped**, as `ARCHITECTURE.md` describes. In the Java version each frame shared the live stroke list, so the 12 fps setting did nothing and a client's last strokes stayed frozen on screen after it stopped.
4. **WebSocket strokes now expire.** In the original that code was commented out, so the stroke list grew forever.
5. **The mode label is pinned top-left and shows only in control modes.** In the original its position followed the camera, which put it off-screen with the saved camera.

## Carried over as-is

- The scope uses only each stroke's newest segment, with raw x/y scaled to the window size.
- Sharpen is off by default (`useSharpen` in `ofApp.h`), since `sharpenDraw()` is commented out in the Processing sketch.

## Removed

- Code that nothing called: the `Encoding` helpers, `LNPoint`, `sendOscContour`, and the unused shaders.
- `Quaternion` became `glm::quat`, keeping the exact matrix formula so saved rotations look the same.

## Not verified

- On-screen output and frame rate on the Pi 4's real GPU (tests used software GL).
- The live `vr.fox-gieg.com` server.
- Real scope or laser hardware.
- Mac and Windows builds.
- How the glow compares side by side with PixelFlow, whose source isn't on this machine; `Bloom Amount` may need retuning.
