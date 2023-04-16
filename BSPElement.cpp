#include "BSPElement.h"
#include "utils.h"

// Swap the y and z values, and negate the new z so Y is up 
// the swapping of y and z values and negating the new z value is related 
// to the differences in coordinate systems used in the Quake 3 engine and 
// most 3D modeling software
void BSPVertices::updateYAndZ() {
	for (int i = 0; i < elements.size(); i++) {
		float temp = elements[i].position.y;
		elements[i].position.y = elements[i].position.z;
		elements[i].position.z = -temp;
	}
}

void BSPVertices::displayData() {
	std::cout << "Vertex Data:" << std::endl;
	for (size_t i = 0; i < elements.size(); i++) {
		std::cout << "Vertex " << i << ":" << std::endl;
		std::cout << "  Position: (" << elements[i].position.x << ", " << elements[i].position.y << ", " << elements[i].position.z << ")" << std::endl;
		std::cout << "  Texture Coord: (" << elements[i].textureCoord.x << ", " << elements[i].textureCoord.y << ")" << std::endl;
		std::cout << "  Lightmap Coord: (" << elements[i].lightmapCoord.x << ", " << elements[i].lightmapCoord.y << ")" << std::endl;
		std::cout << "  Normal: (" << elements[i].normal.x << ", " << elements[i].normal.y << ", " << elements[i].normal.z << ")" << std::endl;
		std::cout << "  Color: (" << static_cast<int>(elements[i].color[0]) << ", " << static_cast<int>(elements[i].color[1]) << ", " << static_cast<int>(elements[i].color[2]) << ", " << static_cast<int>(elements[i].color[3]) << ")" << std::endl;
	}
}

void BSPVertices::validateData() {
	float minPosition = -std::numeric_limits<float>::max();
	float maxPosition = std::numeric_limits<float>::max();

	for (const auto& vertex : elements) {
		warning_assert(vertex.position.x >= minPosition && vertex.position.x <= maxPosition,
			"Position x out of range");
		warning_assert(vertex.position.y >= minPosition && vertex.position.y <= maxPosition,
			"Position y out of range");
		warning_assert(vertex.position.z >= minPosition && vertex.position.z <= maxPosition,
			"Position z out of range");

		int mapWidth = 2048; //a huge value for a quake 3 texture width
		int mapHeight = 2048;
		warning_assert(vertex.textureCoord.x >= -((float)(mapWidth / 64)) && vertex.textureCoord.x <= (float)(mapWidth / 64),
			"Texture coordinate x is out of the allowed range for the map width.");

		warning_assert(vertex.textureCoord.y >= -((float)(mapWidth / 64)) && vertex.textureCoord.y <= (float)(mapHeight / 64),
			"Texture coordinate y is out of the allowed range for the map height.");

		Vector<float, 3> copiedNormal = vertex.normal.normalized();
		warning_assert(copiedNormal.x >= -1.0f && copiedNormal.x <= 1.0f,
			"Normalized normal x component is out of the range [-1.0, 1.0].");

		warning_assert(copiedNormal.y >= -1.0f && copiedNormal.y <= 1.0f,
			"Normalized normal y component is out of the range [-1.0, 1.0].");

		warning_assert(copiedNormal.z >= -1.0f && copiedNormal.z <= 1.0f,
			"Normalized normal z component is out of the range [-1.0, 1.0].");
	}
}

void BSPFaces::validateData() {
	for (const auto& face : elements) {
		warning_assert(face.textureID >= 0, "TextureID is negative.");

		warning_assert(face.effect >= -1, "Effect is less than -1.");

		warning_assert(face.type >= 1 && face.type <= 4, "Face type is out of the valid range.");

		warning_assert(face.startVertIndex >= 0, "startVertIndex is negative.");
		warning_assert(face.numOfVerts >= 0, "numOfVerts is negative.");
		warning_assert(face.startIndex >= 0, "startIndex is negative.");
		warning_assert(face.numOfIndices >= 0, "numOfIndices is negative.");

		warning_assert(face.lightmapID >= 0, "LightmapID is negative.");

		warning_assert(face.lightMapCorner[0] >= 0 && face.lightMapCorner[1] >= 0,
			"Invalid lightMapCorner values.");
		warning_assert(face.lightMapSize[0] >= 0 && face.lightMapSize[1] >= 0,
			"Invalid lightMapSize values.");

		warning_assert(face.size[0] >= 0 && face.size[1] >= 0, "Invalid face size values.");

		warning_assert(face.normal.length() <= 1.0f, "Normal vector magnitude is greater than 1.0f.");

		for (int i = 0; i < 2; ++i) {
			float vecMagnitude = std::sqrt(face.lightMapVecs[i].x * face.lightMapVecs[i].x +
				face.lightMapVecs[i].y * face.lightMapVecs[i].y +
				face.lightMapVecs[i].z * face.lightMapVecs[i].z);
			warning_assert(vecMagnitude <= 1.0f,
				"LightMapVecs s and t unit vectors magnitude is greater than 1.0f.");
		}
	}
}

//FIX ME: complete this function
void BSPFaces::displayData() {
	for (const auto& face : elements) {
	}
}

void BSPTextures::displayData() {
	for (const auto& texture : elements) {
		std::cout << texture.name << std::endl;
		std::cout << texture.flags << std::endl;
		std::cout << texture.textureType << std::endl;
	}
}

//FIX ME: complete this function
void BSPTextures::validateData() {
	for (const auto& texture : elements){
	}
}