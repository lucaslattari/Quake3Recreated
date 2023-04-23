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

		loadLumps(file);
	
		file.close();

		return true;
	}

	void Loader::loadLumps(std::ifstream& file)
	{
		// Read the vertex data
		vertices.load(file, lumps[static_cast<int>(LUMPS::VERTICES)]);

		// Read the face data
		faces.load(file, lumps[static_cast<int>(LUMPS::FACES)]);

		// Read texture data
		textures.load(file, lumps[static_cast<int>(LUMPS::TEXTURES)]);

		// Read lightmap data
		lightmaps.load(file, lumps[static_cast<int>(LUMPS::LIGHTMAPS)]);

		// Read nodes data
		nodes.load(file, lumps[static_cast<int>(LUMPS::NODES)]);

		// Read leaves data
		leaves.load(file, lumps[static_cast<int>(LUMPS::LEAVES)]);

		// Read planes data
		planes.load(file, lumps[static_cast<int>(LUMPS::PLANES)]);

		// Read pvs (potentially visible sets) data
		pvs.load(file, lumps[static_cast<int>(LUMPS::PVS)]);

		// Read brushes data
		brushes.load(file, lumps[static_cast<int>(LUMPS::BRUSHES)]);

		// Read brush sides data
		brushsides.load(file, lumps[static_cast<int>(LUMPS::BRUSH_SIDES)]);

		// Read indices data
		indices.load(file, 
			lumps[static_cast<int>(LUMPS::INDICES)].length,
			lumps[static_cast<int>(LUMPS::INDICES)].offset);

		// Read leaf faces data
		leafFaces.load(file,
			lumps[static_cast<int>(LUMPS::LEAF_FACES)].length,
			lumps[static_cast<int>(LUMPS::LEAF_FACES)].offset);

		// Read leaf brushes data
		leafBrushes.load(file,
			lumps[static_cast<int>(LUMPS::LEAF_BRUSHES)].length,
			lumps[static_cast<int>(LUMPS::LEAF_BRUSHES)].offset);
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