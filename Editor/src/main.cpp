#include <Application.h>
#include <Logging.h>

int main(int argc, char** argv)
{
	int returnValue{ 0u };
	try 
	{
		maj::editor::Application app(argc, argv);
		returnValue = app.exec();
	}
	catch (const std::exception& e)
	{
		LOG(LogError, e.what())
		return returnValue;
	}

	return EXIT_SUCCESS;
}