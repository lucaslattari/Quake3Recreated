#include "Textures.h"
#include "utils.h"

namespace BSP {
	void Textures::displayData() const {
		for (const auto& texture : elements) {
			std::cout << texture.name << std::endl;
			std::cout << texture.flags << std::endl;
			std::cout << texture.textureType << std::endl;
		}
	}

	void Textures::validate() {
		for (const auto& texture : elements) {
			// Verificar se o nome da textura � v�lido (n�o vazio, por exemplo)
			warning_assert(texture.name[0] != '\0', "Texture name is empty.");

			// Verificar se os flags da textura s�o v�lidos (por exemplo, n�o negativos)
			warning_assert(texture.flags >= 0, "Texture flags are negative.");

			// TODO: descobrir qual � o intervalo m�ximo
			// Verificar se o tipo da textura est� dentro de um intervalo esperado
			//warning_assert(texture.textureType >= 1 && texture.textureType <= 4, "Texture type is out of the valid range.");
		}
	}
}