# renderingEngine
### A 3D rendering pipeline using real-life data.

I'm attempting to create a very basic 3D reconstruction pipeline! The goal of this project is to generate a series of point clouds using an Arduino Lidar scanner, and processing the data load in a rendering engine. I also wish to automate this process from scanning to processing to rendering. I'm using Open3D to convert the data into a mesh, OpenGL and Assimp to render the mesh, and ImGUI to dynamically load files. 

## Milestones
- [x] Create a rendering engine capable of displaying meshes with textures.
- [x] Enable the renderer to load in pre-defined meshes through Assimp.
- [ ] Statically scan a room from a fixed camera perspective using a Lidar scanner. Display results as a mesh.
- [ ] Scan an object using a circular rig for a 360 perspective of the object. Display results as a mesh.

### WIP Screenshots
**Phong lighting + texture mapping.**

![specularmap](https://user-images.githubusercontent.com/53636492/161247388-4decaa4f-1857-4cd9-a612-51091729c35a.png)

**Model Loading.**

![bunny cube map](https://user-images.githubusercontent.com/53636492/161961294-f9484a60-6a3d-433f-a049-ef14f40e208e.PNG)

**Edge Detection** (Post-processing filter).

![edge detection](https://user-images.githubusercontent.com/53636492/161684568-062ef1f2-e11e-4f23-81e5-607e855b11c0.PNG)

**Materials!**

![glass wih split channels](https://user-images.githubusercontent.com/53636492/163186450-2c3daa3f-49d5-4c4c-8b52-54c5b45d6a74.PNG)

**Point Cloud translation to Mesh**

![geom in open3d_169](https://user-images.githubusercontent.com/53636492/163922155-09a43ba4-0bc0-4454-9b2d-91c2b9cae639.png)


## Boilerplate Features

- [x] Triangles!
- [x] 3D Projection and Camera Movement
- [x] Custom texture mapping
- [x] Blinn-Phong lighting
- [x] Depth Buffer
- [x] Simple Materials
- [x] Anti-aliasing (MSAA)

## Planned Advanced Features

- [x] .gltf Loading using assimp libraries
- [x] Multiple texture maps (e.g. spec, normal, diff) 
- [x] Simple materials; reflections, refractions, diffusion
- [x] Cube maps (static, TODO: dynamic)
- [x] Frame buffer trickery (current effects: colour manipulation, edge detection, TODO: bloom, depth of field, kuwahara blur)
- [ ] Volume rendering; fog, clouds, fur 
- [ ] Instancing
- [ ] Shadow Mapping
- [ ] SSAO
- [ ] Multiple Lights


## Credits

This work is based on "Stanford Dragon PBR" (https://sketchfab.com/3d-models/stanford-dragon-pbr-5d610f842a4542ccb21613d41bbd7ea1) by hackmans (https://sketchfab.com/hackmans) licensed under CC-BY-4.0 (http://creativecommons.org/licenses/by/4.0/)

This work is based on "Stanford Bunny PBR" (https://sketchfab.com/3d-models/stanford-bunny-pbr-42c9bdc4d27a418daa19b2d5ff690095) by hackmans (https://sketchfab.com/hackmans) licensed under CC-BY-4.0 (http://creativecommons.org/licenses/by/4.0/)


