# iKanEngine
Game Engine

![](/Resources/Branding/iKan.png)

iKan is primarily an early-stage interactive application and rendering engine for Mac 

## Getting Started
XCode is recommended

You can clone the repository to a local destination using git:

`git clone --recursive https://github.com/ashish1009/iKanEngine`

Make sure that you do a `--recursive` clone to fetch all of the submodules!

## Features
1. Able to render Quad or multiple quads, with color or Texture or Sprite with least number of draw calls
2. Mario Game is developing inside the Workspace

## Road Map
1. Render 3D using Assimp
2. Add ECS for 3D rendering
3. Proper COllision for 2D

## How to use Library
1. Add the submodule with the command  - `git submodule add --force "https://github.com/ashish1009/iKanEngine" <DIR>`
2. Update all the submodule inside the iKan Engine - `git submodule update --init --recursive`
3. Add iKan.a to your project 
4. Add libglfw.a to your project from the path "/iKan/Vendor/GLFW/lib/", and Add Frameworks like IOKit, OpenGL and COCOA to support static library of GLFW
5. Add Preprocessors "IK_ENABLE_LOG", "IK_ENABLE_ASSERT"

## List of APIs <TODO>
