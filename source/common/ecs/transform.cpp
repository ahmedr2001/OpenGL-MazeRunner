#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //TODO: (Req 3) Write this function
        
        glm::mat4 IDmatrix = glm::mat4(1.0f);

        glm::mat4 S = glm::scale(IDmatrix, scale);

        glm::mat4 R = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);

        glm::mat4 T = glm::translate(IDmatrix, position);

        glm::mat4 retMatrix = T * R * S;  

        return retMatrix; 
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }
}