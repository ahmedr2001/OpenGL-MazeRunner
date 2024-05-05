#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include "../components/wall.hpp"
#include "../components/zwall.hpp"
#include "../components/car.hpp"
#include "../components/movement.hpp"

#define COLLIDED_WITH_XWALL 1
#define COLLIDED_WITH_ZWALL -1
#define NO_COLLISION 0

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic. 
    // For more information, see "common/components/movement.hpp"
    class MovementSystem {
    public:

        // This should be called every frame to update all entities containing a MovementComponent. 
        void update(World* world, float deltaTime) {
            // For each entity in the world
            for(auto entity : world->getEntities()){
                // Get the movement component if it exists
                MovementComponent* movement = entity->getComponent<MovementComponent>();
                // If the movement component exists
                if(movement){

                    // We get a reference to the entity's position and rotation

                    for(int x = 0; x < 10; x++)
                    {
                        entity->localTransform.position += deltaTime * movement->linearVelocity;
                        entity->localTransform.rotation += deltaTime * movement->angularVelocity;

                        // When the scarecrow collides with a wall, it changes its motion direction
                        if(iscollide(world, entity->localTransform.position) == COLLIDED_WITH_ZWALL)
                        {
                            entity->getComponent<MovementComponent>()->linearVelocity.x *= -1;
                        }
                        if(iscollide(world, entity->localTransform.position) == COLLIDED_WITH_XWALL)
                        {
                            entity->getComponent<MovementComponent>()->linearVelocity.z *= -1;
                        }
                    }
                }

            }
        }
        // Collision detection handling
        int iscollide(World*World, glm::vec3& position){

            glm::vec3 wallPosition;
            glm::vec3 zwallPosition;
            
            auto entities = World->getEntities();
            //Loop over all walls to check if the car collided with any of them

            for(auto entity : entities)
            {
                // This wall is aligned with the x-axis
                if(entity->getComponent<wall>())
                {
                    wallPosition = entity->localTransform.position;
                    if(abs(position.x - wallPosition.x)  <= 5.7 && abs(position.z - wallPosition.z) <= 1)
                    {
                        return COLLIDED_WITH_XWALL;
                    }

                }

                // This wall is aligned with the z-axis
                if(entity->getComponent<zwall>())
                {
                    zwallPosition = entity->localTransform.position;

                    if(abs(position.z-zwallPosition.z) <= 5.7 && abs(position.x - zwallPosition.x)  <= 1)
                    {
                        return COLLIDED_WITH_ZWALL;
                    }
                    
                }
            }
            return NO_COLLISION;
        }
    };

}
