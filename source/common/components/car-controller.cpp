#include "car-controller.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    void CarControllerComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;

    }
}