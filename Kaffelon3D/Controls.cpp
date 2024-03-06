#include "Controls.h"

float mouseWheel = 0.0f;
float mouseRate = .5f;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	mouseWheel += yoffset * mouseRate;
}

Controls::Controls(GLFWwindow* pwind, glm::vec3 position)
{
	lastXPos = windowDimensions.x / 2;
	lastYPos = height / 2;
	pWindow = pwind;
	glfwGetWindowSize(pWindow, &width, &height);
	mouseWheel = 0.0f;
	glfwSetCursorPos(pWindow, width / 2, height / 2);
	verticalAngle = 0.0f;
	horizontalAngle = 3.14f;
	mouseVangle = verticalAngle;
	mouseHAngle = horizontalAngle;
	// Direction : Spherical coordinates to Cartesian coordinates conversion
	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle));

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);
	up = glm::cross(right, direction);
	ViewMatrix = glm::lookAt(position, position + direction, up);
	ProjectionMatrix = glm::mat4(1) * glm::perspective(initialFoV * 3.14f / 180.0f, 4.0f / 3.0f, 0.1f, 100.0f);

}

Controls::~Controls()
{
}


void Controls::UpdateCameraAngle(double deltaTime, bool isEdit)
{

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(pWindow, &xpos, &ypos);
	glfwSetCursorPos(pWindow, width / 2, height / 2);


	// Compute new orientation
	deltaHAngle = mouseSpeed * deltaTime * float(width / 2 - xpos);
	deltaVAngle = mouseSpeed * deltaTime * float(height / 2 - ypos);

	horizontalAngle += deltaHAngle;
	verticalAngle += deltaVAngle;
	// Lock camera to top and bottom
	if (verticalAngle > 3.14/2)
	{
		verticalAngle = 3.14 / 2;
	}
	else if (verticalAngle < -3.14 / 2)
	{
		verticalAngle = -3.14 / 2;
	}
	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle));
	right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f));

	glfwSetScrollCallback(pWindow, scroll_callback);
	float FoV = initialFoV - 5 * mouseWheel;
		
}

void Controls::EditModeMouseUpdate(double deltaTime)
{
	double xpos, ypos;
	glfwGetCursorPos(pWindow, &xpos, &ypos);
	deltaHAngle = mouseSpeed * deltaTime * ((float)(width / 2 - xpos) - lastXPos);
	deltaVAngle = mouseSpeed * deltaTime * ((float)(height / 2 - ypos) - lastYPos);
	lastXPos = width / 2 - xpos;
	lastYPos = height / 2 - ypos;
	// Lock camera to top and bottom
	if (verticalAngle > 3.14 / 2)
	{
		verticalAngle = 3.14 / 2;
	}
	else if (verticalAngle < -3.14 / 2)
	{
		verticalAngle = -3.14 / 2;
	}
	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle));
	right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f));
}

void Controls::SetViewMatrix(glm::vec3 newPos)
{
	up = glm::cross(right, direction);
	ViewMatrix = glm::lookAt(
		newPos,           // Camera is here
		newPos + direction, // and looks here : at the same position, plus "direction"
		up
	// Head is up (set to 0,-1,0 to look upside-down)
	);
}


glm::mat4 Controls::GetProjectionMatrix()
{
	return ProjectionMatrix;
}

glm::mat4 Controls::GetViewMatrix()
{
	return ViewMatrix;
}

//float Controls::GetHorizontalAngle()
//{
//	return horizontalAngle;
//}

//float Controls::GetVerticalAngle()
//{
//	return verticalAngle;
//}

glm::vec3 Controls::GetLookDirection()
{
	return direction;
}

glm::vec3 Controls::GetPlayersRight()
{
	return right;
}
//
//glm::vec3 Controls::GetUp()
//{
//	return up;
//}

float Controls::GetDeltaHAngle()
{
	return deltaHAngle;
}

float Controls::GetDeltaVAngle()
{
	return deltaVAngle;
}

//std::string Controls::GetConsoleCommand()
//{
//	using namespace std::literals;
//	std::string s = ""s;
//	if (std::cin >> s)
//		return s;
//
//	return s;
//}

