#include "Player.h"

Player::Player(GLFWwindow* w)
{
	window = w;
	controls = new Controls(window, position);
	hitBoxMin = glm::vec3(-.3f, -2.0f, -.3f);
	hitBoxMax = glm::vec3(.3f, 2.0f, .3f);
	feetMin = glm::vec3(-.2f, -.8f, -.2f);
	feetMax = glm::vec3(.2f, -.5f, .2f);
	velocity = glm::vec3(0);

	SetNextPoints();

	axes[0] = glm::vec3(1, 0, 0);
	axes[1] = glm::vec3(0, 1, 0);
	axes[2] = glm::vec3(0, 0, 1);

	edges[0] = glm::vec3(hitBoxMax.x, hitBoxMin.y, hitBoxMin.z) - hitBoxMin;
	edges[1] = glm::vec3(hitBoxMin.x, hitBoxMax.y, hitBoxMin.z) - hitBoxMin;
	edges[2] = glm::vec3(hitBoxMin.x, hitBoxMin.y, hitBoxMax.z) - hitBoxMin;

	//Set Starting position
	SetDesiredPos(glm::vec3(0, 2, 0));
	MoveToDesired();
}

void Player::UpdateFromInputs(double deltaTime, bool edit)
{
	if ((!edit && !glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE))
	{
		controls->UpdateCameraAngle(deltaTime, edit);
	}
	else if (edit){
		controls->EditModeMouseUpdate(deltaTime);
	}
	desiredPosition = position;
	SetXZVelocity(glm::vec2(0));
	float speed = inputSpeed;
	if (edit)
		speed = editInputSpeed;

	// Move forward
	glm::vec2 noY = glm::normalize(glm::vec2(controls->GetLookDirection().x, controls->GetLookDirection().z));
	noY = glm::normalize(noY);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		SetXZVelocity(noY * speed + glm::vec2(velocity.x, velocity.z));
	}

	// Move backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		SetXZVelocity(-noY * speed + glm::vec2(velocity.x, velocity.z));
	}

	noY = glm::vec2(controls->GetPlayersRight().x, controls->GetPlayersRight().z);
	noY = glm::normalize(noY);
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		SetXZVelocity(noY * speed + glm::vec2(velocity.x, velocity.z));
	}

	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		SetXZVelocity(-noY * speed + glm::vec2(velocity.x, velocity.z));
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && onGround) {
		Jump(deltaTime);
	}
	if (edit)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
		{
			SetYVelocity(-speed);
		}
		else if (glfwGetKey(window, GLFW_KEY_SPACE))
		{
			SetYVelocity(speed);
		}
		else
		{
			SetYVelocity(0);
		}
	}
	//desiredPosition = position + velocity * (float)deltaTime;
	controls->SetViewMatrix(position);
}

glm::mat4 Player::GetProjectionMatrix()
{
	return controls->GetProjectionMatrix();
}

glm::mat4 Player::GetViewMatrix()
{
	return controls->GetViewMatrix();
}

glm::vec3 Player::GetDesiredPosition()
{
	return desiredPosition;
}

glm::vec3 Player::GetHitBoxMin()
{
	return hitBoxMin;
}

glm::vec3 Player::GetHitBoxMax()
{
	return hitBoxMax;
}

glm::vec3 Player::GetFeetMin()
{
	return feetMin + desiredPosition;
}

glm::vec3 Player::GetFeetMax()
{
	return feetMax + desiredPosition;
}

glm::vec3 Player::GetPosition()
{
	return position;
}

glm::vec3 Player::GetVelocity()
{
	return velocity;
}

Controls* Player::GetControls()
{
	return controls;
}

void Player::SetOnGround()
{
	onGround = true;
}

bool Player::IsOnGround()
{
	return onGround;
}

void Player::PlayerInAir()
{
	onGround = false;
	onCeiling = false;
}

void Player::SetPlayerCeiling(bool t)
{
	onCeiling = t;
}

bool Player::isCeiling()
{
	return onCeiling;
}

void Player::Jump(double deltaTime)
{
	float time = sqrt((2 * jumpHeight) / -gravity) * 2;
	float yVel = -(jumpHeight + ((0.5f * gravity) * time * time));
	SetYVelocity(yVel);
	onGround = false;
}

void Player::adjustVelocity(glm::vec3 adjustment)
{
	velocity = velocity + adjustment;
}

void Player::SetXZVelocity(glm::vec2 adjustment)
{
	velocity = glm::vec3(adjustment.x, velocity.y, adjustment.y);
}

void Player::SetYVelocity(float y)
{
	velocity.y = y;
}

void Player::MoveToDesired()
{
	position = desiredPosition;
}

void Player::SetDesiredPos(glm::vec3 pos)
{
	desiredPosition = pos;
}

void Player::ApplyPlayerGravity(double deltaTime)
{
	glm::vec3 gravVector = glm::vec3(0, gravity * deltaTime, 0);
	adjustVelocity(gravVector);
	if (IsOnGround())
	{
		SetYVelocity(gravVector.y);
	}
	if (onCeiling == true)
	{
		SetYVelocity(gravVector.y);
	}
}

void Player::SetOnCeiling()
{
	onCeiling = true;
}

void Player::TakeDamage(float damage)
{
	health -= damage;
}

float Player::GetHP()
{
	return health;
}

glm::vec3* Player::GetAxes()
{
	return axes;
}

glm::vec3* Player::GetEdges()
{
	return edges;
}

glm::vec3 Player::GetPoint(int i)
{
	return NextPoints[i];
}

void Player::SetNextPoints()
{
	NextPoints[0] = glm::vec3(hitBoxMin.x, hitBoxMin.y, hitBoxMin.z);
	NextPoints[1] = glm::vec3(hitBoxMin.x, hitBoxMin.y, hitBoxMax.z);
	NextPoints[2] = glm::vec3(hitBoxMin.x, hitBoxMax.y, hitBoxMin.z);
	NextPoints[3] = glm::vec3(hitBoxMin.x, hitBoxMax.y, hitBoxMax.z);
	NextPoints[4] = glm::vec3(hitBoxMax.x, hitBoxMin.y, hitBoxMin.z);
	NextPoints[5] = glm::vec3(hitBoxMax.x, hitBoxMin.y, hitBoxMax.z);
	NextPoints[6] = glm::vec3(hitBoxMax.x, hitBoxMax.y, hitBoxMin.z);
	NextPoints[7] = glm::vec3(hitBoxMax.x, hitBoxMax.y, hitBoxMax.z);
}
