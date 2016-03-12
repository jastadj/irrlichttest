#ifndef CLASS_GAME
#define CLASS_GAME

#include <irrlicht.h>
#include <iostream>
#include <string>

//irrlicht namespaces
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class MyEventReceiver : public IEventReceiver
{
public:
    // This is the one method that we have to implement
    virtual bool OnEvent(const SEvent& event)
    {
        // Remember whether each key is down or up
        if (event.EventType == irr::EET_KEY_INPUT_EVENT)
        {
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
        }


        return false;
    }

    // This is used to check whether a key is being held down
    virtual bool IsKeyDown(EKEY_CODE keyCode) const
    {
        return KeyIsDown[keyCode];
    }

    MyEventReceiver()
    {
        for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
            KeyIsDown[i] = false;
    }

private:
    // We use this array to store the current state of each key
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

class Game
{
private:

    //irrlicht renderer
    IrrlichtDevice *m_Device;
    IVideoDriver *m_Driver;
    ISceneManager *m_SMgr;
    IGUIEnvironment *m_GUIEnv;
    MyEventReceiver m_Receiver;

    //font
    IGUIFont *m_Font;

    //camera
    void updateCamera();
    ICameraSceneNode *m_Camera;
    vector3df m_CameraPos;
    vector3df m_CameraTarget;

    //actor
    IAnimatedMesh *mymesh;
    IAnimatedMeshSceneNode *mymeshnode;
    vector3df mymeshnodeposoff;
    vector3df mymeshnodepos;
    f32 mymeshnodeangle;
    scene::EMD2_ANIMATION_TYPE mymeshnodestate;
    vector3df mymeshnodevel;
    void updateMyMesh(f32 frameDeltaTime);

    //init
    bool initIrrlicht();

    //mainloop
    int mainLoop();

    //debug
    void printVector3f(vector3df &tvec, std::string vstr = std::string("vec"));
    SMesh *getCube(f32 cubesize);

public:
    Game();
    ~Game();

    void start();

};



#endif // CLASS_GAME
