#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float outer; // outer
    float inner; // cutoff
    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform Light light;
uniform Material material;
uniform vec3 u_cameraPos;

out vec4 FragColor;
void main()
{
    vec3 lightDir = normalize(light.position - FragPos);

    // ambient
    vec3 result = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // Spotlight
    float theta = dot(lightDir, normalize(-light.direction));
    if (theta > light.outer)
    {
        
        // diffuse 
        vec3 norm = normalize(Normal);
        //vec3 lightDir = normalize(-light.direction);  
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
        
        // specular
        vec3 viewDir = normalize(u_cameraPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
        
        // Attenuation
        float distance = length(light.position - FragPos);
        float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic * (distance * distance));

        if (light.inner > theta)
        {
            float intensity = clamp((theta - light.outer)/(light.inner - light.outer), 0.0, 1.0);

            attenuation *= intensity;
        }
        
        result += (diffuse + specular) * attenuation;
    }

    FragColor = vec4(result, 1.0);
} 