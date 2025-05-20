#include "Application.h"
#include "Debug.h"
#include "Colors.h"

void Application::Run(void)
{
	// Starting up the loop with the control variable.
	m_bIsRunning = true;

	// Setting the clock for calculating delta time between frames.
	sf::Clock clock = sf::Clock();
	while (m_bIsRunning)
	{
		// Getting the delta time.
		m_pTime = clock.restart();

		// Handling window events.
		sf::Event event;
		while (m_pWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				// Closing the application.
				m_bIsRunning = false;
			}
			else if (event.type == sf::Event::Resized)
			{
				// Adjusting the viewport when the window is resized.
				GLCall(glViewport(0, 0, event.size.width, event.size.height));

				// Calling the window resizing callback method.
				this->OnChangeScreenBounds();
			}
		}

		// Quits out of the game.
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		{
			m_bIsRunning = false;
		}

		// Clearing the previous frame.
		this->ClearScreen(CORNFLOWER_BLUE);

		// Calling the logic update method.
		this->Update();

		// Coloring and drawing.
		this->Render();

		// Ending the current frame (internally swaps the front and back buffers)
		m_pWindow->display();
	}
}

void Application::InitWindow()
{
	m_pWindow->setVerticalSyncEnabled(false);

	// Initializing GLEW.
	glewExperimental = GL_TRUE;
	GLCall(glewInit());

	// Enabling pixel blending and its mode.
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	// Enabling depth testing and its mode.
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LEQUAL));

	// Disallows the faces from being viewed from behind.
	GLCall(glEnable(GL_CULL_FACE));
}