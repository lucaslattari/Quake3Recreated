#include <cassert>
#include <limits>

#include "bsp.h"

//https://github.com/magnusgrander/SDL2_Quake3loader/blob/main/Quake3Bsp.cpp

Quake3BSPLoader::Quake3BSPLoader()
{
	header = { 0 };

	// Initialize lumps with default values
	std::fill_n(lumps, MAX_LUMPS, BSPLump{ 0, 0 });
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
	file.read(reinterpret_cast<char*>(lumps), MAX_LUMPS * sizeof(BSPLump));

	// Read the vertex data
	readVertexData(file);

	// Read the face data
	readFaceData(file);

	file.close();

	return true;
}

void Quake3BSPLoader::readVertexData(std::ifstream& file) {
	// Allocate the vertex memory
	int numberOfVertices = lumps[VERTICES].length / sizeof(BSPVertex) + 1;
	vertices.resize(numberOfVertices);

	// Seek to the vertices lump and read the vertex data into the vector
	file.seekg(lumps[VERTICES].offset, std::ios::beg);
	file.read(reinterpret_cast<char*>(vertices.data()), lumps[VERTICES].length);

	// Validate the vertex data
	//validateVertexData();

	// Swap the y and z values, and negate the new z so Y is up 
	// the swapping of y and z values and negating the new z value is related 
	// to the differences in coordinate systems used in the Quake 3 engine and 
	// most 3D modeling software
	for (int i = 0; i < numberOfVertices; i++) {
		float temp = vertices[i].position.y;
		vertices[i].position.y = vertices[i].position.z;
		vertices[i].position.z = -temp;
	}
}

void Quake3BSPLoader::readFaceData(std::ifstream& file) {
	// Allocate the faces memory
	int numberOfFaces = lumps[FACES].length / sizeof(BSPFace) + 1;
	faces.resize(numberOfFaces);

	// Seek to the faces lump and read the face data into the vector
	file.seekg(lumps[FACES].offset, std::ios::beg);
	file.read(reinterpret_cast<char*>(faces.data()), lumps[FACES].length);

	// Validate the face data
	//validateFaceData();
}

void Quake3BSPLoader::displayHeaderData(BSPHeader& header) {
	std::cout << "BSP header data" << std::endl;
	std::cout << header.strID << std::endl;
	std::cout << header.version << std::endl;
}

void Quake3BSPLoader::displayLumpData(BSPLump(&lumps)[MAX_LUMPS]) {
	for (int i = 0; i < MAX_LUMPS; i++) {
		std::cout << "LUMP: " << i << std::endl;
		std::cout << "Offset: " << lumps[i].offset << std::endl;
		std::cout << "Length: " << lumps[i].length << std::endl;
	}
}

void Quake3BSPLoader::displayVertexData() {
	std::cout << "Vertex Data:" << std::endl;
	for (size_t i = 0; i < vertices.size(); i++) {
		std::cout << "Vertex " << i << ":" << std::endl;
		std::cout << "  Position: (" << vertices[i].position.x << ", " << vertices[i].position.y << ", " << vertices[i].position.z << ")" << std::endl;
		std::cout << "  Texture Coord: (" << vertices[i].textureCoord.x << ", " << vertices[i].textureCoord.y << ")" << std::endl;
		std::cout << "  Lightmap Coord: (" << vertices[i].lightmapCoord.x << ", " << vertices[i].lightmapCoord.y << ")" << std::endl;
		std::cout << "  Normal: (" << vertices[i].normal.x << ", " << vertices[i].normal.y << ", " << vertices[i].normal.z << ")" << std::endl;
		std::cout << "  Color: (" << static_cast<int>(vertices[i].color[0]) << ", " << static_cast<int>(vertices[i].color[1]) << ", " << static_cast<int>(vertices[i].color[2]) << ", " << static_cast<int>(vertices[i].color[3]) << ")" << std::endl;
	}
}

void Quake3BSPLoader::validateVertexData() {
	float minPosition = -std::numeric_limits<float>::max();
	float maxPosition = std::numeric_limits<float>::max();

	for (const auto& vertex : vertices) {
		//FIX ME: Update to an assert that does not close the program
		//assert(vertex.position.x >= minPosition && vertex.position.x <= maxPosition);
		//assert(vertex.position.y >= minPosition && vertex.position.y <= maxPosition);
		//assert(vertex.position.z >= minPosition && vertex.position.z <= maxPosition);

		int mapWidth = 2048; //a huge value for a quake 3 texture width
		int mapHeight = 2048;
		//assert(vertex.textureCoord.x >= -((float)(mapWidth / 64)) && vertex.textureCoord.x <= (float)(mapWidth / 64));
		//assert(vertex.textureCoord.y >= -((float)(mapWidth / 64)) && vertex.textureCoord.y <= (float)(mapHeight / 64));

		Vector<float, 3> copiedNormal = vertex.normal.normalized();
		//assert(copiedNormal.x >= -1.0f && copiedNormal.x <= 1.0f);
		//assert(copiedNormal.y >= -1.0f && copiedNormal.y <= 1.0f);
		//assert(copiedNormal.z >= -1.0f && copiedNormal.z <= 1.0f);
	}
}

void Quake3BSPLoader::validateFaceData() {
	/*for (const auto& face : faces) {
		// Asserting textureID is non-negative
		assert(face.textureID >= 0);

		// Asserting effect is non-negative or -1
		assert(face.effect >= -1);

		// Asserting type is within the valid range
		assert(face.type >= 1 && face.type <= 4);

		// Asserting startVertIndex, numOfVerts, startIndex, and numOfIndices are non-negative
		assert(face.startVertIndex >= 0);
		assert(face.numOfVerts >= 0);
		assert(face.startIndex >= 0);
		assert(face.numOfIndices >= 0);

		// Asserting lightmapID is non-negative
		assert(face.lightmapID >= 0);

		// Asserting lightMapCorner and lightMapSize have valid values
		assert(face.lightMapCorner[0] >= 0 && face.lightMapCorner[1] >= 0);
		assert(face.lightMapSize[0] >= 0 && face.lightMapSize[1] >= 0);

		// Asserting size has valid values
		assert(face.size[0] >= 0 && face.size[1] >= 0);

		// No specific range for lightMapPos, so no assert check needed

		// Asserting the normal vector has a magnitude less than or equal to 1.0f
		assert(face.normal.length() <= 1.0f);

		// Asserting lightMapVecs s and t unit vectors have magnitudes less than or equal to 1.0f
		for (int i = 0; i < 2; ++i) {
			float vecMagnitude = std::sqrt(face.lightMapVecs[i].x * face.lightMapVecs[i].x +
				face.lightMapVecs[i].y * face.lightMapVecs[i].y +
				face.lightMapVecs[i].z * face.lightMapVecs[i].z);
			assert(vecMagnitude <= 1.0f);
		}
	}*/
}