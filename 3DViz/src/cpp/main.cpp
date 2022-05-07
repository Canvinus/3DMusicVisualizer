#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>

#include "loaders/text/load_text.h"
#include "utils/opengl.h"
#include "camera.h"
#include "objects/textured_model.h"
#include "objects/skybox.h"

const float PI = glm::pi<float>();

// The field of view
const float FOV_Y = 70;

const float CAMERA_ROTATION_SPEED = 60;

const float CAMERA_MOVEMENT_SPEED_LOW = 6;
const float CAMERA_MOVEMENT_SPEED_HIGH = 60;

const int SHADOW_TEXTURE_SIZE = 2048;

float cameraMovementSpeed = CAMERA_MOVEMENT_SPEED_LOW;

bool spotLightEnabled = true;

GLFWwindow* window;

// The viewport width
int width = 800;

// The viewport height
int height = 640;

// The viewport aspect
float aspect = (float) width / (float) height;

// The mouse x coordinate
double mouseX = 0;

// The mouse y coordinate
double mouseY = 0;

// Shows whether the mouse is captured
bool mouseCaptured;

// The elapsed time since the GLFW initialization in seconds
float t;

// The elapsed time since the last frame in seconds
float dt;

struct AmbientLightUniform {
    GLuint color;
};

struct DirectionalLightUniform {
    GLuint matrix;
    GLuint color;
    GLuint direction;
};

struct PointLightUniform {
    GLuint matrix;
    GLuint color;
    GLuint position;
    GLuint attenuation;
    GLuint enabled;
};

struct SpotLightUniform {
    GLuint matrix;
    GLuint color;
    GLuint position;
    GLuint direction;
    GLuint innerAngleCos;
    GLuint outerAngleCos;
    GLuint attenuation;
    GLuint enabled;
};

// The OpenGL variables for the skybox program
GLuint skyboxProgram;
struct {
    GLuint u_forward;
    GLuint u_clipPlaneRight;
    GLuint u_clipPlaneUp;
    GLuint u_texture;
} skyboxUniforms;

// The OpenGL variables for the texture program
GLuint textureProgram;
struct {
    GLuint u_modelMatrix;
    GLuint u_normalMatrix;
    GLuint u_matrix;
    GLuint u_cameraPosition;
    GLuint u_specular;
    GLuint u_shininess;
    AmbientLightUniform u_ambientLight;
    DirectionalLightUniform u_directionalLight;
    PointLightUniform u_pointLight;
    SpotLightUniform u_spotLight;
    GLuint u_texture;
    GLuint u_directionalLightTexture;
    GLuint u_pointLightTexture;
    GLuint u_spotLightTexture;
} textureUniforms;

// The OpenGL variables for the font program
GLuint fontProgram;
struct {
    GLuint u_textColor;
    GLuint u_projectionMatrix;
} fontUniforms;

// The OpenGL variables for the shadow program
GLuint shadowProgram;
struct {
    GLuint u_matrix;
} shadowUniforms;
GLuint shadowFramebuffer;
GLuint directionalLightTexture;
GLuint pointLightTexture;
GLuint spotLightTexture;

Camera directionalLightCamera;
Camera pointLightCamera;

Camera camera(0, 2, 10);

glm::mat4 projectionViewMatrix;

Skybox skybox("../res/image/skybox/right.png",
              "../res/image/skybox/left.png",
              "../res/image/skybox/up.png",
              "../res/image/skybox/down.png",
              "../res/image/skybox/front.png",
              "../res/image/skybox/back.png");

TexturedModel plane("../res/model/plane/plane.obj");

TexturedModel tetrahedron("../res/model/tetrahedron/tetrahedron.obj");

TexturedModel cube("../res/model/cube/cube.obj");

TexturedModel cube2("../res/model/cube/cube.obj");

TexturedModel cube3("../res/model/cube/cube.obj");

TexturedModel cube4("../res/model/cube/cube.obj");

std::vector<TexturedModel*> objects;
std::vector<TexturedModel> models;
void initializeScene();

void initializeSkyboxProgram();

void initializeFontProgram();

void initializeTextureProgram(GLuint lightingShader);

void initializeShadowProgram();

void resizeViewport();

void update();

void updateControls();

void updateProjectionViewMatrix();

void updateObjects();

void rotateObject(Object3D& object, glm::vec3 angularVelocity);

void draw();

void drawShadows(glm::mat4 lightMatrix, GLuint lightTexture);

void glfwErrorCallback(int error, const char* description) {
    std::cerr << "The GLFW function call completed with errors:\n" << description << std::endl;
}

void glfwCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (mouseCaptured) {
        camera.rotateHorizontally((xpos - mouseX) * glm::radians(0.1f));
        camera.rotateVertically(-(ypos - mouseY) * glm::radians(0.1f));

        mouseX = xpos;
        mouseY = ypos;
    }
}

void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        mouseCaptured = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwGetCursorPos(window, &mouseX, &mouseY);
    }
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        mouseCaptured = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) {
        if (cameraMovementSpeed == CAMERA_MOVEMENT_SPEED_LOW) cameraMovementSpeed = CAMERA_MOVEMENT_SPEED_HIGH;
        else cameraMovementSpeed = CAMERA_MOVEMENT_SPEED_LOW;
    }
    else if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        spotLightEnabled = !spotLightEnabled;
    }
}

int main() {
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit()) return -1;

    // Set the OpenGL version window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Set the OpenGL context window hints (required for macOS)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Task 6", nullptr, nullptr);

    // Close the application if the window isn't created
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Set the callbacks for controls
    glfwSetCursorPosCallback(window, glfwCursorPosCallback);
    glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
    glfwSetKeyCallback(window, glfwKeyCallback);

    // Enable the raw mouse motion for the camera control
    if (glfwRawMouseMotionSupported()) glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    else std::cout << "Raw mouse motion isn't supported" << std::endl;

    glfwMakeContextCurrent(window);

    // Limit the frame rate to a screen refresh rate
    glfwSwapInterval(1);

    GLenum glewStatus = glewInit();

    if (glewStatus != GLEW_OK) {
        std::cerr << "The GLEW initialization completed with errors:\n" << glewGetErrorString(glewStatus) << std::endl;
        glfwTerminate();
        return -1;
    }

    try {
        initializeScene();
    }
    catch (const std::exception& e) {
        // Close the application if there is an error while loading the files, compiling the shaders or linking the program
        std::cerr << e.what() << std::endl;
        glfwTerminate();
        return -1;
    }

    // Get the initial time
    t = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        // Don't update and draw the scene if the program window is minimized
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED)) {
            glfwWaitEvents();
            continue;
        }

        // Calculate the elapsed time
        float nt = glfwGetTime();
        dt = nt - t;
        t = nt;


        resizeViewport();
        update();
        draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void initializeScene() {
    std::string lightingShaderSource = loadText("../src/glsl/lighting.glsl");

    GLuint lightingShader = createShader(lightingShaderSource, GL_FRAGMENT_SHADER);

    initializeSkyboxProgram();
    initializeFontProgram();
    initializeTextureProgram(lightingShader);
    initializeShadowProgram();

    directionalLightCamera.position = glm::vec3(0, 10, 0);
    directionalLightCamera.rotateHorizontally(glm::radians(-45.0f));
    directionalLightCamera.rotateVertically(glm::radians(-60.0f));
    directionalLightCamera.updateDirection();

    pointLightCamera.position = glm::vec3(0, 10, -10);
    pointLightCamera.rotateHorizontally(glm::radians(180.0f));
    pointLightCamera.rotateVertically(glm::radians(-45.0f));
    pointLightCamera.updateDirection();

    skybox.initialize();

    plane.initialize();
    plane.scale = glm::vec3(50, 1, 50);

    tetrahedron.initialize();
    tetrahedron.position = glm::vec3(-5, 2, 0);

    cube.initialize();
    cube.position = glm::vec3(5, 2, 0);

    cube2.initialize();
    cube2.position = glm::vec3(0, 2, 0);

	cube3.initialize();
	cube3.position = glm::vec3(10, 2, 0);

	objects.push_back(&plane);
	objects.push_back(&tetrahedron);
	objects.push_back(&cube);
	objects.push_back(&cube2);
	objects.push_back(&cube3);

/*	GLuint y_init = 2;
	for(int i = 0; i < 5; i++){
		models.push_back(TexturedModel("../res/model/cube/cube.obj"));

		models[i].initialize();
		models[i].position = glm::vec3(11, y_init++, 0);

		objects.push_back(&models[i]);
	}

	for(int i = 0; i < 5; i++){
		std::cout << models[i].position.x << " " << models[i].position.y << " " << models[i].position.z << std::endl;
	}*/
}

void initializeSkyboxProgram() {
    std::string vertexShaderSource = loadText("../src/glsl/skybox-vertex.glsl");
    std::string fragmentShaderSource = loadText("../src/glsl/skybox-fragment.glsl");

    GLuint vertexShader = createShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    skyboxProgram = createProgram(vertexShader, fragmentShader);

    skyboxUniforms.u_forward = glGetUniformLocation(skyboxProgram, "u_forward");
    skyboxUniforms.u_clipPlaneRight = glGetUniformLocation(skyboxProgram, "u_clipPlaneRight");
    skyboxUniforms.u_clipPlaneUp = glGetUniformLocation(skyboxProgram, "u_clipPlaneUp");
    skyboxUniforms.u_texture = glGetUniformLocation(skyboxProgram, "u_texture");

    glUseProgram(skyboxProgram);

    glUniform1i(skyboxUniforms.u_texture, 0);
}

void initializeFontProgram() {
    std::string vertexShaderSource = loadText("../src/glsl/font-vertex.glsl");
    std::string fragmentShaderSource = loadText("../src/glsl/font-fragment.glsl");

    GLuint vertexShader = createShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    fontProgram = createProgram(vertexShader, fragmentShader);

    fontUniforms.u_textColor = glGetUniformLocation(fontProgram, "u_textColor");
    fontUniforms.u_projectionMatrix = glGetUniformLocation(fontProgram, "u_projectionMatrix");

    glUseProgram(fontProgram);

    glm::mat4 projectionMatrix = glm::ortho(0.0f, (float) width, 0.0f, (float) height);
    glUniformMatrix4fv(fontUniforms.u_projectionMatrix, 1, GL_FALSE, (GLfloat*) &projectionMatrix);
}

void initializeTextureProgram(GLuint lightingShader) {
    std::string vertexShaderSource = loadText("../src/glsl/texture-vertex.glsl");
    std::string fragmentShaderSource = loadText("../src/glsl/texture-fragment.glsl");

    GLuint vertexShader = createShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    textureProgram = createProgram({ vertexShader, fragmentShader, lightingShader });

    textureUniforms.u_modelMatrix = glGetUniformLocation(textureProgram, "u_modelMatrix");
    textureUniforms.u_normalMatrix = glGetUniformLocation(textureProgram, "u_normalMatrix");
    textureUniforms.u_matrix = glGetUniformLocation(textureProgram, "u_matrix");
    textureUniforms.u_cameraPosition = glGetUniformLocation(textureProgram, "u_cameraPosition");

    textureUniforms.u_specular = glGetUniformLocation(textureProgram, "u_specular");
    textureUniforms.u_shininess = glGetUniformLocation(textureProgram, "u_shininess");

    TexturedModel::u_specular = textureUniforms.u_specular;
    TexturedModel::u_shininess = textureUniforms.u_shininess;

    textureUniforms.u_ambientLight.color = glGetUniformLocation(textureProgram, "u_ambientLight.color");

    textureUniforms.u_directionalLight.matrix = glGetUniformLocation(textureProgram, "u_directionalLight.matrix");
    textureUniforms.u_directionalLight.color = glGetUniformLocation(textureProgram, "u_directionalLight.color");
    textureUniforms.u_directionalLight.direction = glGetUniformLocation(textureProgram, "u_directionalLight.direction");

    textureUniforms.u_pointLight.matrix = glGetUniformLocation(textureProgram, "u_pointLight.matrix");
    textureUniforms.u_pointLight.color = glGetUniformLocation(textureProgram, "u_pointLight.color");
    textureUniforms.u_pointLight.position = glGetUniformLocation(textureProgram, "u_pointLight.position");
    textureUniforms.u_pointLight.attenuation = glGetUniformLocation(textureProgram, "u_pointLight.attenuation");
    textureUniforms.u_pointLight.enabled = glGetUniformLocation(textureProgram, "u_pointLight.enabled");

    textureUniforms.u_spotLight.matrix = glGetUniformLocation(textureProgram, "u_spotLight.matrix");
    textureUniforms.u_spotLight.color = glGetUniformLocation(textureProgram, "u_spotLight.color");
    textureUniforms.u_spotLight.position = glGetUniformLocation(textureProgram, "u_spotLight.position");
    textureUniforms.u_spotLight.direction = glGetUniformLocation(textureProgram, "u_spotLight.direction");
    textureUniforms.u_spotLight.innerAngleCos = glGetUniformLocation(textureProgram, "u_spotLight.innerAngleCos");
    textureUniforms.u_spotLight.outerAngleCos = glGetUniformLocation(textureProgram, "u_spotLight.outerAngleCos");
    textureUniforms.u_spotLight.attenuation = glGetUniformLocation(textureProgram, "u_spotLight.attenuation");
    textureUniforms.u_spotLight.enabled = glGetUniformLocation(textureProgram, "u_spotLight.enabled");

    textureUniforms.u_texture = glGetUniformLocation(textureProgram, "u_texture");

    textureUniforms.u_directionalLightTexture = glGetUniformLocation(textureProgram, "u_directionalLightTexture");
    textureUniforms.u_pointLightTexture = glGetUniformLocation(textureProgram, "u_pointLightTexture");
    textureUniforms.u_spotLightTexture = glGetUniformLocation(textureProgram, "u_spotLightTexture");

    glUseProgram(textureProgram);

    glUniform1i(textureUniforms.u_texture, 0);

    glUniform1i(textureUniforms.u_directionalLightTexture, 1);
    glUniform1i(textureUniforms.u_pointLightTexture, 2);
    glUniform1i(textureUniforms.u_spotLightTexture, 3);
}

void initializeShadowProgram() {
    std::string vertexShaderSource = loadText("../src/glsl/shadow-vertex.glsl");
    std::string fragmentShaderSource = loadText("../src/glsl/shadow-fragment.glsl");

    GLuint vertexShader = createShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    shadowProgram = createProgram(vertexShader, fragmentShader);

    shadowUniforms.u_matrix = glGetUniformLocation(shadowProgram, "u_matrix");

    // Create the framebuffer
    glGenFramebuffers(1, &shadowFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer);

    // Disable the color buffers
    glDrawBuffer(GL_NONE);

    // Create the shadow textures
    // The compare mode is required to use the sampler2DShadow type in the fragment shader and compare the fragment depth with the depth in the texture
    // If the linear filtering is turned on, neighboring pixels will be used to approximate the visibility
    glGenTextures(1, &directionalLightTexture);
    glBindTexture(GL_TEXTURE_2D, directionalLightTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_TEXTURE_SIZE, SHADOW_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glGenTextures(1, &pointLightTexture);
    glBindTexture(GL_TEXTURE_2D, pointLightTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_TEXTURE_SIZE, SHADOW_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glGenTextures(1, &spotLightTexture);
    glBindTexture(GL_TEXTURE_2D, spotLightTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_TEXTURE_SIZE, SHADOW_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
}

void resizeViewport() {
    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float) width / (float) height;
}

void update() {
    updateControls();
    updateProjectionViewMatrix();
    updateObjects();
}

void updateControls() {
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) camera.rotateHorizontally(-glm::radians(CAMERA_ROTATION_SPEED) * dt);
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) camera.rotateHorizontally(glm::radians(CAMERA_ROTATION_SPEED) * dt);

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) camera.rotateVertically(-glm::radians(CAMERA_ROTATION_SPEED) * dt);
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) camera.rotateVertically(glm::radians(CAMERA_ROTATION_SPEED) * dt);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.position -= camera.right * cameraMovementSpeed * dt;
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.position += camera.right * cameraMovementSpeed * dt;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.position -= camera.forward * cameraMovementSpeed * dt;
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.position += camera.forward * cameraMovementSpeed * dt;

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) camera.position.y -= cameraMovementSpeed * dt;
    else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) camera.position.y += cameraMovementSpeed * dt;

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) cube3.position.y += 0.5;
	else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) cube3.position.y -= 0.5;

    camera.updateDirection();
}

void updateProjectionViewMatrix() {
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(FOV_Y), aspect, 0.1f, 500.0f);
    glm::vec3 center = camera.position + camera.forward;
    glm::mat4 viewMatrix = glm::lookAt(camera.position, center, camera.up);
    projectionViewMatrix = projectionMatrix * viewMatrix;
}

void updateObjects() {
    glm::vec3 angularVelocity = glm::radians(glm::vec3(10, 10, 0));

    rotateObject(tetrahedron, angularVelocity);
    rotateObject(cube, angularVelocity);

    glm::vec3 pos = cube2.position;

    cube2.position = glm::vec3(pos.x + 0.01, pos.y, pos.z);
}

void rotateObject(Object3D& object, glm::vec3 angularVelocity) {
    object.rotation = object.rotation + angularVelocity * dt;

    object.rotation = glm::mod(object.rotation, PI * 2);
}

void draw() {
    // Create the shadow maps
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer);

    glViewport(0, 0, SHADOW_TEXTURE_SIZE, SHADOW_TEXTURE_SIZE);

    glEnable(GL_DEPTH_TEST);

    glUseProgram(shadowProgram);

    TexturedModel::setUniforms = false;

    // Create the shadow map for the directional light
    glm::mat4 directionalLightMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 20.0f)
            * glm::lookAt(directionalLightCamera.position, directionalLightCamera.position + directionalLightCamera.forward, directionalLightCamera.up);

    drawShadows(directionalLightMatrix, directionalLightTexture);

    // Create the shadow map for the point light
    glm::mat4 pointLightMatrix = glm::perspective(glm::radians(60.0f), 1.0f, 1.0f, 20.0f)
            * glm::lookAt(pointLightCamera.position, pointLightCamera.position + pointLightCamera.forward, pointLightCamera.up);

    drawShadows(pointLightMatrix, pointLightTexture);

    // Create the shadow map for the spot light
    glm::vec3 spotLightPosition = camera.position + camera.up;

    glm::mat4 spotLightMatrix = glm::perspective(glm::radians(60.0f), 1.0f, 1.0f, 20.0f)
            * glm::lookAt(spotLightPosition, spotLightPosition + camera.forward, camera.up);

    drawShadows(spotLightMatrix, spotLightTexture);

    TexturedModel::setUniforms = true;

    glDisable(GL_DEPTH_TEST);

    // Draw the scene
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(skyboxProgram);

    // Draw the skybox
    glUniform3fv(skyboxUniforms.u_forward, 1, (GLfloat*) &camera.forward);

    float tanY = std::tan(glm::radians(FOV_Y / 2));
    float tanX = tanY * aspect;

    glm::vec3 clipPlaneRight = tanX * camera.right;
    glm::vec3 clipPlaneUp = tanY * camera.up;

    glUniform3fv(skyboxUniforms.u_clipPlaneRight, 1, (GLfloat*) &clipPlaneRight);
    glUniform3fv(skyboxUniforms.u_clipPlaneUp, 1, (GLfloat*) &clipPlaneUp);

    skybox.draw();

    glEnable(GL_DEPTH_TEST);

    glm::mat4 modelMatrix;
    glm::mat4 normalMatrix;
    glm::mat4 matrix;

    glUseProgram(textureProgram);

    glUniform3fv(textureUniforms.u_cameraPosition, 1, (GLfloat*) &camera.position);

    glm::vec3 ambientLightColor(0.2, 0.2, 0.2);

    glUniform3fv(textureUniforms.u_ambientLight.color, 1, (GLfloat*) &ambientLightColor);

    glUniformMatrix4fv(textureUniforms.u_directionalLight.matrix, 1, GL_FALSE, (GLfloat*) &directionalLightMatrix);
    glUniform3f(textureUniforms.u_directionalLight.color, 0.3, 0.3, 0.3);
    glUniform3fv(textureUniforms.u_directionalLight.direction, 1, (GLfloat*) &directionalLightCamera.forward);

    glUniformMatrix4fv(textureUniforms.u_pointLight.matrix, 1, GL_FALSE, (GLfloat*) &pointLightMatrix);
    glUniform3f(textureUniforms.u_pointLight.color, 0.3, 0.3, 0.3);
    glUniform3f(textureUniforms.u_pointLight.position, 0, 10, -10);
    glUniform1f(textureUniforms.u_pointLight.attenuation, 0.1);
    glUniform1i(textureUniforms.u_pointLight.enabled, true);

    glUniformMatrix4fv(textureUniforms.u_spotLight.matrix, 1, GL_FALSE, (GLfloat*) &spotLightMatrix);
    glUniform3f(textureUniforms.u_spotLight.color, 0.2, 0.2, 0.2);
    glUniform3fv(textureUniforms.u_spotLight.position, 1, (GLfloat*) &spotLightPosition);
    glUniform3fv(textureUniforms.u_spotLight.direction, 1, (GLfloat*) &camera.forward);
    glUniform1f(textureUniforms.u_spotLight.innerAngleCos, std::cos(glm::radians(15.0f)));
    glUniform1f(textureUniforms.u_spotLight.outerAngleCos, std::cos(glm::radians(30.0f)));
    glUniform1f(textureUniforms.u_spotLight.attenuation, 0.1);
    glUniform1i(textureUniforms.u_spotLight.enabled, spotLightEnabled);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, directionalLightTexture);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, pointLightTexture);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, spotLightTexture);

    // Draw the objects
    for (TexturedModel* object : objects) {
        modelMatrix = object->getModelMatrix();
        normalMatrix = object->getNormalMatrix();
        matrix = projectionViewMatrix * modelMatrix;

        glUniformMatrix4fv(textureUniforms.u_modelMatrix, 1, GL_FALSE, (GLfloat*) &modelMatrix);
        glUniformMatrix4fv(textureUniforms.u_normalMatrix, 1, GL_FALSE, (GLfloat*) &normalMatrix);
        glUniformMatrix4fv(textureUniforms.u_matrix, 1, GL_FALSE, (GLfloat*) &matrix);

        object->draw();
    }

    glDisable(GL_DEPTH_TEST);
}

void drawShadows(glm::mat4 lightMatrix, GLuint lightTexture) {
    // Set the shadow texture as the depth buffer
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, lightTexture, 0);

    // Clear the texture
    glClear(GL_DEPTH_BUFFER_BIT);

    // Draw the objects
    for (TexturedModel* object : objects) {
        glm::mat4 matrix = lightMatrix * object->getModelMatrix();

        glUniformMatrix4fv(shadowUniforms.u_matrix, 1, GL_FALSE, (GLfloat*) &matrix);

        object->draw();
    }
}
