#include "OpenGLClass.h"


namespace MoebiusEngine
{
	OpenGLClass::OpenGLClass()
	{
		m_deviceContext = 0;
		m_renderingContext = 0;
	}

	OpenGLClass::OpenGLClass(const OpenGLClass& other)
	{
	}

	OpenGLClass::~OpenGLClass()
	{
	}

	bool OpenGLClass::InitializeExtensions(HWND hwnd)
	{
		HDC deviceContext;
		PIXELFORMATDESCRIPTOR pixelFormat;
		int error;
		HGLRC renderContext;
		bool result;

		// Get the device context for this window.
		deviceContext = GetDC(hwnd);
		if (!deviceContext)
			return false;
		// Set a temporary default pixel format.
		error = SetPixelFormat(deviceContext, 1, &pixelFormat);
		if (error != 1)
			return false;

		// Create a temporary rendering context.
		renderContext = wglCreateContext(deviceContext);
		if (!renderContext)
			return false;

		// Set the temporary rendering context as the current rendering context for this window.
		error = wglMakeCurrent(deviceContext, renderContext);
		if (error != 1)
			return false;

		//Initialize the OpenGL extensions needed for this application
		result = LoadExtensionList();
		if (!result)
			return false;

		// Release the temporary rendering context now that the extensions have been loaded.
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(renderContext);
		renderContext = NULL;

		// Release the device context for this window.
		ReleaseDC(hwnd, deviceContext);
		deviceContext = 0;

		return true;
	}
	bool OpenGLClass::InitializeOpenGL(HWND hwnd, int screenWidth, int screeHeight, float screenDepth, float screenNear, bool vsync)
	{
		int attributeListInt[19];
		int pixelFormat[1];
		unsigned int formatCount;
		int result;
		PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
		int attributeList[5];
		char *vendorString, *rendererString;

		//Get the device context for this window
		m_deviceContext = GetDC(hwnd);
		if (!m_deviceContext)
			return false;
		//Support for OpenGL rendering
		attributeListInt[0] = WGL_SUPPORT_OPENGL_ARB;
		attributeListInt[1] = TRUE;
		// Support for rendering to a window.
		attributeListInt[2] = WGL_DRAW_TO_WINDOW_ARB;
		attributeListInt[3] = TRUE;
		// Support for hardware acceleration.
		attributeListInt[4] = WGL_ACCELERATION_ARB;
		attributeListInt[5] = WGL_FULL_ACCELERATION_ARB;
		// Support for 24bit color.
		attributeListInt[6] = WGL_COLOR_BITS_ARB;
		attributeListInt[7] = 24;
		// Support for 24 bit depth buffer.
		attributeListInt[8] = WGL_DEPTH_BITS_ARB;
		attributeListInt[9] = 24;
		// Support for double buffer.
		attributeListInt[10] = WGL_DOUBLE_BUFFER_ARB;
		attributeListInt[11] = TRUE;
		// Support for swapping front and back buffer.
		attributeListInt[12] = WGL_SWAP_METHOD_ARB;
		attributeListInt[13] = WGL_SWAP_EXCHANGE_ARB;
		// Support for the RGBA pixel type.
		attributeListInt[14] = WGL_PIXEL_TYPE_ARB;
		attributeListInt[15] = WGL_TYPE_RGBA_ARB;
		// Support for a 8 bit stencil buffer.
		attributeListInt[16] = WGL_STENCIL_BITS_ARB;
		attributeListInt[17] = 8;
		// Null terminate the attribute list.
		attributeListInt[18] = 0;

		// Query for a pixel format that fits the attributes we want.
		result = wglChoosePixelFormatARB(m_deviceContext, attributeListInt, NULL, 1, pixelFormat, &formatCount);
		if (result != 1)
			return false;
		
		// If the video card/display can handle our desired pixel format then we set it as the current one.
		result = SetPixelFormat(m_deviceContext, pixelFormat[0], &pixelFormatDescriptor);
		if (result != 1)
			return false;

		// Set the 4.0 version of OpenGL in the attribute list.
		attributeList[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
		attributeList[1] = 4;
		attributeList[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
		attributeList[3] = 0;

		// Null terminate the attribute list.
		attributeList[4] = 0;

		// Create a OpenGL 4.0 rendering context.
		m_renderingContext = wglCreateContextAttribsARB(m_deviceContext, 0, attributeList);
		if (m_renderingContext == NULL)
			return false;
		
		// Set the rendering context to active.
		result = wglMakeCurrent(m_deviceContext, m_renderingContext);
		if (result != 1)
			return false;

		// Get the name of the video card.
		vendorString = (char*)glGetString(GL_VENDOR);
		rendererString = (char*)glGetString(GL_RENDERER);

		// Store the video card name in a class member variable so it can be retrieved later.
		strcpy_s(m_videoCardDescription, vendorString);
		strcat_s(m_videoCardDescription, " - ");
		strcat_s(m_videoCardDescription, rendererString);
		
		// Turn on or off the vertical sync depending on the input bool value.
		if (vsync)
			result = wglSwapIntervalEXT(1);
		else
			result = wglSwapIntervalEXT(0);

		// Check if vsync was set correctly.
		if (result != 1)
			return false;


		return true;
	}
	void OpenGLClass::Shutdown(HWND hwnd)
	{
		// Release the rendering context.
		if (m_renderingContext)
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(m_renderingContext);
			m_renderingContext = 0;
		}
		// Release the device context.
		if (m_deviceContext)
		{
			ReleaseDC(hwnd, m_deviceContext);
			m_deviceContext = 0;
		}
	}
	void OpenGLClass::BeginScene(float r, float g, float b, float a)
	{
		// Set the color to clear the screen to
		glClearColor(r, g, b, a);

		// Clear the screen and depth buffer.
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLClass::EndScene()
	{
		// Present the back buffer to the screen since rendering is complete.
		SwapBuffers(m_deviceContext);
	}
	bool OpenGLClass::LoadExtensionList()
	{
		// Load the OpenGL extensions 
		wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
		if (!wglChoosePixelFormatARB)
			return false;

		wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		if (!wglCreateContextAttribsARB)
			return false;

		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		if (!wglSwapIntervalEXT)
			return false;

		glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
		if (!glAttachShader)
			return false;

		glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
		if (!glBindBuffer)
			return false;

		glBindTexture = (PFNGLBINDTEXTUREPROC)wglGetProcAddress("glBindTexture");
		if (!glBindTexture)
			return false;

		glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
		if (!glBindVertexArray)
			return false;

		glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
		if (!glBufferData)
			return false;

		glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
		if (!glCompileShader)
			return false;

		glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
		if (!glCreateProgram)
			return false;

		glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
		if (!glCreateShader)
			return false;

		glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
		if (!glDeleteBuffers)
			return false;

		glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
		if (!glDeleteProgram)
			return false;

		glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
		if (!glDeleteShader)
			return false;

		glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
		if (!glDeleteVertexArrays)
			return false;

		glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
		if (!glDetachShader)
			return false;

		glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
		if (!glEnableVertexAttribArray)
			return false;

		glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
		if (!glGenBuffers)
			return false;

		glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
		if (!glGenVertexArrays)
			return false;

		glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
		if (!glGetAttribLocation)
			return false;

		glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
		if (!glGetProgramInfoLog)
			return false;

		glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
		if (!glGetProgramiv)
			return false;

		glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
		if (!glGetShaderInfoLog)
			return false;

		glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
		if (!glGetShaderiv)
			return false;

		glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
		if (!glLinkProgram)
			return false;

		glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
		if (!glShaderSource)
			return false;

		glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
		if (!glUseProgram)
			return false;

		glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
		if (!glVertexAttribPointer)
			return false;

		glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
		if (!glBindAttribLocation)
			return false;

		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
		if (!glGetUniformLocation)
			return false;

		glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
		if (!glUniformMatrix4fv)
			return false;

		glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
		if (!glActiveTexture)
			return false;

		glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
		if (!glGenerateMipmap)
			return false;

		glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
		if (!glDisableVertexAttribArray)
			return false;

		glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
		if (!glUniform1i)
			return false;

		glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
		if (!glUniform1f)
			return false;
		
		glUniform2f = (PFNGLUNIFORM2FPROC)wglGetProcAddress("glUniform2f");
		if (!glUniform2f)
			return false;

		glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
		if (!glUniform2fv)
			return false;

		glUniform2i = (PFNGLUNIFORM2IPROC)wglGetProcAddress("glUniform2i");
		if (!glUniform2i)
			return false;

		glUniform2iv = (PFNGLUNIFORM2IVPROC)wglGetProcAddress("glUniform2iv");
		if (!glUniform2iv)
			return false;

		glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
		if (!glUniform3f)
			return false;

		glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
		if (!glUniform3fv)
			return false;

		glUniform3i = (PFNGLUNIFORM3IPROC)wglGetProcAddress("glUniform3i");
		if (!glUniform3i)
			return false;

		glUniform3iv = (PFNGLUNIFORM3IVPROC)wglGetProcAddress("glUniform3iv");
		if (!glUniform3iv)
			return false;

		glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");
		if (!glUniform4f)
			return false;

		glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
		if (!glUniform4fv)
			return false;

		glUniform4i = (PFNGLUNIFORM4IPROC)wglGetProcAddress("glUniform4i");
		if (!glUniform4i)
			return false;

		glUniform4iv = (PFNGLUNIFORM4IVPROC)wglGetProcAddress("glUniform4iv");
		if (!glUniform4iv)
			return false;

		glGenTextures = (PFNGLGENTEXTURESPROC)wglGetProcAddress("glGenTextures");
		if (!glGenTextures)
			return false;

		glBlendColor = (PFNGLBLENDCOLORPROC)wglGetProcAddress("glBlendColor");
		if (!glBlendColor)
			return false;

		return true;
	}
}
