#include "game.hpp"
#include <iostream>
#include <string>

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
    device = createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16, false, false, false, 0);
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

    //add camera to scene
    smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

    //main loop
    while(device->run())
    {
        //clear scene
        driver->beginScene(true, true, SColor(255,100,101,140));

        //draw scene
        smgr->drawAll();

        //draw gui
        guienv->drawAll();

        //done and display
        driver->endScene();
    }

    return 0;
}
