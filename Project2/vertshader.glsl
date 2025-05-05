#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;
out vec3 viewPos;

uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform mat4 model;

void main() {
Normal = normalize(normalMatrix * aNormal);
FragPos = vec3(model * vec4(aPos, 1.0));
gl_Position = projection * view * model * vec4(aPos, 1.0);
}