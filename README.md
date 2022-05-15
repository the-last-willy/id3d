This project contains some of my - Willy Jacquet - [master's degree](http://master-info.univ-lyon1.fr/ID3D/) class projects.

These projects were made using [AbstractGL](https://github.com/the-last-willy/abstractgl), a collection of libraries I made for graphics programming.

The underlying graphics API is OpenGL 4.5.

In this repository you can find all the source code as well as the reports that I submitted.
The reports are written in French though.

Below are images taken from the submitted reports.

# [3D Image Synthesis](3d_image_synthesis.pdf)

The goal of this project was to explore rendering.
I chose to focus on optimization and culling technics (frustum, occlusion and light).

Both the frustum and occlusion culling were implemented on compute shaders.

The whole scene is rendered within a single draw call in forward rendering.

A few final renders:

![image](https://user-images.githubusercontent.com/84744335/168464198-9928b0e7-3b35-4b49-a329-32d24c28ed41.png)

![image](https://user-images.githubusercontent.com/84744335/168464043-89e80def-a181-4148-aea6-34f64d1c2741.png)

![image](https://user-images.githubusercontent.com/84744335/168464084-923a883f-0886-40bf-b4df-9ca4b35176df.png)

![image](https://user-images.githubusercontent.com/84744335/168464101-b7456d01-3c29-43e6-a37e-f8e9734b5ff4.png)

Geometry cell subdivision:

![image](https://user-images.githubusercontent.com/84744335/168464693-e5a24339-844f-4f3a-b8fc-2bacc52bf1fd.png)

Frustum culling - on compute shaders:

![image](https://user-images.githubusercontent.com/84744335/168464647-f1de5eab-3dfb-4781-bd2a-e434e1bb6e19.png)

Hierarchical Z-buffer for occlusion culling - on compute shaders:

![image](https://user-images.githubusercontent.com/84744335/168464436-c44204bf-5818-4dfb-aab3-b895cf591c32.png)

[Occlusion culling](https://blog.selfshadow.com/publications/practical-visibility/) - on compute shaders:

![image](https://user-images.githubusercontent.com/84744335/168464608-b1c1bcda-af47-4457-bfdb-5439b67d4713.png)

Light density of the static grid used for light culling:

![image](https://user-images.githubusercontent.com/84744335/168464298-961a45dc-dcf5-4aa5-ac0a-9fd43d105e28.png)

Alpha cutoff:

![image](https://user-images.githubusercontent.com/84744335/168464232-7c97d241-e8fb-4b9a-a231-d5a53ab31914.png)

Gamma correction:

![image](https://user-images.githubusercontent.com/84744335/168464217-ebe69382-490f-4150-aa64-3b53829ee6de.png)

A bit too shiny:

![image](https://user-images.githubusercontent.com/84744335/168464259-2058b66e-be0e-429f-890d-8f3f4f25d8b0.png)

Real-time ray traced point cloud:

![image](https://user-images.githubusercontent.com/84744335/168463970-7191923d-cc10-45c8-87b8-5d57e6587e74.png)

# [Digital terrain modelling](digital_terrain_modelling.pdf)

The goal of this project was to implement common algortihms for terrain manipulation.

I made a real-time visualizer to load, explore and interact with terrains.

Computing vegetation distributions:

![image](https://user-images.githubusercontent.com/84744335/168465867-1e1ff0da-b3a4-4986-84b5-bf4749efca95.png)

Sampling and instancing vegetation:

![image](https://user-images.githubusercontent.com/84744335/168465881-c0224b6a-922d-45f9-98d6-7955f6ab8353.png)

Applying successive erosion operations - before:

![image](https://user-images.githubusercontent.com/84744335/168465497-1511dd59-0a8f-47b0-8349-bf4f7a6fac8d.png)

Applying successive erosion operations - after:

![image](https://user-images.githubusercontent.com/84744335/168465521-a6cabb05-f0ab-442c-9b32-93c2c80fe795.png)

Breaching:

![image](https://user-images.githubusercontent.com/84744335/168465550-6db8f8fb-2c27-4eff-ab20-76f849647879.png)

Debris slope erosion:

![image](https://user-images.githubusercontent.com/84744335/168465465-a9106410-9a01-4116-a5b0-5f489aa0d2bd.png)

Hill slope erosion:

![image](https://user-images.githubusercontent.com/84744335/168465575-47d7a080-667a-46ea-b4ba-85e8c8a2ef79.png)

Stream power erosion:

![image](https://user-images.githubusercontent.com/84744335/168465590-8a44b814-8e89-47bb-814e-6586f6d51e2f.png)

Computing drainage area using steepest and weighted methods:

![image](https://user-images.githubusercontent.com/84744335/168465442-269debc6-671f-4d91-b6b2-610afc86de0b.png)

Shaded, normals, slope, Laplacian.

![image](https://user-images.githubusercontent.com/84744335/168465616-008e086f-8717-4368-9203-63a62b40ab99.png)

Bigger terrain support:

![image](https://user-images.githubusercontent.com/84744335/168465774-ff2a05d0-0608-4739-ad5a-eafd6e8919b0.png)

# Implicit modelling ([Shadertoy](https://www.shadertoy.com/view/sd3XWX))

![image](https://user-images.githubusercontent.com/84744335/168463849-608fc122-2301-4456-93f5-f14e2005bf9f.png)

# Meshes and computational geometry
