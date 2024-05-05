#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/car-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>
#include <fstream>
#include <iostream>

// This state shows how to use the ECS framework and deserialization.
class PlaystateEve: public our::State {

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    our::CarControllerSystem carController;

    const int speeddown_factor = 60;
    int duration_minutes, duration_seconds, clock;

    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        std::string config_path = "config/play_eve.jsonc";
        std::ifstream file_in(config_path);
        if(!file_in){
            std::cerr << "Couldn't open file: " << config_path << std::endl;
            return;
        }
        nlohmann::json app_config = nlohmann::json::parse(file_in, nullptr, true, true);
        auto& config = app_config["scene"];
        // If we have assets in the scene config, we deserialize them
        if(config.contains("assets")){
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if(config.contains("world")){
            world.deserialize(config["world"]);
        }
        // Initialize timer
        duration_minutes = 5, duration_seconds = 0, clock = speeddown_factor;
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        carController.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
    }

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        std::string remaining_time = std::to_string(duration_minutes) + ":" +
                                     ((duration_seconds >= 10) ? std::to_string(duration_seconds) : "0" + std::to_string(duration_seconds));
        getApp()->printTextCenter(remaining_time, 1, 10);
        if (duration_minutes == 0 && duration_seconds == 0) {
            getApp()->changeState("menu"); // TODO: Change this to loss state
        }
        clock = (clock - 1 + speeddown_factor) % speeddown_factor;
        if (clock == 0) duration_seconds = (duration_seconds - 1 + 60) % 60;
        if (duration_seconds == 59 && clock == 0) duration_minutes--;

        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        carController.update(&world, (float)deltaTime);
        // And finally we use the renderer system to draw the scene
        renderer.render(&world);

        // Get a reference to the keyboard object
        auto& keyboard = getApp()->getKeyboard();


        if(keyboard.justPressed(GLFW_KEY_ESCAPE)){
            // If the escape  key is pressed in this frame, go to the play state
            getApp()->changeState("menu");
        }
    }

    void onDestroy() override {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // Clear the world
        world.clear();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
};