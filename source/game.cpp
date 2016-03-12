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
    m_Device->drop();
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
    m_Device = createDevice( video::EDT_OPENGL, dimension2d<u32>(800, 600), 16, false, false, false, &m_Receiver);
    if(!m_Device) {std::cout << "Error creating device.\n";return false;}
    m_Device->setWindowCaption(L"Game");

    //get video
    m_Driver = m_Device->getVideoDriver();
    //get scene manager
    m_SMgr = m_Device->getSceneManager();
    //get gui
    m_GUIEnv = m_Device->getGUIEnvironment();

    return true;
}

void Game::updateCamera()
{
    m_Camera->setPosition(m_CameraPos);
    m_Camera->setTarget(m_CameraTarget);
}

int Game::mainLoop()
{
    //add camera to scene
    m_Camera = m_SMgr->addCameraSceneNode(0, m_CameraPos, m_CameraTarget);
    m_CameraPos = vector3df(30,30, 20);
    m_CameraTarget = vector3df(0,0,-20);
    updateCamera();
    matrix4 m;
    m.setRotationDegrees(m_Camera->getRotation());
    vector3df upv(0.0f, 0.0f, 1.0f);
    //m.transformVect(upv);
    m_Camera->setUpVector(upv);



    //m_CameraPos = vector3df(0,0,0);
    //m_CameraTarget = vector3df(0,0,0);


    //add some fog
    //m_Driver->setFog(video::SColor(0,138,125,81), video::EFT_FOG_LINEAR, 250, 1000, .003f, true, false);

    //add light
    scene::ILightSceneNode* light1 = m_SMgr->addLightSceneNode(0, vector3df(0,0,300), SColorf(0.5f, 0.5f, 0.5f, 0.f), 200.0f);
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
            vector3df cubepos(cubesize*n, cubesize*i, 0);
            f32 cubeid = -1;
            ISceneNode *mycube = m_SMgr->addCubeSceneNode(cubesize, 0, cubeid, cubepos, cuberot, cubescale);
            if(mycube)
            {
                mycube->setMaterialTexture(0, m_Driver->getTexture("wall.jpg"));
                mycube->setMaterialFlag(video::EMF_LIGHTING, true);
                //mycube->setMaterialFlag(video::EMF_LIGHTING, false); //no lighting
            }
        }
    }

    int lastFPS = -1;

    // In order to do framerate independent movement, we have to know
    // how long it was since the last frame
    u32 then = m_Device->getTimer()->getTime();

    // This is the movemen speed in units per second.
    const f32 MOVEMENT_SPEED = 50.f;





    //main loop
    while(m_Device->run())
    {
        // Work out a frame delta time.
        const u32 now = m_Device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
        then = now;

        if(m_Receiver.IsKeyDown(KEY_ESCAPE))
        {
            m_Device->closeDevice();
        }
        else if(m_Receiver.IsKeyDown(KEY_KEY_A))
        {
            f32 moveval = MOVEMENT_SPEED * frameDeltaTime;
            vector3df movecam( -moveval, moveval, 0);
            m_CameraPos += movecam;
            m_CameraTarget += movecam;
            updateCamera();
        }
        else if(m_Receiver.IsKeyDown(KEY_KEY_D))
        {
            f32 moveval = MOVEMENT_SPEED * frameDeltaTime;
            vector3df movecam( moveval, -moveval, 0);
            m_CameraPos += movecam;
            m_CameraTarget += movecam;
            updateCamera();
        }
        else if(m_Receiver.IsKeyDown(KEY_KEY_W))
        {
            f32 moveval = MOVEMENT_SPEED * frameDeltaTime;
            vector3df movecam( -moveval, -moveval, 0);
            m_CameraPos += movecam;
            m_CameraTarget += movecam;
            updateCamera();
        }
        else if(m_Receiver.IsKeyDown(KEY_KEY_S))
        {
            f32 moveval = MOVEMENT_SPEED * frameDeltaTime;
            vector3df movecam( moveval, moveval, 0);
            m_CameraPos += movecam;
            m_CameraTarget += movecam;
            updateCamera();
        }


        //clear scene
        m_Driver->beginScene(true, true, SColor(255,100,101,140));

        //draw scene
        m_SMgr->drawAll();

        //draw gui
        m_GUIEnv->drawAll();

        //done and display
        m_Driver->endScene();

        int fps = m_Driver->getFPS();

        //test
        std::cout << "m_CameraPos:" << m_CameraPos.X << "," << m_CameraPos.Y << "," << m_CameraPos.Z << " - target:" <<
            m_CameraTarget.X << "," << m_CameraTarget.Y << "," << m_CameraTarget.Z << std::endl;

        if (lastFPS != fps)
        {
            core::stringw tmp(L"Game [");
            tmp += m_Driver->getName();
            tmp += L"] fps: ";
            tmp += fps;

            m_Device->setWindowCaption(tmp.c_str());
            lastFPS = fps;
        }

    }

    return 0;
}
