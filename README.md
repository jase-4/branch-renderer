# OpenGL Renderer

<img width="750" height="450" alt="renderer" src="https://github.com/user-attachments/assets/28547b2c-a3f5-4255-b694-9525cfa21208" />

A real-time 3D renderer written in **C++** and **OpenGL**.  
Supports dynamic lighting, shadow mapping, and an **Entity-Component-System (ECS)** architecture with interface-based polymorphism.

## Features
- **Lighting:** Per-fragment Blinn-Phong lighting (ambient, diffuse, specular).  
- **Shadows:** Depth-based shadow mapping with adjustable bias.  
- **ECS Architecture:** Lightweight ECS for flexible scene and object management.  
- **Shader System:** Custom GLSL vertex and fragment shaders for materials, lighting, and depth passes.  
- **Subsystems:** Singleton managers for shaders and resources.  
