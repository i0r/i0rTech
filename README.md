# i0rTech
A 3D engine, built with OpenGL.

<img src="screenshot1.png" alt="i0rTech Screenshot 1" height="300px" width="300px">
<img src="screenshot2.png" alt="i0rTech Screenshot 2" height="300px" width="300px">
<img src="screenshot3.png" alt="i0rTech Screenshot 3" height="300px" width="300px">
<img src="screenshot5.png" alt="i0rTech Screenshot 4" height="300px" width="300px">
<img src="screenshot5-2.png" alt="i0rTech Screenshot 5" height="302px" width="604px">

## Features
- Deferred Tiled Shading
- Physically Based Rendering
- Dynamic Environment Mapping
- Blender Export Addon
- Input handler
- Debug logging console
- Task Manager for multithreading
- FXAA
- Shaders / Textures hotswapping
- Debug Menu
- Mouse picking (using raycasting)
- Editor view
- Cascaded Shadow Mapping with PCF

## Build Requirements
- CMake (>= 2.8)

## Default keymapping
- ESC - Exit the engine<br/>
- F   - Change Current Mode (Game/Editor)<br/>
- WASD (or ZQSD) - Move the camera (Game mode)<br/>
- Mouse Scroll - Camera Zoom (Editor mode)<br/>
- Right Mouse Button (Held) - Camera 'Focus' Zoom (Editor mode)<br/>
- Left Mouse Button (Held) - Pick and move the item being hovered<br/>

### The following keys are only available in a debug build:<br/>
- HOME - Open the debug menu<br/>
- PAGEUP/PAGEDOWN - Navigate in the debug menu<br/>
- END - Execute the selected item (trigger a callback, enter in a submenu, ...)<br/>
- INSERT/DELETE - Increment/Decrement the selected variable (if the selected item is a bool, float, int, ...)<br/>
