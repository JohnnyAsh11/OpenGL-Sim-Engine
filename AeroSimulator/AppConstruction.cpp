#include "Application.h"
#include "Debug.h"
#include <iostream>

Application::Application() {}

Application::Application(Application const& a_aInput)
{
	Realloc(m_pWindow);

	// Copying stack allocated data.
	m_v3Mouse = a_aInput.m_v3Mouse;
	m_bIsRunning = a_aInput.m_bIsRunning;

	// Deep copying the objects stored on the heap.
	m_pWindow = new sf::Window(
		sf::VideoMode(
			a_aInput.m_pWindow->getSize().x,
			a_aInput.m_pWindow->getSize().y),
		"COPY WINDOW",
		sf::Style::Default,
		sf::ContextSettings(24));
}

Application& Application::operator=(Application const& a_aInput)
{
	Realloc(m_pWindow);

	// Copying stack allocated data.
	m_v3Mouse = a_aInput.m_v3Mouse;
	m_bIsRunning = a_aInput.m_bIsRunning;

	// Deep copying the objects stored on the heap.
	m_pWindow = new sf::Window(
		sf::VideoMode(
			a_aInput.m_pWindow->getSize().x,
			a_aInput.m_pWindow->getSize().y),
		"COPY WINDOW",
		sf::Style::Default,
		sf::ContextSettings(24));

	// Returning this instance of the application.
	return *this;
}

void Application::ClearScreen(glm::vec4 a_v4ClearColor)
{
	// Clearing the render window.
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));
	GLCall(glClearColor(
		a_v4ClearColor.x,
		a_v4ClearColor.y,
		a_v4ClearColor.z,
		a_v4ClearColor.w));
}