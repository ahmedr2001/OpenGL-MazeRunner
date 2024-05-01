#pragma once

#include "../ecs/component.hpp"
#include <glm/glm.hpp>

namespace our
{

    // An enum that defines the type of the light (directional, spot, point)
    enum class LightType
    {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    class LightComponent : public Component
    {
    public:
        LightType lightType; // the type of the light

        // Phong reflection
        glm::vec3 diffuse;  // simulates light scatters in all directions equally (Light . Normal)
        glm::vec3 specular; // simulates reflection at angle of reflection only
        glm::vec3 ambient;  // simulates light from all directions (= const number from all directions)

        glm::vec3 position;  // For Point, Spot Lights (as directional emanates from a distant source)
        glm::vec3 direction; // For Directional, Spot Lights (as point illuminate in all directions)

        glm::vec3 attenuation; // x: quadratic, y: linear, z: constant

        glm::vec2 spotAngles; // x: inner angle, y: outer angle

        // The ID of this component type is "Light"
        static std::string getID() { return "Light"; }

        // Reads light parameters from the given json object
        void deserialize(const nlohmann::json &data) override;
    };
}
