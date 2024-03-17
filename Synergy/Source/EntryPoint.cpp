/// \ project: Synergy-RayTracing
/// \ file: EntryPoint.cpp
/// \ author: Thomas Clermont

#include "Core/Application.h"


extern SNG::Application* SNG::CreateApplication();


int main()
{
	SNG::Application* app = SNG::CreateApplication();
	app->Run();

	delete app;
	return EXIT_SUCCESS;
}
