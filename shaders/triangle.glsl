#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 uModel;
uniform mat4 uProjection;
uniform mat4 uView; 
uniform vec4 uUVRect; // (offsetX, offsetY, scaleX, scaleY)

out vec2 vTexCoord;

void main() {
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    vTexCoord = aTexCoord * uUVRect.zw + uUVRect.xy;
}

#shader fragment
#version 330 core
in vec2 vTexCoord;  
out vec4 FragColour;
uniform sampler2D uTexture; 
void main() { 
    FragColour = texture(uTexture, vTexCoord); 
} 