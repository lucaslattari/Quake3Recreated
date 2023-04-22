#include <cassert>
#include <limits>

#include "bsp.h"
#include "utils.h"

//https://github.com/magnusgrander/SDL2_Quake3loader/blob/main/Quake3Bsp.cpp

namespace BSP {
	Loader::Loader()
	{
		header = { 0 };

		// Initialize lumps with default values
		std::fill_n(lumps, static_cast<int>(LUMPS::MAXLUMPS), LumpData{ 0, 0 });
	}

	Loader::~Loader()
	{

	}

	bool Loader::load(const std::string& filename)
	{
		std::ifstream file;
		file.open(filename, std::ios::binary);

		// Check if the .bsp file could be opened
		if (!file.is_open())
		{
			std::cout << "Could not find or open BSP file: " << filename << std::endl;
			return false;
		}
		else
		{
			std::cout << "File '" << filename << "' found successfully!" << std::endl;
		}

		//BSP HEADER
		file.read(reinterpret_cast<char*>(&header), sizeof(Header));

		// Check if the header ID and version are correct
		if (strncmp(header.strID, "IBSP", 4) != 0 || header.version != 0x2e)
		{
			std::cout << "Invalid BSP header or unsupported version!" << std::endl;
			file.close();
			return false;
		}

		//BSP LUMPS
		file.read(reinterpret_cast<char*>(lumps), 
				  static_cast<int>(LUMPS::MAXLUMPS) * sizeof(LumpData));

		// Read the vertex data
		vertices.readData(file, lumps[static_cast<int>(LUMPS::VERTICES)]);
		vertices.updateYAndZ();

		// Read the face data
		faces.readData(file, lumps[static_cast<int>(LUMPS::FACES)]);

		// Read texture data
		textures.readData(file, lumps[static_cast<int>(LUMPS::TEXTURES)]);

		// Read lightmap data
		lightmaps.readData(file, lumps[static_cast<int>(LUMPS::LIGHTMAPS)]);

		// Read nodes data
		nodes.readData(file, lumps[static_cast<int>(LUMPS::NODES)]);

		// Read leaves data
		leaves.readData(file, lumps[static_cast<int>(LUMPS::LEAVES)]);

		// Read planes data
		planes.readData(file, lumps[static_cast<int>(LUMPS::PLANES)]);

		file.close();

		return true;
	}

	void Loader::displayHeaderData(Header& header) {
		std::cout << "BSP header data" << std::endl;
		std::cout << header.strID << std::endl;
		std::cout << header.version << std::endl;
	}

	void Loader::displayLumpData(LumpData(&lumps)[static_cast<int>(LUMPS::MAXLUMPS)]) {
		for (int i = 0; i < static_cast<int>(LUMPS::MAXLUMPS); i++) {
			std::cout << "LUMP: " << i << std::endl;
			std::cout << "Offset: " << lumps[i].offset << std::endl;
			std::cout << "Length: " << lumps[i].length << std::endl;
		}
	}
}