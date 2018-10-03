
# MAJ Engine

## 1. How to build on Windows

- Make sure you have 7zip installed in "C:\Program Files\"
-> if you don't, Unzip Lib.zip available in the project directory and move the unziped directory before the project directory to have the lib directory and the project directory at the same level (cf TDD 1.c).
- Execute install.bat
- disconnect, reconnnect the session
- Open Visual Studio 2017 and go to File -> Open -> Folder then choose the root directory, "Editor", "Game" and "Editor/src" on different Visual instance.
-  Install root directory CMakeLists.txt

### 1.1 How to build Editor
-  Install Editor CMakeLists.txt
-  Build Game CMakeLists.txt with Script.dll
-  Build Editor/src CMakeLists.txt with MAJEditor.exe

### 1.2 How to build Game
-  Build Game CMakeLists.txt with Game.exe

## 2. Features
- Textured cube
- Blinn lighting
- Input management (mouse, keyboard, controller)
- OBJ loading
- Cubemap
- Post-Process
- RHI Memory Management
- FBX loading
- Animation
- Multiple viewport (icon, cursor management)
- Scene Hierarchy and Component system
- Serialization and Asset Management
- Reflection
- User Behaviours
- Physics Simulation
- Sprite & Text Rendering
- Editor-only Gizmos
- Editor (Inspector, Asset Folder, Hierarchy, ...)
- Runtime Scripting

## 3. Controls
### 3.1. Camera Movement
- In-Editor:
W: Moves forward
A: Moves left
S: Moves backward
D: Moves right
Q: Moves up
E: Moves down
Right-Click + Mouse movement: Rotates
- In-Game:
W: Moves forward
A: Moves left
S: Moves backward
D: Moves right
Mouse movement: Rotates

### 3.2. Demo
- Pad Enter : Toggles gravity on barrel
- Pad  '+' : Enlarge the barrel size
- Pad '-' : Reduce the barrel size
- Up/ Down/ Right/ Left : Make the player move in its local space

### 3.3. Debug
- F10: Memory dump
- F5: play the simulation
- DEL : delete Actor or Scene

## 4. Issues
- can't lock cursor in editor
- MSVC Debug don't  run

## 4. Credits
- Martin Garnier
- Aurel Pierre
- Jannah Mekhaemar
  