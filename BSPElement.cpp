#include "BSPElement.h"
#include "utils.h"

void BSPTextures::displayData() const {
	for (const auto& texture : elements) {
		std::cout << texture.name << std::endl;
		std::cout << texture.flags << std::endl;
		std::cout << texture.textureType << std::endl;
	}
}

//FIX ME: complete this function
void BSPTextures::validateData() {
	for (const auto& texture : elements) {
	}
}

void BSPLightmaps::displayData() const {
	for (const auto& lightmap : elements) {
		for (const auto& row : lightmap.imageBits) {
			for (const auto& pixel : row) {
				// Access each color channel of the pixel
				char red = pixel[0];
				char green = pixel[1];
				char blue = pixel[2];

				// Print each color channel value, separated by commas
				std::cout << "(" << static_cast<int>(red) << ", "
					<< static_cast<int>(green) << ", "
					<< static_cast<int>(blue) << ") ";
			}
			// Print a newline at the end of each row
			std::cout << std::endl;
		}
		// Print a separator between lightmaps
		std::cout << "----------" << std::endl;
	}
}

//FIX ME: complete this function
void BSPLightmaps::validateData() {
	for (const auto& texture : elements) {
	}
}