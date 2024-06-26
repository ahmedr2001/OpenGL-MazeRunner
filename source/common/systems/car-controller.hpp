#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/audio-controller.hpp"
#include "../components/car-controller.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include "../components/wall.hpp"
#include "../components/zwall.hpp"
#include "../components/pickup.hpp"
#include "../components/light.hpp"
#include "../components/car.hpp"
#include "../components/movement.hpp"

#define COLLIDED_WITH_PICKUP 2
#define COLLIDED_WITH_XWALL 1
#define COLLIDED_WITH_ZWALL -1
#define NO_COLLISION 0

namespace our
{

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/components/free-camera-controller.hpp"
    class CarControllerSystem
    {
        Application *app;          // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked
        our::AudioController *audioController = our::AudioController::getAudioController();

    public:
        bool iscolide;
        bool f = false;

        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app)
        {
            this->app = app;
        }

        // This should be called every frame to update all entities containing a CarControllerComponent
        void update(World *world, float deltaTime)
        {
            // First of all, we search for an entity containing both a carComponent and a carControllerComponent
            // As soon as we find one, we break

            Car *car = nullptr;
            CarControllerComponent *controller = nullptr;
            for (auto entity : world->getEntities())
            {
                car = entity->getComponent<Car>();
                controller = entity->getComponent<CarControllerComponent>();
                if (car && controller)
                    break;
            }
            // If there is no entity with both a carComponent and a carControllerComponent, we can do nothing so we return
            if (!(car && controller))
                return;
            // Get the entity that we found via getOwner of car (we could use controller->getOwner())
            Entity *entity = car->getOwner();

            // We get a reference to the entity's position and rotation
            glm::vec3 &position = entity->localTransform.position;
            glm::vec3 &rotation = entity->localTransform.rotation;

            // We prevent the pitch from exceeding a certain angle from the XZ plane to prevent gimbal locks
            if (rotation.x < -glm::half_pi<float>() * 0.99f)
                rotation.x = -glm::half_pi<float>() * 0.99f;
            if (rotation.x > glm::half_pi<float>() * 0.99f)
                rotation.x = glm::half_pi<float>() * 0.99f;
            // This is not necessary, but whenever the rotation goes outside the 0 to 2*PI range, we wrap it back inside.
            // This could prevent floating point error if the player rotates in single direction for an extremely long time.
            rotation.y = glm::wrapAngle(rotation.y);

            // In case of speed up, the following condition raises a flag to apply postprocessing effect
            float fov;
            if (app->getKeyboard().justPressed(GLFW_KEY_LEFT_SHIFT) && app->getKeyboard().isPressed(GLFW_KEY_W))
            {
                f = true;
            }
            if (app->getKeyboard().justReleased(GLFW_KEY_LEFT_SHIFT) && app->getKeyboard().isPressed(GLFW_KEY_W) && f)
            {
                f = false;
            }

            // We get the car model matrix (relative to its parent) to compute the front, up and right directions
            glm::mat4 matrix = entity->localTransform.toMat4();

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0));

            glm::vec3 current_sensitivity = controller->positionSensitivity;
            // If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed up factor
            if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT))
                current_sensitivity *= controller->speedupFactor * 1.2f;

            // We change the car position based on the keys WS
            // S & W moves the player back and forth
            if (app->getKeyboard().isPressed(GLFW_KEY_S))
                position += front * (deltaTime * (current_sensitivity.z));
            iscolide = iscollide(world, position);
            if (iscolide)
            {
                position -= front * (deltaTime * current_sensitivity.z);
                audioController->play("tom-scream", 0, 0);
            }

            if (app->getKeyboard().isPressed(GLFW_KEY_W) && !iscolide)
                position -= front * (deltaTime * current_sensitivity.z);
            iscolide = iscollide(world, position);
            if (iscolide)
            {
                position += front * (deltaTime * current_sensitivity.z);
                audioController->play("tom-scream", 0, 0);
            }

            // A & D moves the player left or right
            // if(app->getKeyboard().isPressed(GLFW_KEY_D)) rotation.y -= deltaTime* 100 * controller->rotationSensitivity;
            // if(app->getKeyboard().isPressed(GLFW_KEY_A)) rotation.y += deltaTime* 100 * controller->rotationSensitivity;

            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1))
            {
                glm::vec2 delta = app->getMouse().getMouseDelta();
                // rotation.x -= delta.y * controller->rotationSensitivity; // The y-axis controls the pitch
                rotation.y -= delta.x * controller->rotationSensitivity; // The x-axis controls the yaw
            }

            if (position.z < -105 && (-5 < position.x && position.x < 5))
                app->changeState("win");
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit()
        {
            if (mouse_locked)
            {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }

        // Collision detection handling
        int iscollide(World *World, glm::vec3 &position)
        {

            glm::vec3 wallPosition;
            glm::vec3 zwallPosition;
            glm::vec3 pickupPosition;
            glm::vec3 movementPosition;

            auto entities = World->getEntities();
            // Loop over all walls to check if the car collided with any of them

            for (auto entity : entities)
            {
                // This wall is aligned with the x-axis
                if (entity->getComponent<wall>())
                {
                    wallPosition = entity->localTransform.position;
                    if (abs(position.x - wallPosition.x) <= 5.7 && abs(position.z - wallPosition.z) <= 1)
                    {
                        return COLLIDED_WITH_XWALL;
                    }
                }

                // This wall is aligned with the z-axis
                if (entity->getComponent<zwall>())
                {
                    zwallPosition = entity->localTransform.position;

                    if (abs(position.z - zwallPosition.z) <= 5.7 && abs(position.x - zwallPosition.x) <= 1)
                    {
                        return COLLIDED_WITH_ZWALL;
                    }
                }

                // collecting pickup
                if (entity->getComponent<Pickup>())
                {
                    pickupPosition = entity->localTransform.position;

                    if (abs(position.x - pickupPosition.x) <= 1 && abs(position.z - pickupPosition.z) <= 1)
                    {
                        // return COLLIDED_WITH_PICKUP;
                        printf("DOING: Collected a pickup\n");
                        entity->deleteComponent(entity->getComponent<Pickup>());
                        if (entity->getComponent<LightComponent>())
                        {
                            printf("deleting: light component\n");
                            entity->deleteComponent(entity->getComponent<LightComponent>());
                        }
                        if (entity->getComponent<MovementComponent>())
                        {
                            printf("deleting: movement component\n");
                            entity->deleteComponent(entity->getComponent<MovementComponent>());
                        }
                        printf("DONE: Collected a pickup\n\n");
                        audioController->play("nice", 0, 0);
                        // return COLLIDED_WITH_PICKUP; // ?
                        app->addExtraTime();
                    }
                }

                if (entity->getComponent<MovementComponent>())
                {
                    movementPosition = entity->localTransform.position;

                    if (glm::distance(movementPosition, position) < 0.5)
                    {
                        printf("Collided!\n");

                        //! should update the time and delete it
                    }
                }
            }
            return NO_COLLISION;
        }
    };

}
