#shader vertex
#version 330 core
layout (location = 0) in vec2 aPos;       // pixel position
layout (location = 1) in vec2 aTexCoord;  // atlas UV

uniform mat4 uProjection;   // screen-space ortho (0..width, 0..height)

out vec2 vTexCoord;

void main() {
    gl_Position = uProjection * vec4(aPos, 0.0, 1.0);
    vTexCoord = aTexCoord;
}

#shader fragment
#version 330 core
in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uText;    // the single-channel glyph atlas
uniform vec3 uColor;        // text color

void main() {
    // the atlas stores glyph coverage in the RED channel; use it as alpha
    float alpha = texture(uText, vTexCoord).r;
    FragColor = vec4(uColor, alpha);
}
