#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
}; 

struct LightProperties {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct LightAttenuation {
    float constant;
    float linear;
    float quadratic;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    LightAttenuation attenuation;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    LightAttenuation attenuation;

    float inner; // cutoff
    float outer; // outer
};


in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform Material material;
uniform vec3 u_cameraPos;
uniform int spotLightActivated;

#define NPOINT_LIGHTS 4
uniform DirLight dirLight;
uniform PointLight pointLight[NPOINT_LIGHTS];
uniform SpotLight spotLight;

out vec4 FragColor;

vec3 GetAmbient(vec3 lightAmbient)
{
    return lightAmbient * texture(material.diffuse, TexCoords).rgb; // ambient
}

vec3 GetDiffuse(vec3 lightDiffuse, vec3 lightDir, vec3 norm)
{
    float diff = max(dot(norm, lightDir), 0.0);
    return lightDiffuse * diff * texture(material.diffuse, TexCoords).rgb;  // diffuse
}

vec3 GetSpecular(vec3 lightSpecular, vec3 lightDir, vec3 norm)
{
    vec3 viewDir = normalize(u_cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    return lightSpecular * spec * texture(material.specular, TexCoords).rgb; // specular 
}

float GetLightAngle(vec3 lightDir, vec3 direction)
{
    return dot(lightDir, normalize(direction));
}

float GetSpotlightIntensity(SpotLight light, float theta)
{
    return clamp((theta - light.outer)/(light.inner - light.outer), 0.0, 1.0);
}

float GetAttenuation(vec3 position, LightAttenuation attenuation)
{
    float distance = length(position - FragPos);
    return 1.0/(attenuation.constant + attenuation.linear * distance + attenuation.quadratic * (distance * distance)); 
}

vec3 GetPointLight(PointLight light, vec3 norm)
{
    vec3 lightDir = normalize(light.position - FragPos);
    // vec3 norm = normalize(Normal);

    float attenuation = GetAttenuation(light.position, light.attenuation);

    vec3 ambient = GetAmbient(light.ambient);
    vec3 diffuse =  GetDiffuse(light.diffuse, lightDir, norm);
    vec3 specular = GetSpecular(light.specular, lightDir, norm);
    
    return  (ambient + diffuse + specular) * attenuation;
}

vec3 GetDirLight(DirLight light, vec3 norm)
{
    vec3 lightDir = normalize(-light.direction);
    // vec3 norm = normalize(Normal);

    vec3 ambient = GetAmbient(light.ambient);
    vec3 diffuse =  GetDiffuse(light.diffuse, lightDir, norm);
    vec3 specular = GetSpecular(light.specular, lightDir, norm);
    return  ambient + diffuse + specular;
}

vec3 GetSpotLight(SpotLight light, vec3 norm)
{
    vec3 result = vec3(0.0); 
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));

    if (theta > light.outer)
    {
        float attenuation = 1.0;
        float intensity = 1.0;
        attenuation = GetAttenuation(light.position, light.attenuation);
        vec3 diffuse =  GetDiffuse(light.diffuse, lightDir, norm);
        vec3 specular = GetSpecular(light.specular, lightDir, norm);

        if (light.inner > theta) {
            intensity = GetSpotlightIntensity(light, theta);
        }

        result = (diffuse + specular) * attenuation * intensity;
    }

    return result;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 result;
    //result = GetDirLight(dirLight, norm);
    
    // for (int i =0; i < NPOINT_LIGHTS; i++)
    // {
    //     result += GetPointLight(pointLight[i], norm);
    // }
    if (spotLightActivated == 1)
    {
        result += GetSpotLight(spotLight, norm);
    }

    FragColor = vec4(result, 1.0);
} 