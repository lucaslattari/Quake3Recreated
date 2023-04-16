#include <cassert>
#include <limits>

#include "bsp.h"
#include "utils.h"

//https://github.com/magnusgrander/SDL2_Quake3loader/blob/main/Quake3Bsp.cpp

Quake3BSPLoader::Quake3BSPLoader()
{
	header = { 0 };

	// Initialize lumps with default values
	std::fill_n(lumps, MAX_LUMPS, BSPLumpData{ 0, 0 });
}

Quake3BSPLoader::~Quake3BSPLoader() 
{

}

bool Quake3BSPLoader::load(const std::string& filename)
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
	file.read(reinterpret_cast<char*>(&header), sizeof(BSPHeader));

	// Check if the header ID and version are correct
	if (strncmp(header.strID, "IBSP", 4) != 0 || header.version != 0x2e)
	{
		std::cout << "Invalid BSP header or unsupported version!" << std::endl;
		file.close();
		return false;
	}

	//BSP LUMPS
	file.read(reinterpret_cast<char*>(lumps), MAX_LUMPS * sizeof(BSPLumpData));

	// Read the vertex data
	//readVertexData(file);
	vertices.readData(file, lumps[VERTICES]);
	vertices.updateYAndZ();

	// Read the face data
	faces.readData(file, lumps[FACES]);

	// Read texture data
	textures.readData(file, lumps[TEXTURES]);

	file.close();

	return true;
}



void Quake3BSPLoader::displayHeaderData(BSPHeader& header) {
	std::cout << "BSP header data" << std::endl;
	std::cout << header.strID << std::endl;
	std::cout << header.version << std::endl;
}

void Quake3BSPLoader::displayLumpData(BSPLumpData(&lumps)[MAX_LUMPS]) {
	for (int i = 0; i < MAX_LUMPS; i++) {
		std::cout << "LUMP: " << i << std::endl;
		std::cout << "Offset: " << lumps[i].offset << std::endl;
		std::cout << "Length: " << lumps[i].length << std::endl;
	}
}