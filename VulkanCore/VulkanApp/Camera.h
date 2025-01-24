#pragma once

#ifndef CAMERA_H
#define CAMERA_H

//#include <glm/glm.hpp>
#include <gtc/quaternion.hpp>
//#include <gtc/matrix_transform.hpp>
#include <gtc/constants.hpp>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    ROLL_LEFT,
    ROLL_RIGHT
};

class Camera {
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;

    // Orientation Angles
    float Yaw;   // Horizontal rotation
    float Pitch; // Vertical rotation

    // Settings
    float MovementSpeed;
    float MouseSensitivity;
    float PitchLimit;

    // Constructor
    Camera(glm::vec3 startPosition, float startYaw = -90.0f, float startPitch = 0.0f);

    // Camera Controls
    void ProcessKeyboard(CameraMovement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset);

    glm::mat4 GetViewMatrix() const;

private:
    void UpdateCameraVectors(); // Updates Front, Right, and Up vectors
};

#endif




