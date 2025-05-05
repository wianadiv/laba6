#define GLEW_DLL
#define GLFW_DLL

#include <stdio.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <cmath>

#include "shader.h"
#include "Model.h"

const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

float lastX = SCR_WIDTH / 2,
lastY = SCR_HEIGHT / 2;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;

bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn),
	ypos = static_cast<float>(yposIn);

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX,
	yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

void processInput(GLFWwindow* window) {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

int main()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3.\n");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Zhambalova ASUb-23-2", NULL, NULL);

	if (!window) {
		fprintf(stderr, "ERROR: could not create window.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	GLenum ret = glewInit();
	if (GLEW_OK != ret) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(ret));
		return 1;
	}
	glewExperimental = GL_TRUE;

	Model model("Lab_3.obj");

	GLuint shader_program = createShader("vertshader.glsl", "fragshader.glsl");

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.8, 0.2, 0.7, 1);
		glEnable(GL_DEPTH_TEST); // тест глубины
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 modelMatrix = glm::mat4(1.0f);

		setUniformMat(shader_program, "projection", projection);
		setUniformMat(shader_program, "view", view);
		setUniformMat(shader_program, "model", modelMatrix);

		glUseProgram(shader_program);

		//свет
		glm::vec3 lightColor = glm::vec3(0.8f, 0.8f, 0.5f); 
		glm::vec3 lightAmbient = lightColor * glm::vec3(0.2f); 
		glm::vec3 lightDiffuse = lightColor * glm::vec3(0.8f); 
		glm::vec3 lightSpecular = lightColor; 
		glm::vec3 lightPos = glm::vec3(-3.2f, 4.0f, 4.0f);

		setUniformVec(shader_program, "light_1.Ambient", lightAmbient);
		setUniformVec(shader_program, "light_1.Diffuse", lightDiffuse);
		setUniformVec(shader_program, "light_1.Specular", lightSpecular);
		setUniformVec(shader_program, "light_1.Position", lightPos);

		//материалы
		glm::vec3 materialAmbient = glm::vec3(0.5f, 0.2f, 0.1f); 
		glm::vec3 materialDiffuse = glm::vec3(0.8f, 0.2f, 0.3f); 
		glm::vec3 materialSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
		float materialShininess = 32.0f;

		setUniformVec(shader_program, "mat_1.Ambient", materialAmbient);
		setUniformVec(shader_program, "mat_1.Diffuse", materialDiffuse);
		setUniformVec(shader_program, "mat_1.Specular", materialSpecular);
		setUniformFloat(shader_program, "mat_1.Shininess", materialShininess);

		//позиции для расчета
		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
		setUniformMat3(shader_program, "normalMatrix", normalMatrix);
		setUniformVec(shader_program, "viewPos", cameraPos);

		model.Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}
