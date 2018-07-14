#include "GraphicsClass.h"



GraphicsClass::GraphicsClass()
{
	m_OpenGL = 0;
	m_Camera = 0;
	m_Model = 0;
	m_ColorShader = 0;
	m_TextureShader = 0;
}


GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(OpenGLClass* OpenGL, HWND hwnd)
{
	m_OpenGL = OpenGL;

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Create the model object.
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	if (!m_Model->Initialize(m_OpenGL, "fire3.tga", 0, true,0))
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Model2 = new ModelClass;
	if (!m_Model2)
	{
		return false;
	}

	// Initialize the model object.
	if (!m_Model2->Initialize(m_OpenGL, "fire3.tga", 0, true,1))
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	
	if (!m_ColorShader->Initialize(m_OpenGL, hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	
	if (!m_TextureShader->Initialize(m_OpenGL, hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	object1 = new ObjectClass();
	object2 = new ObjectClass();

	object1->init(m_OpenGL, 1, m_Model, m_ColorShader, m_TextureShader);
	object2->init(m_OpenGL, 2, m_Model2, m_ColorShader, m_TextureShader);

	return true;
}

void GraphicsClass::Shutdown()
{
	if (object1)
	{
		object1->shutdown(m_OpenGL);
		delete object1;
		object1 = 0;
	}
	if (object2)
	{
		object2->shutdown(m_OpenGL);
		delete object2;
		object2 = 0;
	}
	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown(m_OpenGL);
		delete m_TextureShader;
		m_TextureShader = 0;
	}
	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown(m_OpenGL);
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown(m_OpenGL);
		delete m_Model;
		m_Model = 0;
	}

	if (m_Model2)
	{
		m_Model2->Shutdown(m_OpenGL);
		delete m_Model2;
		m_Model2 = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	m_OpenGL = 0;
	return;
}

bool GraphicsClass::Frame(int object)
{
	bool result;
	result = Render(object);
	if (!result)
	{
		return false;
	}
	return true;
}
bool GraphicsClass::Frame()
{
	bool result;
	result = Render();
	if (!result)
	{
		return false;
	}
	return true;
}

bool GraphicsClass::Render(int object)
{
	// Clear the buffers to begin the scene.
	m_OpenGL->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the opengl and camera objects.
	object1->render(m_OpenGL, m_Camera);
	object2->render(m_OpenGL, m_Camera);
	m_OpenGL->EndScene();
	return true;
}
bool GraphicsClass::Render()
{
	float worldMatrix[16];
	float viewMatrix[16];
	float projectionMatrix[16];
	// Clear the buffers to begin the scene.
	m_OpenGL->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the opengl and camera objects.
	m_OpenGL->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_OpenGL->GetProjectionMatrix(projectionMatrix);

	// Set the color shader as the current shader program and set the matrices that it will use for rendering.
	//m_ColorShader->SetShader(m_OpenGL);
	//m_ColorShader->SetShaderParameters(m_OpenGL, worldMatrix, viewMatrix, projectionMatrix);

	// Set the texture shader as the current shader program and set the matrices that it will use for rendering.
	m_TextureShader->SetShader(m_OpenGL);
	m_TextureShader->SetShaderParameters(m_OpenGL, worldMatrix, viewMatrix, projectionMatrix, 0);

	// Render the model using the color shader.
	m_Model->Render(m_OpenGL);

	// Present the rendered scene to the screen.
	m_OpenGL->EndScene();
	return true;
}