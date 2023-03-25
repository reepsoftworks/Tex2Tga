#include "UltraEngine.h"

namespace UltraEngine
{
	std::map<String, String> Arguments;
	std::vector<WString> DroppedFiles;
	std::vector<WString> DroppedDirectories;

	void ParseArguments(int argc, const char* argv[])
	{
		// 221224: Came back in here to make both flags set with '-' and '+' accept a value.
		// Only diffrence is that flags with '+' do not get a default value set.
		for (int i = 1; i < argc; i++)
		{
			auto key = String(argv[i]);
			if (!key.empty())
			{
				if (key[0] == '-')
				{
					// Remove the '-'
					key.erase(
						remove(key.begin(), key.end(), '-'),
						key.end()
					);

					auto value = String(argv[i + 1]);
					if (!value.empty() && value[0] != '+' && value[0] != '-')
					{
						Arguments[key] = value;
					}
					else
					{
						Arguments[key] = "1"; // The flag just existing will have a default of "1" if no value is present..
					}
				}
				else if (key[0] == '+')
				{
					// Remove the '+'
					key.erase(
						remove(key.begin(), key.end(), '+'),
						key.end()
					);

					auto value = String(argv[i + 1]);
					if (!value.empty() && value[0] != '+' && value[0] != '-')
					{
						Arguments[key] = value;
					}					
				}
				else if (FileType(argv[i]) != 0)
				{
					auto entry = WString(argv[i]);
					entry = FixPath(entry);
					if (FileType(entry) == 2)
						DroppedDirectories.push_back(entry);
					else
						DroppedFiles.push_back(entry);
				}
			}
		}
	}

	const bool CheckArgument(const String& key)
	{
		return !Arguments[key].empty();
	}

	const String GetArgument(const String& key, const String& defaultvalue)
	{
		if (Arguments[key].empty()) return defaultvalue;
		return Arguments[key];
	}

	std::map<String, String> GetArguments()
	{
		return Arguments;
	}

	const std::vector<WString> GetDroppedFiles()
	{
		return DroppedFiles;
	}

	const std::vector<WString> GetDroppedDirectories()
	{
		return DroppedDirectories;
	}
}