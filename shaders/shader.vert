#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 mv;
uniform mat4 mvp;

uniform vec3 lightPos;

out vec3 fragPos;
out vec3 normal;
out vec3 viewLightPos;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
    fragPos = vec3(mv * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(mv))) * aNormal;
    viewLightPos = vec3(view * vec4(lightPos, 1.0));
}