/*
 * This is free and unencumbered software released into the public domain. 
 */
#include <bigg.hpp>
#include <iostream>

#include <bx/math.h>
#include <bgfx/platform.h>

#include <glm/glm.hpp>
#include <fstream>

#include "bigg_assets.h"
#include "bigg_shaders.hpp"

#include <stb/stb_image.h>
//#include "bigg_imgui.hpp" //will need to renable  - but need to switch to sdl first

// bgfx utils
const bgfx::Memory* bigg::loadMemory( const char* filename )
{
	std::ifstream file( filename, std::ios::binary | std::ios::ate );
	std::streamsize size = file.tellg();
	file.seekg( 0, std::ios::beg );
	const bgfx::Memory* mem = bgfx::alloc( uint32_t( size + 1 ) );
	if ( file.read( ( char* )mem->data, size ) )
	{
		mem->data[ mem->size - 1 ] = '\0';
		return mem;
	}
	return nullptr;
}

bgfx::ShaderHandle bigg::loadShader( const char* shader )
{
	return bgfx::createShader( loadMemory( shader ) );
}

bgfx::ProgramHandle bigg::loadProgram( const char* vsName, const char* fsName )
{
	bgfx::ShaderHandle vs = loadShader( vsName );
	bgfx::ShaderHandle fs = loadShader( fsName );
	return bgfx::createProgram( vs, fs, true );
}

bgfx::TextureHandle bigg::loadTexture(const char* filePath)//, uint32_t _flags, bool track)
{
	bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

	//IG_CORE_INFO("Searching for texture: {}", path);
	//if (textures.count(path))
	//{
		//IG_CORE_INFO("Found identical texture: {}", path);
		//handle = textures.at(path);
	//}
	//else
	{
		//IG_CORE_INFO("Loading texture: {}", path);
		std::cout << "Loading texture: " << filePath << "\n";
		int width, height;
		stbi_uc* data = stbi_load(filePath, &width, &height, 0, 4);
		const bgfx::Memory* mem = bgfx::copy(data, width * height * 4);
		if (nullptr != mem)
		{
			handle = bgfx::createTexture2D((uint16_t)width, (uint16_t)height, false, 1, bgfx::TextureFormat::RGBA8, BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT, mem);
		}
		else
		{
			std::cerr << "Could not load texture: " << filePath << "\n";
			//IG_CORE_ERROR("Could not load texture: {}", path);
		}
		stbi_image_free(data);

//#if IG_DEBUG
		//if (bgfx::isValid(handle))
		//{
			//bgfx::setName(handle, path.c_str());
		//}
//#endif
		//if (track)
		//{
			//textures[path] = handle;
		//}
	}

	return handle;
}

bigg::Application::Application( const char* title, uint32_t width, uint32_t height )
{
	mWidth = width;
	mHeight = height;
	mTitle = title;
}
//bool isDown = false;
int bigg::Application::run( int argc, char** argv, bgfx::RendererType::Enum type, uint16_t vendorId, uint16_t deviceId, bgfx::CallbackI* callback, bx::AllocatorI* allocator )
{
	
	// Initialize SDL systems
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error : " << SDL_GetError();
	}
	else {
		//Create a window
		window = SDL_CreateWindow(getTitle(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, getWidth(), getHeight(), SDL_WINDOW_SHOWN);
		if (window == nullptr) {
			std::cout << "SDL Window could not be created! : " << SDL_GetError();
		}
	}

	// tell SDL to capture our mouse
	SDL_SetRelativeMouseMode(SDL_TRUE);

	// Collect information about the window from SDL
	SDL_SysWMinfo wmi;
	SDL_VERSION(&wmi.version);
	if (!SDL_GetWindowWMInfo(window, &wmi)) {
		return 1;
	}

	void* native_window_handle = nullptr;

#	if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#		if ENTRY_CONFIG_USE_WAYLAND
	wl_egl_window* win_impl = (wl_egl_window*)SDL_GetWindowData(_window, "wl_egl_window");
	if (!win_impl)
	{
		int width, height;
		SDL_GetWindowSize(_window, &width, &height);
		struct wl_surface* surface = wmi.info.wl.surface;
		if (!surface)
			native_window_handle = nullptr;
		win_impl = wl_egl_window_create(surface, width, height);
		SDL_SetWindowData(_window, "wl_egl_window", win_impl);
	}
	native_window_handle = (void*)(uintptr_t)win_impl;
#		else
	native_window_handle = (void*)wmi.info.x11.window;
#		endif
#	elif BX_PLATFORM_OSX || BX_PLATFORM_IOS
	native_window_handle = wmi.info.cocoa.window;
#	elif BX_PLATFORM_WINDOWS
	native_window_handle = wmi.info.win.window;
#   elif BX_PLATFORM_ANDROID
	native_window_handle = wmi.info.android.window;
#	endif // BX_PLATFORM_

	bgfx::PlatformData pd;
#	if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#		if ENTRY_CONFIG_USE_WAYLAND
	pd.ndt = wmi.info.wl.display;
#		else
	pd.ndt = wmi.info.x11.display;
#		endif
#	else
	pd.ndt = NULL;
#	endif // BX_PLATFORM_
	pd.nwh = native_window_handle;

	pd.context = NULL;
	pd.backBuffer = NULL;
	pd.backBufferDS = NULL;
	bgfx::setPlatformData(pd);

	// Render an empty frame
	bgfx::renderFrame();

	bgfx::Init init;
	init.type = bgfx::RendererType::OpenGL;
	bgfx::init(init);

	// Setup ImGui
	//imguiInit( mWindow ); //fix header first

	// Initialize the application
	reset();
	initialize(  );

	/*bool running = true;
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;*/
	Uint32 lastUpdateTime = 0;
	Uint32 currentTime = SDL_GetTicks();
	float deltaTime = 0;
	bool running = true;


	while (running)
	{
		/* Calculating delta */
		/*LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST) * 1 / (double)SDL_GetPerformanceFrequency());*/
		lastUpdateTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime = (currentTime - lastUpdateTime) / 1000.0f;
		

		/* Handle Events */
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}

			if (event.type == SDL_MOUSEMOTION) {
				int x, y;
				SDL_GetRelativeMouseState(&x, &y);
				onCursorPos(x, -y);

			}

			if (event.type == SDL_KEYDOWN) {
				//keyDown == true;
				//std::cout << "HALLOO" << "\n";
				//isDown == true;
				//currentKey = event.key.keysym.sym;
			}
		}
		//imguiEvents(deltaTime); //fix header first

		/* Update Game State */
		//ImGui::NewFrame(); //fix header first
		update(deltaTime);

		/* Render */
		//ImGui::Render(); //fix header first
		//imguiRender(ImGui::GetDrawData()); //fix header first 
		//SDL_GL_SwapWindow(window);
		bgfx::frame();
	}















	/* Shutdown applicationand glfw */
	int ret = shutdown();
	//imguiShutdown(); //fix header first
	bgfx::shutdown();
	SDL_DestroyWindow(window);
	SDL_Quit();
	return ret;
}

void bigg::Application::reset( uint32_t flags )
{
	mReset = flags;
	bgfx::reset( mWidth, mHeight, mReset );
	//imguiReset( uint16_t( getWidth() ), uint16_t( getHeight() ) ); //fix header first
	onReset();
}

uint32_t bigg::Application::getWidth() const
{
	return mWidth;
}

uint32_t bigg::Application::getHeight() const
{
	return mHeight;
}

void bigg::Application::setSize( int width, int height )
{
	//glfwSetWindowSize( mWindow, width, height );
}

const char* bigg::Application::getTitle() const
{
	return mTitle;
}

void bigg::Application::setTitle( const char* title )
{
	mTitle = title;
	//glfwSetWindowTitle( mWindow, title );
}
bool bigg::Application::isKeyDown(int key) const
{
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

	
	//return SDL_GetKeyboardState(NULL)[key];
	return keyboardState[key] == SDL_PRESSED;

}

void bigg::Application::close()
{
	SDL_Event event;
	event.type = SDL_QUIT;
	SDL_PushEvent(&event);
}