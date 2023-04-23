#include "lightmaps.h"
#include "utils.h"

namespace BSP {
	void Lightmaps::displayData() const {
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

	void Lightmaps::validate() {
		for (const auto& lightmap : elements) {
			for (int i = 0; i < 128; i++) {
				for (int j = 0; j < 128; j++) {
					const auto& pixel = lightmap.imageBits[i][j];
					// Check if each color channel value is between 0 and 255
					for (int k = 0; k < 3; k++) {
						warning_assert(pixel[k] >= 0 && pixel[k] <= 255, "Invalid RGB value.");
					}
				}
			}
		}
	}
}