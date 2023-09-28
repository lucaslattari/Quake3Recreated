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
		// Delete the unique VAO
		glDeleteVertexArrays(1, &globalVAO);

		// Se você tiver um VBO e EBO globais, também pode deletá-los aqui.
		glDeleteBuffers(1, &globalVBO);
		//glDeleteBuffers(1, &globalEBO);
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

	void Loader::drawLevel(const Vec3<float>& vPos) {
		// For each face
		for (int faceIndex = 0; faceIndex < faces.size(); faceIndex++) {
			if (faces.getData()[faceIndex].getType() != BSP::FACE_POLYGON && faces.getData()[faceIndex].getType() != BSP::FACE_MESH)
				continue;

			drawFace(faceIndex);
		}
	}

	void Loader::initializeFaces() {
		std::vector<float> bufferVertexData;
		std::vector<GLuint> bufferIndicesData;

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

			if (face.getType() == BSP::FACE_POLYGON || face.getType() == BSP::FACE_MESH) {
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

		glGenVertexArrays(1, &globalVAO);
		glBindVertexArray(globalVAO);

		glGenBuffers(1, &globalVBO);

		// Imprime o conteúdo de bufferVertexData para depuração
		/*std::cout << "Conteudo de bufferVertexData:" << std::endl;
		for (size_t i = 0; i < bufferVertexData.size(); i += 7) {
			std::cout << "Vertice " << (i / 7) << ": ";
			std::cout << "X = " << bufferVertexData[i] << ", ";
			std::cout << "Y = " << bufferVertexData[i + 1] << ", ";
			std::cout << "Z = " << bufferVertexData[i + 2] << std::endl;

			std::cout << "R = " << bufferVertexData[i + 3] << ", ";
			std::cout << "G = " << bufferVertexData[i + 4] << ", ";
			std::cout << "B = " << bufferVertexData[i + 5] << ", ";
			std::cout << "A = " << bufferVertexData[i + 6] << std::endl;
		}
		std::cin.get();*/

		glBindBuffer(GL_ARRAY_BUFFER, globalVBO);
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
		// Contando o número de patches
		int numPatches = 0;
		for (int faceIndex = 0; faceIndex < faces.size(); ++faceIndex) {
			const BSP::Face& face = faces.getData()[faceIndex];
			if (face.getType() == FACE_PATCH) {
				numPatches++;
			}
		}

		// Reservando espaço para os dados de patch
		patches.reserve(numPatches);

		// Inicializando dados de patch
		for (int faceIndex = 0; faceIndex < faces.size(); ++faceIndex) {
			const BSP::Face& face = faces.getData()[faceIndex];
			if (face.getType() == FACE_PATCH) {
				// Criar um objeto PatchData usando o construtor
				PatchData patchData(faceIndex, face.getTextureID(), 
					face.getLightmapID(), face.getBezierPatchesSize()[0], 
					face.getBezierPatchesSize()[1]);

				// Chamando uma função separada para inicializar os patches quadráticos
				initializeQuadraticPatches(patchData, face);

				patches.push_back(patchData);
			}
		}
	}

	void Loader::initializeQuadraticPatches(PatchData& patchData, const Face& face) {
		int width = (patchData.getWidth() - 1) / 2;
		int height = (patchData.getHeight() - 1) / 2;
		patchData.resizeQuadraticPatches(width * height);

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				const QuadraticPatch& quadPatch = patchData.getQuadraticPatches()[y * width + x];
				for (int r = 0; r < 3; r++) {
					for (int c = 0; c < 3; c++) {
						// ... preencher os pontos de controle ...
					}
				}
				// Chamando uma função separada para tessellation
				//tessellateQuadraticPatch(quadPatch);
			}
		}
	}

	void Loader::drawFace(int faceIndex) {
		const BSP::Face& face = faces.getData()[faceIndex];

		// Bind VAO
		glBindVertexArray(globalVAO);

		glDrawArrays(GL_TRIANGLES, static_cast<GLuint>(faceToOffsetMap[faceIndex] / 7), 
			face.getNumOfIndices());

		// Desvincule o VAO
		glBindVertexArray(0);
	}
}