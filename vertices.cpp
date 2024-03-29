#include "vertices.h"

namespace BSP {
	std::ostream& operator<<(std::ostream& os, const Vertex& vertex) {
		os << "Vertex Data: " << std::endl;
		os << "  Position: (" << vertex.getPosition().x() << ", " << vertex.getPosition().y() << ", " << vertex.getPosition().z() << ")" << std::endl;
		os << "  Texture Coord: (" << vertex.getTextureCoord().x() << ", " << vertex.getTextureCoord().y() << ")" << std::endl;
		os << "  Lightmap Coord: (" << vertex.getLightmapCoord().x() << ", " << vertex.getLightmapCoord().y() << ")" << std::endl;
		os << "  Normal: (" << vertex.getNormal().x() << ", " << vertex.getNormal().y() << ", " << vertex.getNormal().z() << ")" << std::endl;
		os << "  Color: (" << static_cast<int>(vertex.getColor()[0]) << ", " << static_cast<int>(vertex.getColor()[1]) << ", " << static_cast<int>(vertex.getColor()[2]) << ", " << static_cast<int>(vertex.getColor()[3]) << ")";
		return os;
	}

	void Vertices::load(std::ifstream& file, LumpData& lumpData) {
		Element::load(file, lumpData);
		updateYAndZ();
	}

	// Swap the y and z values, and negate the new z so Y is up 
	// the swapping of y and z values and negating the new z value is related 
	// to the differences in coordinate systems used in the Quake 3 engine and 
	// most 3D modeling software
	void Vertices::updateYAndZ() {
		for (auto& vertex : elements) {
			Vec3<float> newPosition = vertex.getPosition();

			newPosition.y() = vertex.getPosition().z(); // Access z using () instead of .z
			newPosition.z() = -vertex.getPosition().y(); // Access y using () instead of .y

			vertex.setPosition(newPosition);
		}
	}

	void Vertices::displayData() const {
		std::cout << "Vertex Data:" << std::endl;
		for (size_t i = 0; i < elements.size(); i++) {
			std::cout << "Vertex " << i << ":" << std::endl;
			std::cout << "  Position: (" << elements[i].getPosition().x() << ", " << elements[i].getPosition().y() << ", " << elements[i].getPosition().z() << ")" << std::endl;
			std::cout << "  Texture Coord: (" << elements[i].getTextureCoord().x() << ", " << elements[i].getTextureCoord().y() << ")" << std::endl;
			std::cout << "  Lightmap Coord: (" << elements[i].getLightmapCoord().x() << ", " << elements[i].getLightmapCoord().y() << ")" << std::endl;
			std::cout << "  Normal: (" << elements[i].getNormal().x() << ", " << elements[i].getNormal().y() << ", " << elements[i].getNormal().z() << ")" << std::endl;
			std::cout << "  Color: (" << static_cast<int>(elements[i].getColor()[0]) << ", " << static_cast<int>(elements[i].getColor()[1]) << ", " << static_cast<int>(elements[i].getColor()[2]) << ", " << static_cast<int>(elements[i].getColor()[3]) << ")" << std::endl;
		}
	}

	void Vertices::validate() {
		constexpr float minPosition = -std::numeric_limits<float>::max(); //max, dont change
		constexpr float maxPosition = std::numeric_limits<float>::max();

		for (const auto& vertex : elements) {
			warning_assert(vertex.getPosition().x() >= minPosition && vertex.getPosition().x() <= maxPosition,
				"Position x out of range");
			warning_assert(vertex.getPosition().y() >= minPosition && vertex.getPosition().y() <= maxPosition,
				"Position y out of range");
			warning_assert(vertex.getPosition().z() >= minPosition && vertex.getPosition().z() <= maxPosition,
				"Position z out of range");

			int mapWidth = 2048; //a huge value for a quake 3 texture width
			int mapHeight = 2048;
			warning_assert(vertex.getTextureCoord().x() >= -((float)(mapWidth / 64)) && vertex.getTextureCoord().x() <= (float)(mapWidth / 64),
				"Texture coordinate x is out of the allowed range for the map width.");

			warning_assert(vertex.getTextureCoord().y() >= -((float)(mapHeight / 64)) && vertex.getTextureCoord().y() <= (float)(mapHeight / 64),
				"Texture coordinate y is out of the allowed range for the map height.");

			Vec3f copiedNormal = vertex.getNormal().normalize();
			warning_assert(copiedNormal.x() >= -1.0f && copiedNormal.x() <= 1.0f,
				"Normalized normal x component is out of the range [-1.0, 1.0].");

			warning_assert(copiedNormal.y() >= -1.0f && copiedNormal.y() <= 1.0f,
				"Normalized normal y component is out of the range [-1.0, 1.0].");

			warning_assert(copiedNormal.z() >= -1.0f && copiedNormal.z() <= 1.0f,
				"Normalized normal z component is out of the range [-1.0, 1.0].");
		}
	}
}