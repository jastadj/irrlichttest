#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int main(int argc, char *argv[])
{

    IrrlichtDevice *device = createDevice( video::EDT_SOFTWARE, dimension2d<u32>(640, 480), 16, false, false, false, 0);

    if(!device) return 1;

    //get video
    IVideoDriver* driver = device->getVideoDriver();
    //get scene manager
    ISceneManager* smgr = device->getSceneManager();
    //get gui
    IGUIEnvironment* guienv = device->getGUIEnvironment();

    //add camera to scene
    smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

    //main loop
    while(device->run())
    {
        driver->beginScene(true, true, SColor(255,100,101,140));

        smgr->drawAll();
        guienv->drawAll();

        driver->endScene();
    }


    device->drop();

    return 0;
}
