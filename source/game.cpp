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

    //init font
    m_Font = m_GUIEnv->getBuiltInFont();

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
    m_CameraTarget = mymeshnodepos;
    m_CameraPos = vector3df(20,20,35) + m_CameraTarget;
    m_Camera->setPosition(m_CameraPos);
    m_Camera->setTarget(m_CameraTarget);
}

void Game::updateMyMesh(f32 frameDeltaTime)
{
    mymeshnodepos.X += mymeshnodevel.X*frameDeltaTime;
    mymeshnodepos.Y += mymeshnodevel.Y*frameDeltaTime;
    mymeshnodepos.Z = mymeshnodepos.Z;

    mymeshnodepos += mymeshnodevel*frameDeltaTime;

    mymeshnode->setPosition( mymeshnodepos + mymeshnodeposoff);
    mymeshnode->setRotation(vector3df(90,0, mymeshnodeangle));

    if(mymeshnodevel == vector3df(0,0,0))
    {
        if(mymeshnodestate == scene::EMAT_RUN)
        {
            mymeshnodestate = scene::EMAT_STAND;
            mymeshnode->setMD2Animation(mymeshnodestate);
        }
    }
    else if(mymeshnodestate == scene::EMAT_STAND)
    {
        mymeshnodestate = scene::EMAT_RUN;
        mymeshnode->setMD2Animation(mymeshnodestate);
    }

    printVector3f(mymeshnodepos);

}

int Game::mainLoop()
{
    //add camera to scene
    m_Camera = m_SMgr->addCameraSceneNode(0, m_CameraPos, m_CameraTarget);
    m_CameraTarget = vector3df(0,0,-20);
    updateCamera();
    matrix4 m;
    m.setRotationDegrees(m_Camera->getRotation());
    vector3df upv(0.0f, 0.0f, 1.0f);
    //m.transformVect(upv);
    m_Camera->setUpVector(upv);

    mymesh = m_SMgr->getMesh("sydney.md2");
    mymeshnode = m_SMgr->addAnimatedMeshSceneNode(mymesh);
    mymeshnodeposoff = vector3df(0,0,9.5);
    mymeshnode->setMaterialTexture(0, m_Driver->getTexture("sydney.bmp") );
    mymeshnode->setScale(vector3df(0.2,0.2,0.2));
    mymeshnodeangle = 45;
    mymeshnodestate = scene::EMAT_STAND;
    mymeshnodevel = vector3df(0,0,0);
    mymeshnode->setMD2Animation(mymeshnodestate);

    updateMyMesh(0);
    updateCamera();










    //m_CameraPos = vector3df(0,0,0);
    //m_CameraTarget = vector3df(0,0,0);


    //add some fog
    //m_Driver->setFog(video::SColor(0,138,125,81), video::EFT_FOG_LINEAR, 250, 1000, .003f, true, false);

    //add light
    scene::ILightSceneNode* light1 = m_SMgr->addLightSceneNode(0, mymeshnodepos + vector3df(0,0,20), SColorf(1.0f, 0.5f, 0.5f, 0.f), 20.0f);
    light1->setLightType(video::ELT_SPOT);
    light1->setRotation(vector3df(0,180,0));
    light1->getLightData().Falloff = 5;
    //light1->getLightData().DiffuseColor = SColor(100,100,100,100);
    //light1->getLightData().SpecularColor = SColor(0,0,0,0);
    //light1->enableCastShadow(false);
    m_SMgr->setAmbientLight( SColor(100,100,100,100));





    ISceneNode *mytestcube;




    int msize = 1;
    for(int i = 0; i < msize; i++)
    {
        for(int n = 0; n < msize; n++)
        {
            f32 cubesize = 10;
            vector3df cubepos(cubesize*n, cubesize*i, 0);
            ISceneNode *mycube = m_SMgr->addMeshSceneNode(getCube(cubesize));
            if(mycube)
            {
                mycube->setPosition(vector3df(n*cubesize, i*cubesize, 0));
                mycube->setMaterialTexture(0, m_Driver->getTexture("walltest.jpg"));
                mycube->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
                //mycube->setMaterialFlag(video::EMF_LIGHTING, false);
                //mycube->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
                //mycube->setMaterialFlag(video::EMF_BLEND_OPERATION, true);
                //mycube->setMaterialType(video::EMT_PARALLAX_MAP_SOLID);
            }

            mytestcube = mycube;
        }
    }



    int lastFPS = -1;

    // In order to do framerate independent movement, we have to know
    // how long it was since the last frame
    u32 then = m_Device->getTimer()->getTime();

    // This is the movemen speed in units per second.
    const f32 MOVEMENT_SPEED = 20.f;





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
            mymeshnodevel = vector3df(-MOVEMENT_SPEED, MOVEMENT_SPEED, 0);
            mymeshnodeangle = 135;
        }
        else if(m_Receiver.IsKeyDown(KEY_KEY_D))
        {
            mymeshnodevel = vector3df(MOVEMENT_SPEED, -MOVEMENT_SPEED, 0);
            mymeshnodeangle = -45;
        }
        else if(m_Receiver.IsKeyDown(KEY_KEY_W))
        {
            mymeshnodevel = vector3df(-MOVEMENT_SPEED, -MOVEMENT_SPEED, 0);
            mymeshnodeangle = -135;
        }
        else if(m_Receiver.IsKeyDown(KEY_KEY_S))
        {
            mymeshnodevel = vector3df(MOVEMENT_SPEED, MOVEMENT_SPEED, 0);
            mymeshnodeangle = 45;
        }
        else mymeshnodevel = vector3df(0,0,0);

        //update actor and camera
        updateMyMesh(frameDeltaTime);
        updateCamera();
        light1->setPosition(mymeshnodepos + mymeshnodeposoff + vector3df(0,0,20));

        //clear scene
        m_Driver->beginScene(true, true, SColor(255,100,101,140));

        //draw scene
        m_SMgr->drawAll();

        //draw gui
        m_GUIEnv->drawAll();

        /*
        m_Driver->setMaterial(SMaterial());
        m_Driver->setTransform(video::ETS_WORLD, IdentityMatrix);
        m_Driver->draw3DLine( vector3df(0,0,0), vector3df(100,0,0), SColor(0,255,0,0));
        */

        //done and display
        m_Driver->endScene();

        int fps = m_Driver->getFPS();

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


void Game::printVector3f(vector3df &tvec, std::string vstr)
{
    std::cout << vstr << ":" << tvec.X << "," << tvec.Y << "," << tvec.X << std::endl;
}


SMesh *Game::getCube(f32 cubesize)
{

        SMesh* Mesh = new SMesh();

        int vcount = 36;

        SMeshBuffer *buf = new SMeshBuffer();
        Mesh->addMeshBuffer(buf);
        buf->drop();

        buf->Vertices.reallocate(vcount);
        buf->Vertices.set_used(vcount);

        //top
        buf->Vertices[0] = S3DVertex(0,0,0, 0,0,1,    video::SColor(255,255,255,255), 0, 0);
        buf->Vertices[1] = S3DVertex(1*cubesize,0,0, 0,0,1,  video::SColor(255,255,255,255), 1, 0);
        buf->Vertices[2] = S3DVertex(0,1*cubesize,0, 0,0,1,    video::SColor(255,255,255,255), 0, 1);
        buf->Vertices[3] = S3DVertex(1*cubesize,0,0, 0,0,1,    video::SColor(255,255,255,255), 1, 0);
        buf->Vertices[4] = S3DVertex(1*cubesize,1*cubesize,0, 0,0,1,    video::SColor(255,255,255,255), 1, 1);
        buf->Vertices[5] = S3DVertex(0,1*cubesize,0, 0,0,1,    video::SColor(255,255,255,255), 0, 1);

        //front
        buf->Vertices[6] = S3DVertex(0,1*cubesize,0, 0,1,0,    video::SColor(255,255,255,255), 0, 0);
        buf->Vertices[7] = S3DVertex(1*cubesize,1*cubesize,0, 0,1,0,    video::SColor(255,255,255,255), 1, 0);
        buf->Vertices[8] = S3DVertex(0,1*cubesize,-1*cubesize, 0,1,0,    video::SColor(255,255,255,255), 0, 1);
        buf->Vertices[9] = S3DVertex(1*cubesize,1*cubesize,0, 0,1,0,    video::SColor(255,255,255,255), 1, 0);
        buf->Vertices[10] = S3DVertex(1*cubesize,1*cubesize,-1*cubesize, 0,1,0,    video::SColor(255,255,255,255), 1, 1);
        buf->Vertices[11] = S3DVertex(0,1*cubesize,-1*cubesize, 0,1,0,    video::SColor(255,255,255,255), 0, 1);

        //right
        buf->Vertices[12] = S3DVertex(1*cubesize,1*cubesize,0, 1,0,0,    video::SColor(255,255,255,255), 0, 0);
        buf->Vertices[13] = S3DVertex(1*cubesize,0,0, 1,0,0,    video::SColor(255,255,255,255), 1, 0);
        buf->Vertices[14] = S3DVertex(1*cubesize,1*cubesize,-1*cubesize, 1,0,0,    video::SColor(255,255,255,255), 0, 1);
        buf->Vertices[15] = S3DVertex(1*cubesize,0,0, 1,0,0,    video::SColor(255,255,255,255), 1, 0);
        buf->Vertices[16] = S3DVertex(1*cubesize,0,-1*cubesize, 1,0,0,    video::SColor(255,255,255,255), 1, 1);
        buf->Vertices[17] = S3DVertex(1*cubesize,1*cubesize,-1*cubesize, 1,0,0,    video::SColor(255,255,255,255), 0, 1);

        //left
        buf->Vertices[18] = S3DVertex(0,0,0, -1,0,0,    video::SColor(255,255,255,255), 0, 0);
        buf->Vertices[19] = S3DVertex(0,1*cubesize,0, -1,0,0,    video::SColor(255,255,255,255), 1, 0);
        buf->Vertices[20] = S3DVertex(0,0,-1*cubesize, -1,0,0,    video::SColor(255,255,255,255), 0, 1);
        buf->Vertices[21] = S3DVertex(0,1*cubesize,0, -1,0,0,    video::SColor(255,255,255,255), 1, 0);
        buf->Vertices[22] = S3DVertex(0,1*cubesize,-1*cubesize, -1,0,0,    video::SColor(255,255,255,255), 1, 1);
        buf->Vertices[23] = S3DVertex(0,0,-1*cubesize, -1,0,0,    video::SColor(255,255,255,255), 0, 1);

        //back
        buf->Vertices[24] = S3DVertex(1*cubesize,0,0, 0,-1,0,    video::SColor(255,255,255,255), 0, 0);
        buf->Vertices[25] = S3DVertex(0,0,0, 0,-1,0,    video::SColor(255,255,255,255), 1, 0);
        buf->Vertices[26] = S3DVertex(1*cubesize,0,-1*cubesize, 0,-1,0,    video::SColor(255,255,255,255), 0, 1);
        buf->Vertices[27] = S3DVertex(0,0,0, 0,-1,0,    video::SColor(255,255,255,255), 1, 0);
        buf->Vertices[28] = S3DVertex(0,0,-1*cubesize, 0,-1,0,    video::SColor(255,255,255,255), 1, 1);
        buf->Vertices[29] = S3DVertex(1*cubesize,0,-1*cubesize, 0,-1,0,    video::SColor(255,255,255,255), 0, 1);

        //bottom
        buf->Vertices[30] = S3DVertex(0,1*cubesize,-1*cubesize, 0,0,-1,    video::SColor(255,255,255,255), 0, 0);
        buf->Vertices[31] = S3DVertex(1*cubesize,1*cubesize,-1*cubesize, 0,0,-1,  video::SColor(255,255,255,255), 1, 0);
        buf->Vertices[32] = S3DVertex(0,0,-1*cubesize, 0,0,-1,    video::SColor(255,255,255,255), 0, 1);
        buf->Vertices[33] = S3DVertex(1*cubesize,1*cubesize,-1*cubesize, 0,0,-1,    video::SColor(255,255,255,255), 1, 0);
        buf->Vertices[34] = S3DVertex(1*cubesize,0,-1*cubesize, 0,0,-1,    video::SColor(255,255,255,255), 1, 1);
        buf->Vertices[35] = S3DVertex(0,0,-1*cubesize, 0,0,-1,    video::SColor(255,255,255,255), 0, 1);

        buf->Indices.reallocate(vcount);
        buf->Indices.set_used(vcount);

        for(int i = 0; i < vcount; i++) buf->Indices[i] = i;
        /*
        buf->Indices[0]=0;
        buf->Indices[1]=1;
        buf->Indices[2]=2;
        buf->Indices[3]=3;
        buf->Indices[4]=4;
        buf->Indices[5]=5;
        */

        buf->recalculateBoundingBox();

        return Mesh;

}
