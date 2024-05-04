#include "light.hpp"
// #include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our
{
    // Reads lights parameters from the given json object
    void LightComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;

        std::string lightTypeStr = data.value("typeOfLight", "directional");

        if (lightTypeStr == "directional")
        {
            this->lightType = LightType::DIRECTIONAL;

            this->direction = data.value("direction", glm::vec3(1.0f));
        }
        else if (lightTypeStr == "point")
        {
            this->lightType = LightType::POINT;

            this->position = data.value("position", glm::vec3(1.0f));
            this->attenuation = data.value("attenuation", glm::vec3(1.0f));
        }
        else
        {
            this->lightType = LightType::SPOT;

            this->position = data.value("position", glm::vec3(1.0f));
            this->direction = data.value("direction", glm::vec3(1.0f));

            this->attenuation = data.value("attenuation", glm::vec3(1.0f));
            this->spotAngles = data.value("spotAngles", glm::vec2(1.0f));
        }

        this->diffuse = data.value("diffuse", glm::vec3(1.0f, 0.98f, 0.92f));
        this->specular = data.value("specular", glm::vec3(1.0f));
        this->ambient = data.value("ambient", glm::vec3(1.0f, 0.2f, 0.3f));
    }
}