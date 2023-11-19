/*
 * This is free and unencumbered software released into the public domain. 
 */

#pragma once

#include <bgfx/bgfx.h>
//#include <imgui.h>
//#include <GLFW/glfw3.h> //no
#include <glm/glm.hpp> 
#include <vector>

#include <SDL.h>
#include <SDL_syswm.h>

#include <bx/allocator.h>
#include <bx/readerwriter.h>
#include <bimg/bimg.h>

namespace bigg
{
	// bgfx utils
	const bgfx::Memory* loadMemory( const char* filename );
	bgfx::ShaderHandle loadShader( const char* shader );
	bgfx::ProgramHandle loadProgram( const char* vsName, const char* fsName );
	bgfx::TextureHandle loadTexture(const char* filePath);
	
	/*
	// allocator
	class Allocator : public bx::AllocatorI
	{
	public:
		void* realloc( void* _ptr, size_t _size, size_t _align, const char* _file, uint32_t _line )
		{
			if ( _size == 0 )
			{
				free( _ptr );
				return nullptr;
			}
			else
			{
				return malloc( _size );
			}
		}
	};
	*/
	// application
	class Application
	{
		static void cursorPosCallback(SDL_Window* window, double xpos, double ypos);

	public:
		Application( const char* title = "", uint32_t width = 1280, uint32_t height = 768 );

		int run(
			int argc,
			char** argv,
			bgfx::RendererType::Enum type = bgfx::RendererType::Count,
			uint16_t vendorId = BGFX_PCI_ID_NONE,
			uint16_t deviceId = 0,
			bgfx::CallbackI* callback = NULL,
			bx::AllocatorI* allocator = NULL
		);

		void close();
		void reset( uint32_t flags = 0 );
		uint32_t getWidth() const;
		uint32_t getHeight() const;
		void setSize( int width, int height );
		const char* getTitle() const;
		void setTitle( const char* title );
		bool isKeyDown(int key) const;
		bool isKeyReleased(int key) const;
		
		virtual void initialize( ) {};
		virtual void update( float dt ) {};
		virtual int shutdown() { return 0; };

		virtual void onReset() {};
		virtual void onCursorPos(double xpos, double ypos) {}
		virtual void onWindowSize( int width, int height ) {}

		int getCursorPosX(); int getCursorPosY();
		Application* getApp() { return this; }
	protected:
		//GLFWwindow* mWindow; //gone
		SDL_Window* window = nullptr;
		//bigg::Allocator mAllocator;
	private:
		int currentKey;

		uint32_t mReset;
		uint32_t mWidth;
		uint32_t mHeight;
		const char* mTitle;
	};
}
