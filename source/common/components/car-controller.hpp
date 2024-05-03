#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp> 

namespace our {

    class CarControllerComponent : public Component {
    public:

        // The senstivity paramter defined sensitive the camera rotation & fov is to the mouse moves and wheel scrolling
        float rotationSensitivity = 0.01f; // The angle change per pixel of mouse movement
        float fovSensitivity = 0.3f; // The fov angle change per unit of mouse wheel scrolling
        glm::vec3 positionSensitivity = {3.0f, 3.0f, 3.0f}; // The unity per second of camera movement if WASD is pressed
        float speedupFactor = 2.0f; // A multiplier for the positionSensitivity if "Left Shift" is held.


        // The ID of this component type is "Free Camera Controller"
        static std::string getID() { return "Car Controller"; }

        // Reads sensitivities & speedupFactor from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}