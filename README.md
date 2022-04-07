# renderingEngine
A 3D rendering engine using OpenGL.

I'm attempting to make a rendering engine using the OpenGL API!

Boilerplate Features
--

- [x] Triangles!
- [x] 3D Projection and Camera Movement
- [x] Custom texture mapping
- [x] Phong lighting
- [ ] Multiple Lights
- [x] Depth Buffer
- [x] Simple Materials

Planned Advanced Features
--

- [x] .gltf Loading using assimp libraries
- [ ] Geometry Shader
- [ ] Compute Shader
- [x] Multiple texture maps (e.g. spec, normal, diff) 
- [ ] Advanced materials; PBR (dynamic reflections, refractions, diffusion, translucency)
- [ ] Volume rendering; fog, clouds, fur (probably through ray marching)
- [x] Frame buffer trickery (current effects: colour manipulation, edge detection, TODO: bloom, depth of field, kuwahara blur)
- [ ] Instancing
- [x] Cube maps (static, TODO: dynamic)
- [ ] Shadow Mapping
- [ ] SSAO
- [ ] Skeletal Animation

The final aim for the project is to flesh out a 3D scene using all the above rendering techniques.

WIP Screenshots
--
Phong lighting + texture mapping.

![specularmap](https://user-images.githubusercontent.com/53636492/161247388-4decaa4f-1857-4cd9-a612-51091729c35a.png)

Model Loading.

![bunny cube map](https://user-images.githubusercontent.com/53636492/161961294-f9484a60-6a3d-433f-a049-ef14f40e208e.PNG)

Edge Detection (Post-processing filter).

![edge detection](https://user-images.githubusercontent.com/53636492/161684568-062ef1f2-e11e-4f23-81e5-607e855b11c0.PNG)

Materials!

![materials](https://user-images.githubusercontent.com/53636492/162161527-b93bce22-d92f-4d77-a7e7-1f06394a02de.gif)


Credits
--
This work is based on "Stanford Dragon PBR" (https://sketchfab.com/3d-models/stanford-dragon-pbr-5d610f842a4542ccb21613d41bbd7ea1) by hackmans (https://sketchfab.com/hackmans) licensed under CC-BY-4.0 (http://creativecommons.org/licenses/by/4.0/)

This work is based on "Stanford Bunny PBR" (https://sketchfab.com/3d-models/stanford-bunny-pbr-42c9bdc4d27a418daa19b2d5ff690095) by hackmans (https://sketchfab.com/hackmans) licensed under CC-BY-4.0 (http://creativecommons.org/licenses/by/4.0/)


