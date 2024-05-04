#include "light.hpp"
// #include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our
{
    // Reads lights parameters from the given json object
    void LightComponent::deserialize(const nlohmann::json &data)
    {
        printf("//////////////\n");
        printf("I am in light deserialize\n\n\n");

        if (!data.is_object())
            return;

        std::string lightTypeStr = data.value("lightType", "directional");
        this->color = data.value("color", glm::vec3(1.0f,1.0f,1.0f));

        if (lightTypeStr == "directional")
        {
            printf("//////////////\n");
            printf("Light is directional\n\n\n");
            this->lightType = LightType::DIRECTIONAL;

            this->direction = data.value("direction", glm::vec3(1.0f));
        }
        else if (lightTypeStr == "point")
        {
            printf("//////////////\n");
            printf("Light is point\n\n\n");
            this->lightType = LightType::POINT;

            this->position = data.value("position", glm::vec3(1.0f));
            this->attenuation = data.value("attenuation", glm::vec3(1.0f));
        }
        else
        {
            printf("//////////////\n");
            printf("Light is spot\n\n\n");
            this->lightType = LightType::SPOT;

            this->position = data.value("position", glm::vec3(0.0f,0.0f,0.0f));
            this->direction = data.value("direction", glm::vec3(0.0f,-1.0f,0.0f));

            this->attenuation = data.value("attenuation", glm::vec3(1.0f,1.0f,1.0f));
            this->spotAngles = data.value("cone_angle", glm::vec2(50, 60));
        }

    }
}