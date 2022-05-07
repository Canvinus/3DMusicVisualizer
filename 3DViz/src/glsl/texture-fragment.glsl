#version 400 core

struct AmbientLight {
    vec3 color;
};

struct DirectionalLight {
    mat4 matrix;
    vec3 color;
    vec3 direction;
};

struct PointLight {
    mat4 matrix;
    vec3 color;
    vec3 position;
    float attenuation;
    bool enabled;
};

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

struct Colors {
    vec3 light;
    vec3 specular;
};

vec3 calculateDirectionalLightColor(DirectionalLight directionalLight, vec3 normal);

Colors calculatePointLightColor(PointLight pointLight, vec3 cameraPosition, vec3 normal, vec3 position, float specular, float shininess);

Colors calculateSpotLightColor(SpotLight spotLight, vec3 cameraPosition, vec3 normal, vec3 position, float specular, float shininess);

uniform vec3 u_cameraPosition; // The camera position
uniform float u_specular; // The specular parameter (how much the specular lighting affects the object)
uniform float u_shininess; // The shininess parameter (how reflective the object is, opposite to roughness)
uniform AmbientLight u_ambientLight; // The ambient light parameters
uniform DirectionalLight u_directionalLight; // The directional light parameters
uniform PointLight u_pointLight; // The point light parameters
uniform SpotLight u_spotLight; // The spot light parameters
uniform sampler2D u_texture; // The texture
uniform sampler2DShadow u_directionalLightTexture; // The directional light shadow texture
uniform sampler2DShadow u_pointLightTexture; // The point light shadow texture
uniform sampler2DShadow u_spotLightTexture; // The spot light shadow texture

in vec3 v_position; // The fragment position in the world space
in vec3 v_normal; // The fragment normal for lighting
in vec2 v_textureCoordinates; // The texture coordinates

out vec4 color; // The fragment color

// Calculates the fragment visibility based on the shadow map
// If you don't see shadows, try setting the bias to zero
float calculateVisibility(vec3 position, mat4 lightMatrix, sampler2DShadow lightTexture, float bias) {
    // Calculate the fragment position in the light camera clip space
    vec4 transformedPosition = lightMatrix * vec4(position, 1.0);
    vec3 clipSpacePosition = transformedPosition.xyz / transformedPosition.w;

    // Return the full visibility if the fragment is outside of the clip space (the depth comparison may be incorrect in this case)
    if (clipSpacePosition.x < -1 || clipSpacePosition.x > 1
        || clipSpacePosition.y < -1 || clipSpacePosition.y > 1
        || clipSpacePosition.z < -1 || clipSpacePosition.z > 1) return 1;

    vec3 shadowCoordinates = clipSpacePosition / 2.0 + 0.5; // Calculate the coordinates in the light shadow texture
    shadowCoordinates.z -= bias; // Add the bias to the fragment depth to avoid the artifacts
    return texture(lightTexture, shadowCoordinates); // Compare the fragment depth with the depth stored in the texture (the value from zero to one is returned since the linear filtering is turned on)
}

void main() {
    // Make sure that the normal has a unit length after the interpolation
    vec3 normal = normalize(v_normal);

    //Calculate the light and specular colors
    vec3 lightColor = u_ambientLight.color;

    // Calculate the visibility for the directional light
    float visibility = calculateVisibility(v_position, u_directionalLight.matrix, u_directionalLightTexture, 0.05);

    lightColor += visibility * calculateDirectionalLightColor(u_directionalLight, normal);

    vec3 specularColor = vec3(0.0);

    if (u_pointLight.enabled) {
        // Calculate the visibility for the point light
        visibility = calculateVisibility(v_position, u_pointLight.matrix, u_pointLightTexture, 0.01);

        Colors colors = calculatePointLightColor(u_pointLight, u_cameraPosition, normal, v_position, u_specular, u_shininess);
        lightColor += visibility * colors.light;
        specularColor += visibility * colors.specular;
    }

    if (u_spotLight.enabled) {
        // Calculate the visibility for the spot light
        visibility = calculateVisibility(v_position, u_spotLight.matrix, u_spotLightTexture, 0.05);

        Colors colors = calculateSpotLightColor(u_spotLight, u_cameraPosition, normal, v_position, u_specular, u_shininess);
        lightColor += visibility * colors.light;
        specularColor += visibility * colors.specular;
    }

    // Set the fragment color based on the texture, light and specular colors
    color = texture(u_texture, v_textureCoordinates) * vec4(lightColor, 1.0) + vec4(specularColor, 0.0);
}
