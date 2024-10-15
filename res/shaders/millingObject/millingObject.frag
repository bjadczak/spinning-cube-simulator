#version 400 core
out vec4 fragColor;

in vec3 normal;
in vec3 fragPos;

uniform vec3 viewPos;

struct DirLight {
    vec3 direction;
    vec3 color;
};

vec3 calculateLight(DirLight light, vec3 N, vec3 fragPos, vec3 viewDir) {

    vec3 lightDir = normalize(light.direction);
    // diffuse shading
    float diff = max(dot(N, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = max(normalize(lightDir + viewDir), 0);

    // attenuation
    //    float distance    = length(light.position - fragPos);
    //    float attenuation = 1.0 / ((distance * distance)* light.quadraticAttenuation + distance*light.linearAttenuation + light.constantAttenuation);

    //float spec = material.shininess != 0 ? pow(max(dot(N, halfwayDir), 0.0), material.shininess): 0;
    float spec = pow(max(dot(N, halfwayDir), 0.0), 256);
    // combine results
    vec3 ambient  = light.color * vec3(0.2f);
    vec3 diffuse  = light.color * diff;
    vec3 specular = light.color * spec;
    return ambient + (diffuse + specular);// * light.strength * attenuation;
}

void main() {

    vec3 normNormal = normalize(normal);

    vec4 objectColor = vec4(1,1,1,1);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 result = vec3(0);

    DirLight light = DirLight(vec3(1,1,1), vec3(1,1,1));
    result += calculateLight(light, normNormal, fragPos, viewDir);
    result *= objectColor.rgb;

    result =  pow(result, vec3(1.0/2.2f));
    fragColor = vec4(result, objectColor.a);
}