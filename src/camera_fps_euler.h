#ifndef CAMERA_FPS_EULER_H
#define CAMERA_FPS_EULER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Default camera values.
static const float YAW = -90.0f;
static const float PITCH = .0f;
static const float SPEED = 2.5f;
static const float SENSITIVITY = .1f;
static const float ZOOM = 45.0f;

class CameraFPS_euler {
private:

public:
    // Defines several possible options for camera movement.
    // Used as abstraction to stay away from window-system specific input methods
    enum CameraMoveMent {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
    };

    // constructor with vectors.
    CameraFPS_euler(glm::vec3 position_ = glm::vec3(.0f, .0f, .0f),
                    glm::vec3 up_ = glm::vec3(.0f, 1.0f, .0f),
                    float yaw_ = YAW, float pitch_ = PITCH)
        : front(glm::vec3(.0f, .0f, -1.0f)),
        movement_speed(SPEED),
        mouse_sensitivity(SENSITIVITY),
        zoom(ZOOM)
    {
        position = position_;
        world_up = up_;
        yaw = yaw_;
        pitch = pitch_;
        UpdateCameraVectors();
    }

    // constructor with scalar values
    CameraFPS_euler(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch):
        CameraFPS_euler(glm::vec3(posX, posY, posZ), glm::vec3(upX, upY, upZ), yaw, pitch) {}

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix.
    glm::mat4 GetViewMatrix() {
        return glm::lookAt(position, position + front, up);
    }

    // processes input received from any keyboard-like input system.
    // Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(CameraMoveMent direction, float delta_time) {
        const float velocity = movement_speed * delta_time;
        if(FORWARD == direction) {
            position += front * velocity;
        }
        if(BACKWARD == direction) {
            position -= front * velocity;
        }
        if(LEFT == direction) {
            position -= right * velocity;
        }
        if(RIGHT == direction) {
            position += right * velocity;
        }
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrain_pitch = true) {
        xoffset *= mouse_sensitivity;
        yoffset *= mouse_sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped.
        if(constrain_pitch) {
            const float max = 89.0f, min = -89.0f;
            if(max < pitch) {
                pitch = max;
            }
            if(min > pitch) {
                pitch = min;
            }
        }

        // update Front, Right and Up Vectors using the updated Euler angles.
        UpdateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset) {
        zoom -= yoffset;
        const float max = 90.0, min = 1.0f;
        if(min > zoom) {
            zoom = min;
        }
        if(max < zoom) {
            zoom = max;
        }
    }

    inline float GetZoom() const  { return zoom; }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles.
    void UpdateCameraVectors() {
        // calculate the new Front vector.
        glm::vec3 fro;
        fro.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        fro.y = sin(glm::radians(pitch));
        fro.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(fro);
        // also re-calcuate the Right and Up vector.
        //      normalize the vectors, because their length gets closer to 0 the more you look up
        //      or down which results in slower movement.
        right = glm::normalize(glm::cross(front, world_up));
        up = glm::normalize(glm::cross(right, front));
    }


    // camera attributes.
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;
    // euler angles
    float yaw;
    float pitch;
    // euler options.
    float movement_speed;
    float mouse_sensitivity;
    float zoom;
};


#endif // CAMERA_FPS_EULER_H
