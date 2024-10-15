#version 400

// Geometry shader for rendering cube-based grid, deforming based on a heightmap.
// This shader uses triangle strips to construct the geometry.

layout(points) in;  // Single point input per instance
layout(triangle_strip, max_vertices = 6) out;  // Outputting up to 6 vertices

// Uniform variables (global shader inputs)
uniform sampler2D uHeightMap;  // Heightmap texture for terrain
uniform vec2 uGridSize;        // Grid size in (columns, rows)
uniform vec2 uBaseSize;        // Size of the grid in world space
uniform float uMaxHeight;      // Maximum possible height of the cubes
uniform float uHeightScale;    // Scaling factor for height

uniform mat4 view;             // View matrix
uniform mat4 projection;       // Projection matrix

// Inputs from the vertex shader
in vec3 vPosition[];           // Local position of the cube
flat in int instanceID[];      // Instance ID to uniquely identify each cube

// Outputs to the fragment shader
out vec3 normal;               // Normal for lighting calculations
out vec3 fragPos;              // World-space position of the fragment
out vec2 texCoords;            // Texture coordinates (unused here)

// Helper function to create a quad wall (used for grid edges)
void CreateQuadWall(vec3 position1, vec3 position2, vec3 norm) {
    vec2 cubeSize = 1.0 / uGridSize * uBaseSize; // Cube dimensions in world space
    vec3 cubeVertices[4] = vec3[](
        vec3(position2.x, 0, position2.z),        // Bottom-right
        vec3(position2),                          // Top-right
        vec3(position1),                          // Top-left
        vec3(position1.x, 0, position1.z)         // Bottom-left
    );

    int indices[6] = int[](
        0, 3, 2, 0, 2, 1  // Indices for two triangles forming the quad
    );

    // Emit vertices as triangle strip
    for (int i = 0; i < 6; i++) {
        gl_Position = projection * view * vec4(cubeVertices[indices[i]], 1.0); // Transform to clip space
        fragPos = cubeVertices[indices[i]];  // Output fragment position
        normal = norm;                      // Set the normal
        texCoords = vec2(0, 0);             // Dummy texture coordinates
        EmitVertex();                       // Emit vertex
        if (i % 3 == 2) EndPrimitive();     // End primitive after each triangle
    }
}

// Helper function to create a cube face based on heights and normals
void CreateQuad(vec3 position, vec4 height, vec3 norm1, vec3 norm2) {
    vec2 cubeSize = 1.0 / uGridSize * uBaseSize;  // Cube dimensions
    vec2 cubeSizeHalf = cubeSize / 2.0;           // Half dimensions for positioning

    // Define the 4 vertices of the cube face
    vec3 cubeVertices[4] = vec3[](
        vec3(-cubeSizeHalf.x, height.x, -cubeSizeHalf.y), // Top-left-back
        vec3(cubeSizeHalf.x, height.y, -cubeSizeHalf.y),  // Top-right-back
        vec3(cubeSizeHalf.x, height.z, cubeSizeHalf.y),   // Top-right-front
        vec3(-cubeSizeHalf.x, height.w, cubeSizeHalf.y)   // Top-left-front
    );

    // Compute normals for the two triangles
    norm1 = normalize(cross(cubeVertices[3] - cubeVertices[0], cubeVertices[2] - cubeVertices[0]));
    norm2 = normalize(cross(cubeVertices[2] - cubeVertices[0], cubeVertices[1] - cubeVertices[0]));

    int indices[6] = int[](
        0, 3, 2, 0, 2, 1  // Indices for two triangles forming the quad
    );

    // Emit vertices as triangle strip
    for (int i = 0; i < 6; i++) {
        gl_Position = projection * view * vec4(position + cubeVertices[indices[i]], 1.0);  // Transform to clip space
        fragPos = position + cubeVertices[indices[i]];  // Output fragment position
        normal = (i < 3) ? norm1 : norm2;               // Set the normal for each triangle
        texCoords = vec2(0, 0);                         // Dummy texture coordinates
        EmitVertex();                                   // Emit vertex
        if (i % 3 == 2) EndPrimitive();                 // End primitive after each triangle
    }
}

// Cubic interpolation helper function
vec4 cubic(float v) {
    vec4 n = vec4(1.0, 2.0, 3.0, 4.0) - v;
    vec4 s = n * n * n;
    return vec4(s.x, s.y - 4.0 * s.x, s.z - 4.0 * s.y + 6.0 * s.x, 6.0 - s.x - s.y - s.z) * (1.0 / 6.0);
}

// Function for bicubic texture sampling
vec4 textureBicubic(sampler2D samp, vec2 texCoords) {
    ivec2 texSize = textureSize(samp, 0);
    vec2 invTexSize = 1.0 / vec2(texSize);

    // Adjust texture coordinates
    texCoords = texCoords * texSize - 0.5;
    vec2 fxy = fract(texCoords);
    texCoords -= fxy;

    // Compute cubic interpolation weights
    vec4 xcubic = cubic(fxy.x);
    vec4 ycubic = cubic(fxy.y);

    // Sample offsets
    vec4 c = texCoords.xxyy + vec2(-0.5, +1.5).xyxy;
    vec4 s = vec4(xcubic.xz + xcubic.yw, ycubic.xz + ycubic.yw);
    vec4 offset = c + vec4(xcubic.yw, ycubic.yw) / s;
    offset *= invTexSize.xxyy;

    // Sample texture using offsets
    vec4 sample0 = texture(samp, offset.xz);
    vec4 sample1 = texture(samp, offset.yz);
    vec4 sample2 = texture(samp, offset.xw);
    vec4 sample3 = texture(samp, offset.yw);

    // Interpolate final color
    float sx = s.x / (s.x + s.y);
    float sy = s.z / (s.z + s.w);

    return mix(mix(sample3, sample2, sx), mix(sample1, sample0, sx), sy);
}

void main() {
    // Compute grid position based on instance ID
        int row = instanceID[0] / int(uGridSize.x + 2);  // Y grid coordinate
        int col = instanceID[0] % int(uGridSize.x + 2);  // X grid coordinate

        vec2 cubeSize = 1.0 / uGridSize * uBaseSize;  // Cube size
        vec2 cubeSizeHalf = cubeSize / 2.0;           // Half size for positioning

    // Map to grid position in world space
    if(row == 0 || row == uGridSize.y+1 || col == 0 || col == uGridSize.x+1)
    {
        if((row == 0 && col == 0 )
          || (row == 0 && col == uGridSize.x+1)
          || (row == uGridSize.y+1 && col == 0 )
          || (row == uGridSize.y+1 && col == uGridSize.x+1)
        )
        {
            return;
        }
        if(row == 0)
        {
            vec3 gridPosition = vec3(
                float(col-1)/uGridSize.x * uBaseSize.x - uBaseSize.x/2.f,
                0.0,
                float(row)/uGridSize.y * uBaseSize.y - uBaseSize.y/2.f - cubeSizeHalf.y
            );
            vec2 uv;
            uv = vec2(
                float(col-1 - 0.5) / uGridSize.x,
                float(row) / uGridSize.y
            );
            float height1 = textureBicubic(uHeightMap, uv).r * uHeightScale;
            uv = vec2(
                float(col-1 + 0.5) / uGridSize.x,
                float(row) / uGridSize.y
            );
            float height2 = textureBicubic(uHeightMap, uv).r * uHeightScale;
            CreateQuadWall(
                vec3(gridPosition.x - cubeSizeHalf.x, height1, gridPosition.z),
                vec3(gridPosition.x + cubeSizeHalf.x, height2, gridPosition.z),
                vec3(0, 0, -1)
            );
        }
        if(col == 0) {
            vec3 gridPosition = vec3(float(col)/uGridSize.x * uBaseSize.x - uBaseSize.x/2.f - cubeSizeHalf.x, 0.0, float(row-1)/uGridSize.y * uBaseSize.y - uBaseSize.y/2.f);
            vec2 uv;
            uv = vec2(float(col) / uGridSize.x, float(row-1-0.5) / uGridSize.y);
            float height1 = textureBicubic(uHeightMap, uv).r * uHeightScale;
            uv = vec2(float(col) / uGridSize.x, float(row-1+0.5) / uGridSize.y);
            float height4 = textureBicubic(uHeightMap, uv).r * uHeightScale;
            CreateQuadWall(
                vec3(gridPosition.x, height4, gridPosition.z+cubeSizeHalf.y),
                vec3(gridPosition.x, height1, gridPosition.z-cubeSizeHalf.y),
                vec3(-1,0,0)
            );
        }
        if(row == uGridSize.y+1) {
            vec3 gridPosition = vec3(float(col-1)/uGridSize.x * uBaseSize.x - uBaseSize.x/2.f, 0.0, float(row-2)/uGridSize.y * uBaseSize.y - uBaseSize.y/2.f + cubeSizeHalf.y);
            vec2 uv;
            uv = vec2(float(col-1+0.5) / uGridSize.x, float(row-2) / uGridSize.y);
            float height3 = textureBicubic(uHeightMap, uv).r * uHeightScale;
            uv = vec2(float(col-1-0.5) / uGridSize.x, float(row-2) / uGridSize.y);
            float height4 = textureBicubic(uHeightMap, uv).r * uHeightScale;
            CreateQuadWall(
                vec3(gridPosition.x + cubeSizeHalf.x, height3, gridPosition.z),
                vec3(gridPosition.x - cubeSizeHalf.x, height4, gridPosition.z),
                vec3(0,0,1)
            );
        }
        if(col == uGridSize.x+1) {
            vec3 gridPosition = vec3(float(col-2)/uGridSize.x * uBaseSize.x - uBaseSize.x/2.f + cubeSizeHalf.x, 0.0, float(row-1)/uGridSize.y * uBaseSize.y - uBaseSize.y/2.f);
            vec2 uv;
            uv = vec2(float(col-2) / uGridSize.x, float(row-1-0.5) / uGridSize.y);
            float height2 = textureBicubic(uHeightMap, uv).r * uHeightScale;
            uv = vec2(float(col-2) / uGridSize.x, float(row-1+0.5) / uGridSize.y);
            float height3 = textureBicubic(uHeightMap, uv).r * uHeightScale;
            CreateQuadWall(
                vec3(gridPosition.x, height2, gridPosition.z-cubeSizeHalf.y),
                vec3(gridPosition.x, height3, gridPosition.z+cubeSizeHalf.y),
                vec3(1,0,0)
            );
        }
        return;
    }

    // Adjust row and col for non-edge cubes
    row--;
    col--;

    // Calculate world position for this grid cell
    vec3 gridPosition = vec3(float(col) / uGridSize.x * uBaseSize.x - uBaseSize.x / 2.0, 0.0,
                             float(row) / uGridSize.y * uBaseSize.y - uBaseSize.y / 2.0);

    // Sample heights from the heightmap
    vec2 uv;
    uv = vec2(float(col - 0.5) / uGridSize.x, float(row - 0.5) / uGridSize.y);
    float height1 = textureBicubic(uHeightMap, uv).r * uHeightScale;
    uv = vec2(float(col + 0.5) / uGridSize.x, float(row - 0.5) / uGridSize.y);
    float height2 = textureBicubic(uHeightMap, uv).r * uHeightScale;
    uv = vec2(float(col + 0.5) / uGridSize.x, float(row + 0.5) / uGridSize.y);
    float height3 = textureBicubic(uHeightMap, uv).r * uHeightScale;
    uv = vec2(float(col - 0.5) / uGridSize.x, float(row + 0.5) / uGridSize.y);
    float height4 = textureBicubic(uHeightMap, uv).r * uHeightScale;

    // Compute normals for the two triangles forming the top face
    vec3 p1 = vec3(float(col - 0.5) / uGridSize.x, float(row - 0.5) / uGridSize.y, height1);
    vec3 p2 = vec3(float(col + 0.5) / uGridSize.x, float(row - 0.5) / uGridSize.y, height2);
    vec3 p3 = vec3(float(col + 0.5) / uGridSize.x, float(row + 0.5) / uGridSize.y, height3);
    vec3 p4 = vec3(float(col - 0.5) / uGridSize.x, float(row + 0.5) / uGridSize.y, height4);

    vec3 norm1 = normalize(cross(p3 - p1, p2 - p1));
    vec3 norm2 = normalize(cross(p4 - p1, p3 - p1));

    // Construct the top face quad
    CreateQuad(gridPosition, vec4(height1, height2, height3, height4), norm1, norm2);
}
