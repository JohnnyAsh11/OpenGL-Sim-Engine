#include "Application.h"
#include "FileReader.h"
#include <iostream>
#include <glm\ext.hpp>
#include "Debug.h"
#include "Colors.h"
#include "Math.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_win32.h"

using namespace glm;

void Application::Init(std::string a_sAppName, uint a_uWidth, uint a_uHeight)
{
	std::cout << "Initializing the Window." << std::endl;

	// Initializing the window object.
	m_pWindow = new sf::Window(
		sf::VideoMode(a_uWidth, a_uHeight),
		a_sAppName,
		sf::Style::Default,
		sf::ContextSettings(24));

	// Initializing the window settings.
	InitWindow();

	std::shared_ptr<Shader> shader = std::make_shared<Shader>();
	shader->CompileShader("shaders/BasicVertex.glsl", "shaders/BasicFrag.glsl");
	std::shared_ptr<Material> matScratchedMetal = std::make_shared<Material>(shader, 0.5f);
	matScratchedMetal->AddTextureFromFile("textures/cobblestone.png", "Texture");

	// Loading the cube model for the skybox.
	std::shared_ptr<Mesh> cube = std::make_shared<Mesh>("models/cube.graphics_obj");

	// Loading in the car models.
	std::shared_ptr<Mesh> helix = std::make_shared<Mesh>("models/helix.graphics_obj");
	std::shared_ptr<Mesh> cylinder  = std::make_shared<Mesh>("models/cylinder.graphics_obj");
	std::shared_ptr<Mesh> sphere = std::make_shared<Mesh>("models/sphere.graphics_obj");
	std::shared_ptr<Mesh> torus = std::make_shared<Mesh>("models/torus.graphics_obj");
	
	// Adding the models to the entities list.
	m_lEntities.push_back(new Entity(torus, matScratchedMetal));
	m_lEntities.push_back(new Entity(sphere, matScratchedMetal));
	m_lEntities.push_back(new Entity(helix, matScratchedMetal));
	m_lEntities.push_back(new Entity(cylinder, matScratchedMetal));
	m_lEntities.push_back(new Entity(cube, matScratchedMetal));
	
	// Spacing out all of the entities.
	for (int i = 0; i < m_lEntities.size(); i++)
	{
		Transform* t = m_lEntities[i]->GetTransform();
		t->Scale(vec3(0.25f, 0.25f, 0.25f));
		t->MoveGlobal(glm::vec3(i * 5.0f - 5.0f, 0.0f, 0.0f));
	}

	sf::Vector2u v2WindowSize = m_pWindow->getSize();
	float fAspectRatio = (float)v2WindowSize.x / v2WindowSize.y;
	m_pCamera = new Camera(fAspectRatio, 60.0f);
	m_pCamera->GetTransform().MoveLocal(glm::vec3(0.85f, -0.25f, -5.0f));

	m_pSky = new SkyBox(cube);

	std::shared_ptr<Shader> pLineShader = std::make_shared<Shader>();
	pLineShader->CompileShader("shaders/LineVertex.glsl", "shaders/LineFragment.glsl");

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_InitForOpenGL(static_cast<HWND>(m_pWindow->getSystemHandle()));
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
 }

void Application::Update(void)
{
	// Setting the variable which holds the change 
	// in time between frames.  Also setting ImGui.
	float fDeltaTime = m_pTime.asSeconds();
	SetGUI(fDeltaTime);

	// Updating the camera.
	m_pCamera->Update(fDeltaTime, m_pWindow);

	// Rotating the active entities.
	for (int i = 0; i < m_lEntities.size(); i++)
	{
		Transform* t = m_lEntities[i]->GetTransform();
		t->Rotate(glm::vec3(0.0f, fDeltaTime / 2.0f, 0.0f));
	}
}

void Application::Render(void)
{
	// Setting the variable which holds the change in time between frames.
	float fDeltaTime = m_pTime.asSeconds();
	m_pSky->Render(m_pCamera);

	// Rendering all entities.
	for (int i = 0; i < m_lEntities.size(); i++)
	{
		m_lEntities[i]->Draw(m_pCamera);
	}

	// Rendering the ImGui interface.
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Application::~Application()
{
	// Freeing heap allocated variables.
	Realloc(m_pCamera);
	Realloc(m_pSky);
	for (int i = 0; i < m_lEntities.size(); i++)
	{
		Realloc(m_lEntities[i]);
	}

	// Releasing singletons.
	FileReader::GetInstance()->ReleaseInstance();
	
	// Clearing memory allocated by ImGui.
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// Freeing memory.
	Realloc(m_pWindow);
}

void Application::OnChangeScreenBounds(void)
{
	// Recalculating the Camera's projection matrix.
	sf::Vector2u v2WindowSize = m_pWindow->getSize();
	float fAspectRatio = (float)v2WindowSize.x / v2WindowSize.y;
	m_pCamera->UpdateProjection(fAspectRatio);

	std::cout << "Altering screen bounds" << std::endl;
}

void Application::SetGUI(float a_fDeltaTime)
{
	ImGuiIO& io = ImGui::GetIO();
	
	// Set mouse position for ImGui input handling.
	sf::Vector2i mousePos = sf::Mouse::getPosition(*m_pWindow);
	io.MousePos = ImVec2((float)mousePos.x, (float)mousePos.y);

	// Handle mouse button presses also for input handling.
	io.MouseDown[0] = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	io.MouseDown[1] = sf::Mouse::isButtonPressed(sf::Mouse::Right);
	io.MouseDown[2] = sf::Mouse::isButtonPressed(sf::Mouse::Middle);

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Beginning the debug window.
	ImGui::Begin("Debug Info");

	// Calculating the framerate of this application.
	static std::string sCurrentFPS = "0.0";
	static float totalTime = 0.0f;
	static int frameCount = 0;
	static float timer = 0.0f;
	totalTime += a_fDeltaTime;
	timer += a_fDeltaTime;
	frameCount++;

	// Recalculate the FPS every second.
	if (timer >= 1.0f)
	{
		sCurrentFPS = std::to_string(frameCount / totalTime);
		timer = 0.0f;
		totalTime = 0.0f;
		frameCount = 0;
	}

	// Displaying the resulting framerate.
	std::string displayText = "Framerate: ";
	displayText.append(sCurrentFPS);
	ImGui::Text(displayText.c_str());

	// Closing the window.
	ImGui::End();
}