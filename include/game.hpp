#ifndef CLASS_GAME
#define CLASS_GAME

#include <irrlicht.h>

//irrlicht namespaces
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class Game
{
private:

    //irrlicht renderer
    IrrlichtDevice *device;
    IVideoDriver* driver;
    ISceneManager* smgr;
    IGUIEnvironment* guienv;

    //init
    bool initIrrlicht();

    //mainloop
    int mainLoop();

public:
    Game();
    ~Game();

    void start();

};
#endif // CLASS_GAME
