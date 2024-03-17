/// \ project: Synergy
/// \ file: FileDialogue.h
/// \ author: Thomas Clermont

#pragma once
#include <string>


namespace SNG
{

	class FileDialogue
	{
	public:

		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};

}
