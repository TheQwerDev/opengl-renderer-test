#version 330 core
out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

in vec3 fragPos;
in vec3 normal;

in vec3 viewLightPos;

void main()
{
    //ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //diffuse lighting
    vec3 normDir = normalize(normal);
    vec3 lightDir = normalize(viewLightPos - fragPos);

    float diff = max(dot(normDir, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //specular lighting
    float specularStrength = 0.5;

    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, normDir);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    //final lighting result
    vec3 result = (ambient + diffuse + specular) * objectColor;
    fragColor = vec4(result, 1.0);
}