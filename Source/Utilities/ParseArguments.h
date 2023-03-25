#pragma once
#include "UltraEngine.h"

namespace UltraEngine
{
	extern void ParseArguments(int argc, const char* argv[]);
	extern const bool CheckArgument(const String& key);
	extern const String GetArgument(const String& key, const String& defaultvalue = "");

	extern const std::map<String, String> GetArguments();
	extern const std::vector<WString> GetDroppedFiles();
	extern const std::vector<WString> GetDroppedDirectories();
}