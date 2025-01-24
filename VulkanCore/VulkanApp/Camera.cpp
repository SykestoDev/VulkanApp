#include "Camera.h"
#include <sstream>

Camera::Camera(glm::vec3 startPosition, float startYaw, float startPitch)
    :
    Position(startPosition),
    Yaw(startYaw),
    Pitch(startPitch),
    MovementSpeed(5.0f),
    MouseSensitivity(0.1f),
    PitchLimit(89.0f) {
    UpdateCameraVectors(); // Calculate initial Front, Right, and Up vectors
}

glm::mat4 Camera::GetViewMatrix() const {
   return glm::lookAt(Position, Position + Front, Up);
}





void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD) Position += Front * velocity;
    if (direction == BACKWARD) Position -= Front * velocity;
    if (direction == LEFT) Position -= Right * velocity;
    if (direction == RIGHT) Position += Right * velocity;
    if (direction == UP) Position += Up * velocity;
    if (direction == DOWN) Position -= Up * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    // Update yaw and pitch
    Yaw += xoffset;
    Pitch += yoffset;

    // Clamp pitch to avoid flipping
    Pitch = glm::clamp(Pitch, -PitchLimit, PitchLimit);

    // Recalculate the camera vectors
    UpdateCameraVectors();
}


void Camera::UpdateCameraVectors() {
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    // Recalculate Right and Up vectors
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f); // Vulkan's flipped Y-axis
    Right = glm::normalize(glm::cross(Front, worldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}


