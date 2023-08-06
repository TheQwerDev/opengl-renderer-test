#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 mv;
uniform mat4 mvp;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoords;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
    fragPos = vec3(mv * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(mv))) * aNormal;
    texCoords = aTexCoords;
}