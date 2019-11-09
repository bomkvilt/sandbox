#include "config/config.hpp"
#include <fstream>


namespace reflect
{
	bool FConfig::LoadConfig(const std::string& path)
	{
		if (auto file = std::ifstream(path))
		{
			auto json = std::string(std::istreambuf_iterator(file), {});
			auto arch = Archiver();
			arch.Load(json);
			Unmarshal(arch);
			return true;
		}
		else return false;
	}

	bool FConfig::SaveConfig(const std::string& path) const
	{
		if (auto file = std::ofstream(path))
		{
			auto arch = Archiver();
			Marshal(arch);
			auto json = arch.Save();
			file << json;
			return true;
		}
		else return false;
	}
}
