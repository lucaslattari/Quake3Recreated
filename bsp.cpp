#include <cassert>
#include <limits>

#include "bsp.h"
#include "utils.h"
#include "BasicShapes.h"

//https://github.com/magnusgrander/SDL2_Quake3loader/blob/main/Quake3Bsp.cpp

namespace BSP {
	Loader::Loader()
	{
		header = { 0 };

		// Initialize lumps with default values
		std::fill_n(lumps, static_cast<int>(LUMPS::MAXLUMPS), LumpData{ 0, 0 });

		this->tesselationLevel = 20; //TODO: colocar essa informação em um config.txt no futuro
	}

	Loader::~Loader()
	{
		// Delete the unique VAO
		glDeleteVertexArrays(1, &faceVAO);

		// Se você tiver um VBO e EBO globais, também pode deletá-los aqui.
		glDeleteBuffers(1, &faceVBO);

		// Delete the unique VAO
		glDeleteVertexArrays(1, &faceVAO);

		glDeleteBuffers(1, &patchVBO);
		glDeleteVertexArrays(1, &patchVAO);
		glDeleteBuffers(1, &patchVBO);
		glDeleteBuffers(1, &patchEBO);
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

		// Inicialize as faces, criando VBOs e VAOs
		initializeFaces();
		initializeBezierPatches();

		file.close();

		return true;
	}

	void Loader::loadLumps(std::ifstream& file)
	{
		// Read the vertex data
		vertices.load(file, lumps[static_cast<int>(LUMPS::VERTICES)]);

		//vertices.displayData();
		//vertices.validate();
		//std::cin.get();

		// Read the face data
		faces.load(file, lumps[static_cast<int>(LUMPS::FACES)]);

		//faces.displayData();
		//faces.validate();
		//std::cin.get();

		// Read texture data
		textures.load(file, lumps[static_cast<int>(LUMPS::TEXTURES)]);

		//textures.displayData();
		//textures.validate();
		//std::cin.get();

		// Read lightmap data
		lightmaps.load(file, lumps[static_cast<int>(LUMPS::LIGHTMAPS)]);

		//lightmaps.displayData();
		//lightmaps.validate();
		//std::cin.get();

		// Read nodes data
		nodes.load(file, lumps[static_cast<int>(LUMPS::NODES)]);

		//nodes.displayData();
		//nodes.validate();
		//std::cin.get();

		// Read leaves data
		leaves.load(file, lumps[static_cast<int>(LUMPS::LEAVES)]);

		//leaves.displayData();
		//leaves.validate();
		//std::cin.get();

		// Read planes data
		planes.load(file, lumps[static_cast<int>(LUMPS::PLANES)]);

		//planes.displayData();
		//planes.validate();
		//std::cin.get();

		// Read pvs (potentially visible sets) data
		pvs.load(file, lumps[static_cast<int>(LUMPS::PVS)]);

		//pvs.displayData();
		//pvs.validate();
		//std::cin.get();

		// Read brushes data
		brushes.load(file, lumps[static_cast<int>(LUMPS::BRUSHES)]);

		//brushes.displayData();
		//brushes.validate();
		//std::cin.get();

		// Read brush sides data
		brushsides.load(file, lumps[static_cast<int>(LUMPS::BRUSH_SIDES)]);

		//brushsides.displayData();
		//brushsides.validate();
		//std::cin.get();

		// Read indices data
		indices.load(file,
			lumps[static_cast<int>(LUMPS::INDICES)].length,
			lumps[static_cast<int>(LUMPS::INDICES)].offset);

		//indices.displayData();
		//std::cin.get();

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

	void Loader::drawLevel(const Vec3<float>& vPos, GLuint shaderProgram) {
		this->shaderProgram = shaderProgram;
		// For each face
		for (int faceIndex = 0; faceIndex < faces.size(); faceIndex++) {
			const BSP::Face& face = faces.getData()[faceIndex];

			// Se a primeira flag estiver TRUE, renderiza polygon e mesh, mas não patches.
			if (renderPolygonsAndMeshes &&
				(face.getType() == BSP::FACE_POLYGON || face.getType() == BSP::FACE_MESH)) {
				drawFace(faceIndex);
			}

			// Se a segunda flag estiver TRUE, renderiza patches, mas não polygon e mesh.
			if (renderPatches && face.getType() == BSP::FACE_PATCH) {
				drawFace(faceIndex);
			}
		}
	}

	void Loader::initializeFaces() {
		std::vector<float> bufferVertexData;

		int totalPolygons = 0;
		int totalMeshes = 0;
		int totalPatches = 0;
		int totalBillboards = 0;

		// Loop through all the faces
		for (int faceIndex = 0; faceIndex < faces.size(); ++faceIndex) {
			const BSP::Face& face = faces.getData()[faceIndex];

			if (face.getType() == BSP::FACE_POLYGON)
				totalPolygons++;
			if (face.getType() == BSP::FACE_MESH)
				totalMeshes++;
			if (face.getType() == BSP::FACE_PATCH)
				totalPatches++;
			if (face.getType() == BSP::FACE_BBOARD)
				totalBillboards++;

			if (face.getType() == BSP::FACE_POLYGON ||
				face.getType() == BSP::FACE_MESH) {
				faceToOffsetMap[faceIndex] = bufferVertexData.size();

				int startMeshVert = face.getStartIndex();
				int endMeshVert = startMeshVert + face.getNumOfIndices();

				for (int index = startMeshVert; index < endMeshVert; index++) {
					int meshVertIndex = indices.getValues()[index];

					const Vertex& vertex = vertices.getData()[face.getStartVertIndex() + meshVertIndex];

					bufferVertexData.push_back(vertex.getPosition().x());
					bufferVertexData.push_back(vertex.getPosition().y());
					bufferVertexData.push_back(vertex.getPosition().z());

					for (int i = 0; i < 4; i++) {
						bufferVertexData.push_back(static_cast<float>(vertex.getColor()[i]) / 255.0f);
					}
				}
			}
		}

		std::cout << "total polygons:" << totalPolygons << std::endl;
		std::cout << "total meshes:" << totalMeshes << std::endl;
		std::cout << "total patches:" << totalPatches << std::endl;
		std::cout << "total billboards:" << totalBillboards << std::endl;

		glGenVertexArrays(1, &faceVAO);
		glBindVertexArray(faceVAO);

		glGenBuffers(1, &faceVBO);

		glBindBuffer(GL_ARRAY_BUFFER, faceVBO);
		glBufferData(GL_ARRAY_BUFFER, bufferVertexData.size() * sizeof(float),
			bufferVertexData.data(), GL_STATIC_DRAW);

		// Atributo para posição do vértice
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Atributo para cor do vértice
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	void Loader::initializeBezierPatches() {
		std::vector<float> bufferPatchVertexData;
		std::vector<GLuint> bufferPatchIndexData;

		// Contando o número de patches
		int numPatches = 0;
		for (int faceIndex = 0; faceIndex < faces.size(); ++faceIndex) {
			const BSP::Face& face = faces.getData()[faceIndex];
			if (face.getType() == FACE_PATCH)
				numPatches++;
		}

		// Reservando espaço para os dados de patch
		patches.reserve(numPatches);

		int accumulator;

		// Inicializando dados de patch
		for (int faceIndex = 0; faceIndex < faces.size(); ++faceIndex) {
			const BSP::Face& face = faces.getData()[faceIndex];
			if (face.getType() == FACE_PATCH) {
				patchToOffsetMap[faceIndex] = bufferPatchVertexData.size();
				indexPatchToOffsetMap[faceIndex] = bufferPatchIndexData.size();

				// Criar um objeto PatchData usando o construtor
				PatchData patchData(faceIndex, face.getTextureID(),
					face.getLightmapID(), face.getBezierPatchesSize()[0],
					face.getBezierPatchesSize()[1]);

				// Chamando uma função separada para inicializar os patches quadráticos
				initializeQuadraticPatches(patchData, face, this->tesselationLevel);
				patches.push_back(patchData);

				accumulator = 0;
				for (const QuadraticPatch& quadPatch : patchData.getQuadraticPatches()) {
					for (const Vertex& vertex : quadPatch.getVertices()) {
						bufferPatchVertexData.push_back(vertex.getPosition().x());
						bufferPatchVertexData.push_back(vertex.getPosition().y());
						bufferPatchVertexData.push_back(vertex.getPosition().z());

						for (int i = 0; i < 4; i++)
							bufferPatchVertexData.push_back(static_cast<float>(vertex.getColor()[i]) / 255.0f);
					}

					// Adicionando os índices de triângulo no EBO
					for (const int& index : quadPatch.getIndices().getValues()) {
						bufferPatchIndexData.push_back(index + patchToOffsetMap[faceIndex] / 7 + accumulator);
					}
					accumulator += (this->tesselationLevel + 1) * (this->tesselationLevel + 1);
				}
			}
		}

		// Geração e ligação do VAO para patches
		glGenVertexArrays(1, &patchVAO);
		glBindVertexArray(patchVAO);

		// Geração e ligação do VBO para patches
		glGenBuffers(1, &patchVBO);
		glBindBuffer(GL_ARRAY_BUFFER, patchVBO);

		// Geração e ligação do EBO para patches
		glGenBuffers(1, &patchEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, patchEBO);

		// Supondo que bufferPatchVertexData seja o vetor contendo seus dados de vértice de patch
		glBufferData(GL_ARRAY_BUFFER, bufferPatchVertexData.size() * sizeof(float),
			bufferPatchVertexData.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferPatchIndexData.size() * sizeof(GLuint),
			bufferPatchIndexData.data(), GL_STATIC_DRAW);

		// Atributo para posição do vértice
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Atributo para cor do vértice
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	void Loader::initializeQuadraticPatches(PatchData& patchData, const Face& face, int tesselationLevel) {
		int width = (patchData.getWidth() - 1) / 2;
		int height = (patchData.getHeight() - 1) / 2;
		patchData.resizeQuadraticPatches(width * height);

		// Índice base para os vértices desta face
		int baseVertIndex = face.getStartVertIndex();

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {

				// Obtendo a referência para o patch quadrático atual
				QuadraticPatch& quadPatch = patchData.getQuadraticPatches()[y * width + x];

				for (int r = 0; r < 3; r++) {
					for (int c = 0; c < 3; c++) {

						// Calculando o índice do vértice no vetor de vértices
						int vertIndex = baseVertIndex + (y * 2 * patchData.getWidth() + x * 2) +
							r * patchData.getWidth() + c;
						const Vertex& vertex = vertices.getData()[vertIndex];

						// Atribuindo valores aos pontos de controle
						quadPatch.getControlPoints()[r * 3 + c] = vertex;
					}
				}
				quadPatch.tesselate(tesselationLevel);
			}
		}
	}

	void Loader::drawFace(int faceIndex) {
		const BSP::Face& face = faces.getData()[faceIndex];

		if (face.getType() == BSP::FACE_POLYGON || face.getType() == BSP::FACE_MESH)
		{
			// Bind VAO
			glBindVertexArray(faceVAO);

			glDrawArrays(GL_TRIANGLES, static_cast<GLuint>(faceToOffsetMap[faceIndex] / 7),
				face.getNumOfIndices());

			// Desvincule o VAO
			glBindVertexArray(0);
		}
		else if (face.getType() == BSP::FACE_PATCH) {
			// TODO: Certifique-se de que o índice está correto aqui
			PatchData& currentPatch = patches[faceIndex];

			glBindVertexArray(patchVAO);
			checkGLError("glBindVertexArray");

			glBindBuffer(GL_ARRAY_BUFFER, patchVBO);
			checkGLError("glBindBuffer");

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, patchEBO);
			checkGLError("glBindBuffer");

			// Obter o índice inicial para este patch específico. Isso dependerá de como você armazenou seus dados.
			GLuint startIndex = indexPatchToOffsetMap[faceIndex]; // Suponho que isso dê o índice inicial
			GLuint accumulator = 0; // Acumulador para os índices já processados

			// Suponho que o índice inicial e a contagem de índices para cada patch estejam disponíveis.
			// Se não, você pode calcular com base no tamanho da tesselação ou outras métricas.
			for (auto& quadPatch : currentPatch.getQuadraticPatches()) {

				// Obter a contagem de índices para este patch
				GLsizei indexCount = quadPatch.getIndices().getValues().size();

				// Renderize usando glDrawElements
				glDrawElements(GL_TRIANGLES, indexCount,
					GL_UNSIGNED_INT, (void*)((startIndex + accumulator) * sizeof(GLuint)));

				// Atualiza o acumulador
				accumulator += indexCount;

				checkGLError("glDrawElements");
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			checkGLError("Unbind EBO");

			glBindVertexArray(0);
			checkGLError("Unbind VAO");
		}
	}

	void Loader::setTesselationLevel(int level) {
		this->tesselationLevel = level;
	}

	void Loader::debugEBO(GLuint ebo, size_t indexCount) {
		// Bind the EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		// Allocate memory to hold the EBO data
		std::vector<GLuint> indices(indexCount);

		// Retrieve data from the EBO
		glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexCount * sizeof(GLuint), &indices[0]);

		// Unbind the EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Print the EBO data
		for (size_t i = 0; i < indexCount; ++i) {
			std::cout << "Index at " << i << ": " << indices[i] << std::endl;
		}
	}

	void Loader::debugVBO(GLuint vbo, GLsizei size) {
		// Vincule o VBO
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// Aloque memória para armazenar os dados do VBO
		std::vector<float> bufferData(size);

		// Recupere os dados do VBO
		glGetBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(float), bufferData.data());

		// Desvincule o VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Imprima os dados recuperados
		std::cout << "VBO Data: " << std::endl;
		for (size_t i = 0; i < bufferData.size(); ++i) {
			std::cout << bufferData[i] << " ";
			if ((i + 1) % 7 == 0) {  // Supondo que cada vértice tenha 7 componentes (x, y, z, r, g, b, a)
				std::cout << std::endl;
			}
		}
	}
}