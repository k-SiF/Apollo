#shader vertex
#version 330 core
layout (location = 0) in vec2 aPos;       // world-space position (already transformed on CPU)
layout (location = 1) in vec2 aTexCoord;  // final UV (atlas/sheet sub-rect already applied)
layout (location = 2) in vec4 aColor;     // per-sprite tint

uniform mat4 uViewProjection;   // camera view * projection (no per-sprite model)

out vec2 vTexCoord;
out vec4 vColor;

void main() {
    gl_Position = uViewProjection * vec4(aPos, 0.0, 1.0);
    vTexCoord = aTexCoord;
    vColor = aColor;
}

#shader fragment
#version 330 core
in vec2 vTexCoord;
in vec4 vColor;
out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
    FragColor = texture(uTexture, vTexCoord) * vColor;
}
