/// \ project: RayTracing
/// \ file: MainApp.cpp
/// \ authot: Thomas Clermont

#include <Core/Application.h>
#include "AppLayer.h"



SNG::Application* SNG::CreateApplication()
{
	SNG::AppInfo appInfo{};
	appInfo.Name = "Synergy - RayTracing";
	appInfo.Width = 1600;
	appInfo.Height = 900;

	Application* app = new Application(appInfo);
	app->PushLayer<AppLayer>();

	return app;
}

