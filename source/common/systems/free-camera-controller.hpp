#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/free-camera-controller.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include "../components/wall.hpp"
#include "../components/zwall.hpp"
#include "../components/car.hpp"

#define COLLIDED_WITH_XWALL 1
#define COLLIDED_WITH_ZWALL -1
#define NO_COLLISION 0

namespace our
{

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/components/free-camera-controller.hpp"
    class FreeCameraControllerSystem
    {
        Application *app;          // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked
        bool iscolide;

    public:
        bool effect_flag = false;
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app)
        {
            this->app = app;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent
        void update(World *world, float deltaTime) {
            // // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // // As soon as we find one, we break
            // CameraComponent* camera = nullptr;
            // FreeCameraControllerComponent *controller = nullptr;
            // for(auto entity : world->getEntities()){
            //     camera = entity->getComponent<CameraComponent>();
            //     controller = entity->getComponent<FreeCameraControllerComponent>();
            //     if(camera && controller) break;
            // }
            // // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            // if(!(camera && controller)) return;
            // // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            // Entity* entity = camera->getOwner();

            // If the left mouse button is pressed, we lock and hide the mouse. This common in First Person Games.
            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && !mouse_locked) {
                app->getMouse().lockMouse(app->getWindow());
                mouse_locked = true;
                // // If the left mouse button is released, we unlock and unhide the mouse.
                // } else if(!app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && mouse_locked) {
                //     app->getMouse().unlockMouse(app->getWindow());
                //     mouse_locked = false;
                // }

                // // We get a reference to the entity's position and rotation
                // glm::vec3& position = entity->localTransform.position;
                // glm::vec3& rotation = entity->localTransform.rotation;

                // // If the left mouse button is pressed, we get the change in the mouse location
                // // and use it to update the camera rotation
                // if(app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1)){
                //     glm::vec2 delta = app->getMouse().getMouseDelta();
                //     rotation.x -= delta.y * controller->rotationSensitivity; // The y-axis controls the pitch
                //     rotation.y -= delta.x * controller->rotationSensitivity; // The x-axis controls the yaw
                // }
                // else{
                //     return;
                // }

                // // We prevent the pitch from exceeding a certain angle from the XZ plane to prevent gimbal locks
                // if(rotation.x < -glm::half_pi<float>() * 0.99f) rotation.x = -glm::half_pi<float>() * 0.99f;
                // if(rotation.x >  glm::half_pi<float>() * 0.99f) rotation.x  = glm::half_pi<float>() * 0.99f;
                // // This is not necessary, but whenever the rotation goes outside the 0 to 2*PI range, we wrap it back inside.
                // // This could prevent floating point error if the player rotates in single direction for an extremely long time.
                // rotation.y = glm::wrapAngle(rotation.y);

                // // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
                // glm::mat4 matrix = entity->localTransform.toMat4();

                // glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                //           up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)),
                //           right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

                // glm::vec3 current_sensitivity = controller->positionSensitivity;
                // // If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed up factor
                // if(app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT)) current_sensitivity *= controller->speedupFactor;

                // If the LEFT SHIFT key is pressed (speed up), the following condition raises a flag to apply postprocessing effect
                if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT) && app->getKeyboard().isPressed(GLFW_KEY_W)) {
                    effect_flag = true;
                }
                if (!app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT)) {
                    effect_flag = false;
                }

                // // We change the camera position based on the keys WS
                // // S & W moves the player back and forth
                // if(app->getKeyboard().isPressed(GLFW_KEY_W)) position += front * (deltaTime * (current_sensitivity.z));

                // if(app->getKeyboard().isPressed(GLFW_KEY_S)&&!iscolide) position -= front * (deltaTime * current_sensitivity.z);

                // // A & D moves the player left or right
                // if(app->getKeyboard().isPressed(GLFW_KEY_D)) position += right * (deltaTime * current_sensitivity.x);
                // if(app->getKeyboard().isPressed(GLFW_KEY_A)) position -= right * (deltaTime * current_sensitivity.x);
            }
        }
            // Collision detection handling
            int iscollide(World * World, glm::vec3 & position)
            {

                glm::vec3 carPosition;
                glm::vec3 wallPosition;
                glm::vec3 zwallPosition;

                auto entities = World->getEntities();

                for (auto entity : entities)
                {
                    if (entity->getComponent<wall>())
                    {
                        wallPosition = entity->localTransform.position;
                        if (abs(position.x - wallPosition.x) <= 6 && abs(position.z - wallPosition.z) <= 2)
                        {
                            return COLLIDED_WITH_XWALL;
                        }
                    }
                    if (entity->getComponent<zwall>())
                    {
                        zwallPosition = entity->localTransform.position;

                        if (abs(position.z - zwallPosition.z) <= 6 && abs(position.x - zwallPosition.x) <= 2)
                        {

                            return COLLIDED_WITH_ZWALL;
                        }
                    }
                }
                return NO_COLLISION;
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
        };
    }
