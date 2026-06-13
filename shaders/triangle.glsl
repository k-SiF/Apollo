#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
uniform vec2 uOffset;
void main() {
    gl_Position = vec4(aPos.xy + uOffset, aPos.z, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColour;
void main() { FragColour = vec4(1.0, 0.5, 0.2, 1.0); } 