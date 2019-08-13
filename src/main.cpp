#include <Engine.h>

int main()
{
	try
	{
		maj::Engine engine {};
		while (engine.isRunning())
		{
			engine.startFrame();
			engine.endFrame();
		}
	}
	catch (const std::exception& e)
	{
		LOG(LogError, e.what())
		system("pause");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}