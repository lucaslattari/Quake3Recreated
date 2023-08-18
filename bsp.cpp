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
		for (const auto& pair : m_VAOs) {
			GLuint VAO = pair.second;
			glDeleteVertexArrays(1, &VAO);
		}
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

		// Inicialize as faces, criando VBOs, VAOs e EBOs
		initializeFaces();
	
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

	void Loader::drawLevel(const Vec3<float>& vPos) {
		// For each face
		for (int faceIndex = 0; faceIndex < faces.size(); faceIndex++) {
			if (faces.getData()[faceIndex].getType() != FaceType::FACE_POLYGON)
				continue;

			drawFace(faceIndex);
		}
	}

	void Loader::initializeFaces() {
		// Percorra todas as faces
		for (int faceIndex = 0; faceIndex < faces.size(); faceIndex++) {

			const BSP::Face& face = faces.getData()[faceIndex];

			if (face.getType() == BSP::FACE_POLYGON) {
				int start = face.getStartVertIndex();
				int count = face.getNumOfVerts();

				std::vector<Vertex> verticesForThisFace(vertices.getData().begin() + start, vertices.getData().begin() + start + count);

				std::vector<int> indices = this->indices.getValues();
				std::vector<float> vertexData;
				for (const auto& vertex : verticesForThisFace) {
					vertexData.push_back(vertex.getPosition().x());
					vertexData.push_back(vertex.getPosition().y());
					vertexData.push_back(vertex.getPosition().z());

					// Adicione outros atributos, como coordenadas de textura, se desejar
					// Adicione a cor, assumindo que está armazenada como RGBA (0-255)
					for (int i = 0; i < 4; i++) {
						vertexData.push_back(static_cast<float>(vertex.getColor()[i]) / 255.0f);
					}
				}

				GLuint VAO, VBO, EBO;
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				glGenBuffers(1, &EBO);

				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

				// Configurar atributos do vértice para a posição
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);

				// Configurar atributos do vértice para a cor
				glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);

				// Desvincular o VBO e VAO (opcional, mas uma boa prática)
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);

				m_VAOs[faceIndex] = VAO;
			}
		}
	}

	void Loader::drawFace(int faceIndex) {
		if (faces.getData()[faceIndex].getType() != FaceType::FACE_POLYGON)
			return;

		glBindVertexArray(m_VAOs[faceIndex]); // Ligar VAO
		glDrawElements(GL_TRIANGLES, this->indices.getValues().size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0); // Desligar VAO
	}
}