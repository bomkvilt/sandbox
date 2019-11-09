#include "translator.hpp"
#include <fstream>
#include <sstream>


int main()
{
	auto in  = std::ifstream("in.txt");
	auto out = std::ofstream("out.txt");
	if (!in.is_open() || !out.is_open())
	{
		return 1;
	}

	float l = 0, f = 0, a = 0, r = 0;
	in >> l >> f >> a >> r;
	auto T = Translator(l, f, a, r);

	std::string line;
	while (std::getline(in, line))
	{
		std::stringstream ss(line);
		TVector<float> p; ss >> p.x >> p.y >> p.z;
		TVector<float> v; ss >> v.x >> v.y >> v.z;

		p = T.Position(p);
		v = T.Velocity(v);
		out << p.x << " " << p.y << " " << p.z << "\t" << v.x << " " << v.y << " " << v.z << "\n";
	}
}
