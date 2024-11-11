//
// Created by Bartosz Jadczak on 08/10/2024.
//

#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <imgui.h>
#include "../camera/baseCamera.h"
#include "../camera/CameraAnchorFree.h"
#include "../camera/CameraGameLike.h"
#include "../framebufferManager/FrameBufferManager.h"
#include "../objects/cube.h"
#include "../simulation/cubeSimulation.h"
#include "../simulation/cubeSimulationThreads.h"

struct AppContext {
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    std::unique_ptr<BaseCamera> camera;
    std::unique_ptr<FrameBufferManager> frameBufferManager;

    std::unique_ptr<Cube> cube;
    std::unique_ptr<CubeSimulationBase> cubeSimulation;
    float lastFrameTimeMs;

    CameraType cameraType;
    CubeSimulationType cubeSimulationType;

    bool isRunning = false;

    bool showCube = true;
    bool showGravity = true;
    bool showLine = true;
    bool showDiagonal = true;

    bool guiFocus = true;

    float cameraSensitivity = 0.8f;

    /*
     *  Window resizing callback used in inputManager
     */
    void glfw_window_resize(const unsigned int width, const unsigned int height) const {
        camera->resize(width, height); // NOLINT(*-narrowing-conversions)
        frameBufferManager->create_buffers(camera->screenWidth, camera->screenHeight);
    }

    /*
     *  Allocation of complex attribute Camera
     */
    void allocateCamera(const CameraType setType)
    {
        cameraType = setType;
        switch(setType)
        {
            case CameraType::GAMELIKE:
                if(camera != nullptr)
                    camera.reset();
            camera = std::make_unique<CameraGameLike>(1920, 1080, CameraMode::FREE, glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.f), glm::vec3(-M_PI / 4, 0, 0));
            break;
            case CameraType::FREEANCHOR:
                if(camera != nullptr)
                    camera.reset();
            camera = std::make_unique<CameraAnchorFree>(1920, 1080, CameraMode::ANCHOR, glm::vec3(0.0f, 3.0f, 3.0f), glm::vec3(0.f), glm::vec3(-M_PI / 4, 0, 0));
            break;
            default:
                throw std::invalid_argument("Invalid camera type");

        }
    }

    /*
 *  Allocation of complex attribute Camera
 */
    void allocateCubeSimulation(const CubeSimulationType setType)
    {
        cubeSimulationType = setType;
        switch(setType)
        {
            case CubeSimulationType::THREAD:
                if(cubeSimulation != nullptr)
                {
                    cubeSimulation->stopThread();
                    cubeSimulation.reset();
                }
                cubeSimulation = std::make_unique<CubeSimulationWithThreads>();
                cubeSimulation->startThread();
                break;
            case CubeSimulationType::NONTHREAD:
                if(cubeSimulation != nullptr)
                {
                    cubeSimulation->stopThread();
                    cubeSimulation.reset();
                }
                cubeSimulation = std::make_unique<CubeSimulation>();
                cubeSimulation->startThread();
                break;
            default:
                throw std::invalid_argument("Invalid simulation type");

        }
    }

    void initFrameBufferManager()
    {
        frameBufferManager = std::make_unique<FrameBufferManager>();
        frameBufferManager->create_buffers(camera->screenWidth, camera->screenHeight);
    }

    AppContext()
    {
        allocateCamera(CameraType::GAMELIKE);

    }
};

#endif //APPCONTEXT_H