/*
 * Simple rotating cubes example, based off bgfx's example-01-cubes.
 * Does not mimic the original example completely because we're in a
 * different coordinate system.
 *
 * Copyright 2011-2017 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include <bigg.hpp>
#include <bx/string.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp> 
#include "camera.h"
#include <iostream>


std::vector <float> vertexPositions =
{
    0.5, 0.5,   //top right
    -0.5, 0.5,  //top left
    -0.5, -0.5, //bottom left
    0.5, -0.5,  //bottom right
};

struct PosColorVertex
{
    float x;
    float y;
    float z;
    int16_t m_u;
    int16_t m_v;
    //uint32_t abgr;
    static void init()
    {
        ms_layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Int16, true, true)
            //.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
    }
    static bgfx::VertexLayout ms_layout;
};
bgfx::VertexLayout PosColorVertex::ms_layout;

/*

static PosColorVertex s_cubeVertices[] =
{
    {-1.0f,  1.0f,  1.0f,      0,      0 },
    { 1.0f,  1.0f,  1.0f, 0x7fff,      0 },
    {-1.0f, -1.0f,  1.0f,      0, 0x7fff },
    { 1.0f, -1.0f,  1.0f, 0x7fff, 0x7fff },
    {-1.0f,  1.0f, -1.0f,     0,      0 },
    { 1.0f,  1.0f, -1.0f, 0x7fff,      0 },
    {-1.0f, -1.0f, -1.0f,      0, 0x7fff },
    { 1.0f, -1.0f, -1.0f, 0x7fff, 0x7fff },
    {-1.0f,  1.0f,  1.0f,      0,      0 },
    { 1.0f,  1.0f,  1.0f, 0x7fff,      0 },
    {-1.0f,  1.0f, -1.0f,      0, 0x7fff },
    { 1.0f,  1.0f, -1.0f, 0x7fff, 0x7fff },
    {-1.0f, -1.0f,  1.0f,      0,      0 },
    { 1.0f, -1.0f,  1.0f,  0x7fff,      0 },
    {-1.0f, -1.0f, -1.0f,      0, 0x7fff },
    { 1.0f, -1.0f, -1.0f, 0x7fff, 0x7fff },
    { 1.0f, -1.0f,  1.0f,      0,      0 },
    { 1.0f,  1.0f,  1.0f, 0x7fff,      0 },
    { 1.0f, -1.0f, -1.0f,      0, 0x7fff },
    { 1.0f,  1.0f, -1.0f, 0x7fff, 0x7fff },
    {-1.0f, -1.0f,  1.0f,      0,      0 },
    {-1.0f,  1.0f,  1.0f, 0x7fff,      0 },
    {-1.0f, -1.0f, -1.0f,      0, 0x7fff },
    {-1.0f,  1.0f, -1.0f, 0x7fff, 0x7fff },
};
*/



//static const uint16_t s_cubeTriList[] = { 2, 1, 0, 2, 3, 1, 5, 6, 4, 7, 6, 5, 4, 2, 0, 6, 2, 4, 3, 5, 1, 3, 7, 5, 1, 4, 0, 1, 5, 4, 6, 3, 2, 7, 3, 6 };
static const uint16_t s_cubeTriList[] =
{
     0,  2,  1,
     1,  2,  3,
     4,  5,  6,
     5,  7,  6,

     8, 10,  9,
     9, 10, 11,
    12, 13, 14,
    13, 15, 14,

    16, 18, 17,
    17, 18, 19,
    20, 21, 22,
    21, 23, 22,
};

float s_cubePositions[] =
{
    /*
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    */
    -1.0f,  1.0f,  1.0f, 
     1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,   
     1.0f, -1.0f,  1.0f, 
    -1.0f,  1.0f, -1.0f,  
     1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,   
    1.0f, -1.0f, -1.0f, 
    -1.0f,  1.0f,  1.0f,      
     1.0f,  1.0f,  1.0f, 
    -1.0f,  1.0f, -1.0f,  
     1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f, 
     1.0f, -1.0f,  1.0f, 
    -1.0f, -1.0f, -1.0f, 
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f, 
     1.0f,  1.0f,  1.0f, 
     1.0f, -1.0f, -1.0f, 
     1.0f,  1.0f, -1.0f, 
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f, 
    -1.0f, -1.0f, -1.0f,  
    -1.0f,  1.0f, -1.0f, 
};

//int16_t s_cubeTexCoords[] =
float s_cubeTexCoords[] =
{
    /*
    0, 0,
    0x7fff, 0,
    0, 0x7fff,
    0x7fff, 0x7fff,
    0, 0,
    0x7fff, 0,
    0, 0x7fff,
    0x7fff, 0x7fff,
    */
    /*
          0,      0 ,
     0x7fff,      0 ,
          0, 0x7fff ,
     0x7fff, 0x7fff ,
         0,      0 ,
     0x7fff,      0 ,
         0, 0x7fff ,
     0x7fff, 0x7fff ,
          0,      0 ,
     0x7fff,      0 ,
          0, 0x7fff ,
     0x7fff, 0x7fff ,
          0,      0 ,
      0x7fff,      0 ,
          0, 0x7fff ,
     0x7fff, 0x7fff ,
          0,      0 ,
     0x7fff,      0 ,
          0, 0x7fff ,
     0x7fff, 0x7fff ,
          0,      0 ,
     0x7fff,      0 ,
          0, 0x7fff ,
     0x7fff, 0x7fff ,
    */
             0,      0 ,
     1,      0 ,
          0, 1 ,
     1, 1 ,
         0,      0 ,
     1,      0 ,
         0, 1 ,
     1, 1 ,
          0,      0 ,
     1,      0 ,
          0, 1 ,
     1, 1 ,
          0,      0 ,
      1,      0 ,
          0, 1 ,
     1, 1 ,
          0,      0 ,
     1,      0 ,
          0, 1 ,
     1, 1 ,
          0,      0 ,
     1,      0 ,
          0, 1 ,
     1, 1 ,

};

//settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera new
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

class ExampleCubes : public bigg::Application
{


    // camera
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    void initialize()
    {
        PosColorVertex::init();

        const size_t numVertices = sizeof(s_cubePositions) / (3);
        static PosColorVertex s_cubeVertices[numVertices];

        for (int i = 0; i < numVertices; ++i)
        {


            s_cubeVertices[i].x = s_cubePositions[i * 3];
            s_cubeVertices[i].y = s_cubePositions[i * 3 + 1];
            s_cubeVertices[i].z = s_cubePositions[i * 3 + 2];
            s_cubeVertices[i].m_u = static_cast<int16_t>(s_cubeTexCoords[i * 2] * 32767);
            s_cubeVertices[i].m_v = static_cast<int16_t>(s_cubeTexCoords[i * 2 + 1] * 32767); // Flip the V coordinate
        }



        /*
        // Calculate texture coordinates for each vertex
        for (int face = 0; face < 6; face++) // 6 faces
        {
            int row = face / 2; // 2 faces per row in the atlas
            int col = face % 2; // 2 faces per column in the atlas

            if (face == 0) { // Top face
                row = 0;
                col = 0;
            }
            else if (face == 1 || face == 2 || face == 3) { // Side faces
                row = 1;
                col = face - 1;
            }
            else { // Bottom face
                row = 2;
                col = 0;
            }

            for (int vertexIdx = 0; vertexIdx < 4; vertexIdx++) // 4 vertices per face
            {
                // Calculate the texture coordinates in the range [0, 1] for a 16x16 texture
                float u = col * 0.5f + vertexPositions[vertexIdx * 2] * 0.5f;
                float v = row * 0.5f + vertexPositions[vertexIdx * 2 + 1] * 0.5f;

                // Set the texture coordinates for the vertex
                s_cubeVertices[face * 4 + vertexIdx].m_u = static_cast<int16_t>(u * 32767);
                s_cubeVertices[face * 4 + vertexIdx].m_v = static_cast<int16_t>((1.0f - v) * 32767); // Flip the V coordinate
            }
        }
        
        */





        char vsName[32];
        char fsName[32];

        const char* shaderPath = "???";

        switch (bgfx::getRendererType())
        {
        case bgfx::RendererType::Noop:
        case bgfx::RendererType::Direct3D9:  shaderPath = "shaders/dx9/";   break;
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12: shaderPath = "shaders/dx11/";  break;
        case bgfx::RendererType::Gnm:                                       break;
        case bgfx::RendererType::Metal:      shaderPath = "shaders/metal/"; break;
        case bgfx::RendererType::OpenGL:     shaderPath = "shaders/glsl/";  break;
        case bgfx::RendererType::OpenGLES:   shaderPath = "shaders/essl/";  break;
        case bgfx::RendererType::Vulkan:     shaderPath = "shaders/spirv/"; break;
        case bgfx::RendererType::Count:                                     break;
        }
        std::cout << shaderPath << "\n";
        bx::strCopy(vsName, BX_COUNTOF(vsName), shaderPath);
        bx::strCat(vsName, BX_COUNTOF(vsName), "vs_cubes.bin");

        bx::strCopy(fsName, BX_COUNTOF(fsName), shaderPath);
        bx::strCat(fsName, BX_COUNTOF(fsName), "fs_cubes.bin");

        std::cout << fsName;

        mProgram = bigg::loadProgram(vsName, fsName);
        mVbh = bgfx::createVertexBuffer(bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), PosColorVertex::ms_layout);
        mIbh = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));

        //Create texture sampler uniform
                s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);

        //Load texture

                m_textureColor = bigg::loadTexture("textures/grass.png");


        //bgfx::setDebug(BGFX_DEBUG_TEXT);
        //mTime = 0.0f;

        // Initialize the camera position
        //mCameraPosition = glm::vec3(0.0f, 0.0f, -5.0f);


    }

    void onReset()
    {
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, uint16_t(getWidth()), uint16_t(getHeight()));
    }

    int x = 0;
    void update(float dt)
    {
        std::cout << x << ".\n";
        x++;
        mTime += dt;
        handleInput(dt); // Handle input for camera movement
        //handleMouse(bigg::Application::getApp());

        // camera/view transformation
        //glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 view = camera.GetViewMatrix();


        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); //last two vectors are the bounds of the cropping 3d << w
        //glm::mat4 proj = glm::perspective(glm::radians(60.0f), float(getWidth()) / getHeight(), 0.1f, 100.0f);
        bgfx::setViewTransform(0, &view[0][0], &proj[0][0]);
        //bgfx::setViewRect(0, 0, 0, uint16_t(getWidth()), uint16_t(getHeight()));
        //bgfx::touch(0);

        glm::mat4 mtx = glm::identity<glm::mat4>();
             //mtx *= glm::yawPitchRoll(mTime, 0.0f, 0.0f);
        // Set transform for draw call.
        bgfx::setTransform(&mtx[0][0]);

        // Set vertex and index buffer.
        bgfx::setVertexBuffer(0, mVbh);
        bgfx::setIndexBuffer(mIbh);


        // Bind textures.
                     bgfx::setTexture(0, s_texColor, m_textureColor);

        //bgfx::setState(BGFX_STATE_DEFAULT);
        // Set render states.
        bgfx::setState(0
            | BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | BGFX_STATE_MSAA
        );
        
        // Submit primitive for rendering to view 0.
                    bgfx::submit(0, mProgram);
        //ImGui::ShowDemoWindow();
    }

    int shutdown()
    {
        // Release resources before exiting
        bgfx::destroy(mProgram);
        bgfx::destroy(mVbh);
        bgfx::destroy(mIbh);
        return 0;
    }

public:
    ExampleCubes()
        : bigg::Application("Buttock", SCR_WIDTH, SCR_HEIGHT) {};

private:
    float fov = 45;
    bgfx::TextureHandle m_textureColor;
    bgfx::UniformHandle s_texColor;

    bgfx::ProgramHandle mProgram;
    bgfx::VertexBufferHandle mVbh;
    bgfx::IndexBufferHandle mIbh;
    float mTime;
    //glm::vec3 mCameraPosition; // Camera position variable
    void handleInput(float dt)
    {
        static bool toggle = false;
        toggle = !toggle;
        //if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE])
        //if(isKeyDown(SDL_SCANCODE_ESCAPE))
        if (isKeyDown(SDL_SCANCODE_ESCAPE)) {
            if(toggle){
                //SDL_SetRelativeMouseMode(SDL_TRUE);
            }
            else {
                //SDL_SetRelativeMouseMode(SDL_FALSE);
            }
            close();
        }
        else {
            if (isKeyDown(SDL_SCANCODE_ESCAPE))
            {
                //bigg::Application::close();
                //close();
                SDL_SetRelativeMouseMode(SDL_FALSE);

            }
        }

        float cameraSpeed = 5.0f * dt;

        //if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL])
        if(isKeyDown(SDL_SCANCODE_LCTRL))
            camera.setSpeed(2.5 * 3);
        else
            camera.setSpeed(2.5);
        if (isKeyDown(SDL_SCANCODE_LSHIFT))
            camera.setSpeed(2.5 * 0.33);

        //if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_W])
        if(isKeyDown(SDL_SCANCODE_W))
            camera.ProcessKeyboard(FORWARD, dt);

        //if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S])
        if(isKeyDown(SDL_SCANCODE_S))
            camera.ProcessKeyboard(BACKWARD, dt);

        //if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A])
        if(isKeyDown(SDL_SCANCODE_A))
            camera.ProcessKeyboard(LEFT, dt);

        //if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D])
        if(isKeyDown(SDL_SCANCODE_D))
            camera.ProcessKeyboard(RIGHT, dt);

        //if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_Q])
        if(isKeyDown(SDL_SCANCODE_Q))
            camera.ProcessKeyboard(DOWN, dt);

        //if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_E])
        if(isKeyDown(SDL_SCANCODE_E))
            camera.ProcessKeyboard(UP, dt);
    
        if (isKeyDown(SDL_SCANCODE_O))
            fov += 0.1f;
            camera.setFOV(fov);

        if (isKeyDown(SDL_SCANCODE_P))
            fov -= 0.1f;
            camera.setFOV(fov);
    
    }




    // glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------

    void onCursorPos(double xposIn, double yposIn)
    {
        
        //camera.ProcessMouseMovement(xoffset, yoffset);
        camera.ProcessMouseMovement(xposIn, yposIn);
    }

   

};

int main(int _argc, char** _argv)
{
    ExampleCubes app;
    app.run(_argc, _argv);
    return 0;
}