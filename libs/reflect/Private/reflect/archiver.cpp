#include "reflect/archiver.hpp"
#include <boost/property_tree/json_parser.hpp>
#include <sstream>


namespace reflect
{
	Archiver& Archiver::Load(const std::string& json)
	{
		auto ss = std::stringstream(json);
		pt::read_json(ss, tree);
		return *this;
	}

	std::string Archiver::Save() const
	{
		auto ss = std::stringstream();
		pt::write_json(ss, tree);
		return ss.str();
	}
}
