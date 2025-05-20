#include "Application.h"
#include "Debug.h"
#include <iostream>

int main()
{
	{
		// Creating the application.
		Application* app = new Application();
		app->Init("AshEngine");
		app->Run();

		// Clean up.
		Realloc(app);

		std::cout << "Ended execution" << std::endl;
		return 0;
	}

	if (_CrtDumpMemoryLeaks())
	{
		std::cout << "There are memory leaks present !!" << std::endl;
	}
}