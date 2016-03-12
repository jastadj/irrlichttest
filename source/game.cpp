#include "game.hpp"
#include <iostream>
#include <string>

//irrlicht namespaces
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

Game::Game()
{

}

Game::~Game()
{
    //destroy device
    device->drop();
}

void Game::start()
{
    //init irrlicht
    if(!initIrrlicht()) return;

    //start main loop
    mainLoop();
}

bool Game::initIrrlicht()
{
    //init device
    device = createDevice( video::EDT_OPENGL, dimension2d<u32>(800, 600), 16, false, false, false, &receiver);
    if(!device) {std::cout << "Error creating device.\n";return false;}
    device->setWindowCaption(L"Game");

    //get video
    driver = device->getVideoDriver();
    //get scene manager
    smgr = device->getSceneManager();
    //get gui
    guienv = device->getGUIEnvironment();

    return true;
}

int Game::mainLoop()
{
    cameraPos = vector3df(0,0, 30);
    cameraTarget = vector3df(0,30,0);


    //add camera to scene
    camera = smgr->addCameraSceneNode(0, cameraPos, cameraTarget);

    //add some fog
    driver->setFog(video::SColor(0,138,125,81), video::EFT_FOG_LINEAR, 250, 1000, .003f, true, false);

    //add light
    scene::ILightSceneNode* light1 = smgr->addLightSceneNode(0, vector3df(0,0,300), SColorf(0.5f, 0.5f, 0.5f, 0.f), 200.0f);
    //light1->enableCastShadow(false);

    int msize = 3;

    for(int i = 0; i < msize; i++)
    {
        for(int n = 0; n < msize; n++)
        {
            f32 cubesize = 10;
            vector3df cubescale(1,1,1);
            cubescale = cubescale * 1;
            vector3df cuberot(0,0,0);
            vector3df cubepos(cubesize*n, -cubesize*i, 0);
            f32 cubeid = -1;
            ISceneNode *mycube = smgr->addCubeSceneNode(cubesize, 0, cubeid, cubepos, cuberot, cubescale);
            if(mycube)
            {
                mycube->setMaterialTexture(0, driver->getTexture("wall.jpg"));
                mycube->setMaterialFlag(video::EMF_LIGHTING, true);
                //mycube->setMaterialFlag(video::EMF_LIGHTING, false); //no lighting
            }
        }
    }

    int lastFPS = -1;

    // In order to do framerate independent movement, we have to know
    // how long it was since the last frame
    u32 then = device->getTimer()->getTime();

    // This is the movemen speed in units per second.
    const f32 MOVEMENT_SPEED = 50.f;

    //main loop
    while(device->run())
    {
        // Work out a frame delta time.
        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
        then = now;

        if(receiver.IsKeyDown(KEY_ESCAPE))
        {
            device->closeDevice();
        }
        else if(receiver.IsKeyDown(KEY_KEY_A))
        {
            cameraPos.X += MOVEMENT_SPEED * frameDeltaTime;
            cameraTarget.X += MOVEMENT_SPEED * frameDeltaTime;
            camera->setPosition(cameraPos);
            camera->setTarget(cameraTarget);
        }
        else if(receiver.IsKeyDown(KEY_KEY_D))
        {
            cameraPos.X -= MOVEMENT_SPEED * frameDeltaTime;
            cameraTarget.X -= MOVEMENT_SPEED * frameDeltaTime;
            camera->setPosition(cameraPos);
            camera->setTarget(cameraTarget);
        }
        else if(receiver.IsKeyDown(KEY_KEY_W))
        {
            cameraPos.Y += MOVEMENT_SPEED * frameDeltaTime;
            cameraTarget.Y += MOVEMENT_SPEED * frameDeltaTime;
            camera->setPosition(cameraPos);
            camera->setTarget(cameraTarget);
        }
        else if(receiver.IsKeyDown(KEY_KEY_S))
        {
            cameraPos.Y -= MOVEMENT_SPEED * frameDeltaTime;
            cameraTarget.Y -= MOVEMENT_SPEED * frameDeltaTime;
            camera->setPosition(cameraPos);
            camera->setTarget(cameraTarget);
        }

        //clear scene
        driver->beginScene(true, true, SColor(255,100,101,140));

        //draw scene
        smgr->drawAll();

        //draw gui
        guienv->drawAll();

        //done and display
        driver->endScene();

        int fps = driver->getFPS();

        if (lastFPS != fps)
        {
            core::stringw tmp(L"Game [");
            tmp += driver->getName();
            tmp += L"] fps: ";
            tmp += fps;

            device->setWindowCaption(tmp.c_str());
            lastFPS = fps;
        }

    }

    return 0;
}
