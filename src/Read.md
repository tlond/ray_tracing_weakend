1. [OpenGL - Computer Graphics Basics](https://www3.ntu.edu.sg/home/ehchua/programming/opengl/CG_BasicsTheory.html)
2. [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html#rays,asimplecamera,andbackground)
3. [Scratchapixel - Generating Camera Rays](https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays.html)

Feel free to explore these resources for more in-depth understanding of computer graphics and ray tracing concepts!
## Ray Tracing Process

In a simple ray tracing example, the process you've described is generally accurate. Here's a step-by-step breakdown:

### 1. Raster to Normalized Device Coordinates (NDC):

- You start by iterating through each pixel on the image (raster) plane.
- You convert the pixel's coordinates from raster space (usually integer pixel coordinates) to NDC. This is done by normalizing the coordinates to the range [-1, 1] along both the horizontal (x) and vertical (y) axes.

### 2. NDC to Screen Space:

- The NDC coordinates are then transformed to screen space. Screen space might be the same as NDC if no further transformation is needed, or it might involve scaling and translating to fit the viewport dimensions.

### 3. Screen Space to Camera Coordinates:

- In this step, you apply a transformation that converts points from screen space to camera space. This transformation typically involves reversing the camera's projection transformation (often called the "inverse projection matrix"). It takes points in the screen space and maps them back to the camera's view volume.

### 4. Camera Coordinates to World Coordinates:

- Here, you transform the points from the camera's local coordinate system to the world coordinate system. This is achieved using the camera-to-world matrix that you've constructed earlier. Applying this matrix takes points from the camera's viewpoint to the world space, allowing you to position the camera in the 3D scene.

### 5. Ray Tracing:

- After transforming the point from camera coordinates to world coordinates, you have a 3D point in the scene.
- To simulate the path of light, you create a ray that originates from the camera's center and passes through this world-space point. This ray represents the path that light would take from the camera to the object in the scene.
- You can then use this ray to perform ray-object intersection tests with the scene geometry to determine what objects the ray intersects and calculate lighting, shading, and reflections.

Remember that the details might vary depending on the specifics of the ray tracing implementation and the conventions of the graphics library or framework you are using. But the core idea of transforming coordinates from raster space to world space and casting rays from the camera's perspective is accurate.