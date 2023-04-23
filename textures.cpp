#include "Textures.h"

namespace BSP {
	void Textures::displayData() const {
		for (const auto& texture : elements) {
			std::cout << texture.name << std::endl;
			std::cout << texture.flags << std::endl;
			std::cout << texture.textureType << std::endl;
		}
	}

	//FIX ME: complete this function
	void Textures::validate() {
		for (const auto& texture : elements) {
		}
	}
}