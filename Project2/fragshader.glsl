#version 410 core
uniform vec4 ourColour;
out vec4 frag_colour;

in vec3 Normal;
in vec3 FragPos;
in vec3 viewPos;

struct lightStruct {
vec3 Position;
vec3 Ambient;
vec3 Diffuse;
vec3 Specular;
};
uniform lightStruct light_1;

struct materialStruct {
float Shininess;
vec3 Ambient;
vec3 Diffuse;
vec3 Specular;
};
uniform materialStruct mat_1;

void main() {
//Ambient
vec3 Ambient = light_1.Ambient * mat_1.Ambient;

//Diffuse
vec3 norm = normalize(Normal);
vec3 lightDir = normalize(light_1.Position - FragPos);
float diff = max(dot(norm, lightDir), 0.0);
vec3 Diffuse = light_1.Diffuse * (diff * mat_1.Diffuse);

//Specular
vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);
float ref = pow(max(dot(viewDir, reflectDir), 0.0), mat_1.Shininess);
vec3 Specular = light_1.Specular * (ref * mat_1.Specular);

vec3 result = Ambient + Diffuse + Specular;
frag_colour = vec4(result, 1.0);
}
