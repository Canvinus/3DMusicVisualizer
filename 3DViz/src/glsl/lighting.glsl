#version 400 core

// The ambient light parameters
struct AmbientLight {
    vec3 color;
};

// The directional light parameters
struct DirectionalLight {
    mat4 matrix;
    vec3 color;
    vec3 direction;
};

// The point light parameters
struct PointLight {
    mat4 matrix;
    vec3 color;
    vec3 position;
    float attenuation;
    bool enabled;
};

// The spot light parameters
struct SpotLight {
    mat4 matrix;
    vec3 color;
    vec3 position;
    vec3 direction;
    float innerAngleCos;
    float outerAngleCos;
    float attenuation;
    bool enabled;
};

// The return parameters of the calculatePointLightColor and calculateSpotLightColor functions
// This helps to increase the performance by sharing calculations for the light and specular colors
struct Colors {
    vec3 light;
    vec3 specular;
};

// Calculates the directional light color
vec3 calculateDirectionalLightColor(DirectionalLight directionalLight, vec3 normal) {
    float lighting = max(-dot(directionalLight.direction, normal), 0.0);
    return lighting * directionalLight.color;
}

// Calculates the point light and specular colors
Colors calculatePointLightColor(PointLight pointLight, vec3 cameraPosition, vec3 normal, vec3 position, float specular, float shininess) {
    // These values can be calculated in the vertex shader and interpolated to increase the performance but for simplicity they are calculated in the fragment shader
    float d = distance(pointLight.position, position);
    vec3 direction = normalize(position - pointLight.position);
    vec3 directionToCamera = normalize(cameraPosition - position);

    float intensity = 1.0 / max(pointLight.attenuation * d, 1.0);

    float lighting = intensity * max(-dot(direction, normal), 0.0);
    float specularLighting = intensity * pow(max(dot(directionToCamera, reflect(direction, normal)), 0.0), shininess); // When loading the model the shininess is set to 1 if it is less than 1

    return Colors(lighting * pointLight.color, specular * specularLighting * pointLight.color);
}

// Calculates the spot light and specular colors
Colors calculateSpotLightColor(SpotLight spotLight, vec3 cameraPosition, vec3 normal, vec3 position, float specular, float shininess) {
    // These values can be calculated in the vertex shader and interpolated to increase the performance but for simplicity they are calculated in the fragment shader
    float d = distance(spotLight.position, position);
    vec3 direction = normalize(position - spotLight.position);
    vec3 directionToCamera = normalize(cameraPosition - position);

    float spotIntensity = clamp((dot(spotLight.direction, direction) - spotLight.outerAngleCos) / (spotLight.innerAngleCos - spotLight.outerAngleCos), 0.0, 1.0);
    float intensity = spotIntensity / max(spotLight.attenuation * d, 1.0);

    float lighting = intensity * max(-dot(direction, normal), 0.0);
    float specularLighting = intensity * pow(max(dot(directionToCamera, reflect(direction, normal)), 0.0), shininess); // When loading the model the shininess is set to 1 if it is less than 1

    return Colors(lighting * spotLight.color, specular * specularLighting * spotLight.color);
}
