 #include "native_os_def.h"

#include "FileIO.hpp"
// #include "Input.hpp"
#include "Platform.hpp"

#include "Topl_Renderer_Drx11.hpp"

#include "Geo_Construct.hpp"
#include "composites/Humanoid.hpp"

#define MOVE_AMOUNT 8.0

namespace Topl {
	// Management structures
	Topl_SceneManager sceneManager;
	// Input_KeyLogger keyLogger;

	// Render specific objects
	// TODO: Make these not device specific, relative file paths only!
	std::pair<const char*, Eigen::Vector3f> humanoidProps[HUMANOID_PARTS_COUNT] = {
		std::make_pair("C:\\AntonDocs\\Design\\UrkwinArt\\Normguy\\Head.png", Eigen::Vector3f(0.0f, 0.11f, 0.0)),
		std::make_pair("C:\\AntonDocs\\Design\\UrkwinArt\\Normguy\\LeftArm.png", Eigen::Vector3f(0.0f, -0.1f, 0.0)),
		std::make_pair("C:\\AntonDocs\\Design\\UrkwinArt\\Normguy\\RightArm.png", Eigen::Vector3f(0.12f, -0.14f, 0.0)),
		std::make_pair("C:\\AntonDocs\\Design\\UrkwinArt\\Normguy\\Body.png", Eigen::Vector3f(-0.12f, -0.14f, 0.0)),
		std::make_pair("C:\\AntonDocs\\Design\\UrkwinArt\\Normguy\\LeftLeg.png", Eigen::Vector3f(0.06f, -0.35f, 0.0)),
		std::make_pair("C:\\AntonDocs\\Design\\UrkwinArt\\Normguy\\RightLeg.png", Eigen::Vector3f(-0.06f, -0.35f, 0.0))
	};

	Geo_Humanoid humanoid("humanoid", &sceneManager, humanoidProps, 0.25f);
}

struct VertexShader : public Topl_Shader {
	VertexShader(const char* filePath)
		: Topl_Shader(
			SHDR_Vertex, filePath,
			{ Shader_Type("pos", "POSITION", SHDR_float_vec3), Shader_Type("texcoord", "TEXCOORD", SHDR_float_vec2) } // Inputs
		) {  }

	virtual bool genPerGeoDataBlock(const Geo_Component* const component, std::vector<uint8_t>* bytes) const override {
		const uint8_t* offsetBytes_ptr = reinterpret_cast<const uint8_t*>(component->getPos()->data());
		const uint8_t* rotationBytes_ptr = reinterpret_cast<const uint8_t*>(component->getAngles()->data());

		bytes->assign({
			*(offsetBytes_ptr + 0), *(offsetBytes_ptr + 1), *(offsetBytes_ptr + 2), *(offsetBytes_ptr + 3), // X offset value
			*(offsetBytes_ptr + 4), *(offsetBytes_ptr + 5), *(offsetBytes_ptr + 6), *(offsetBytes_ptr + 7), // Y offset value
			*(offsetBytes_ptr + 8), *(offsetBytes_ptr + 9), *(offsetBytes_ptr + 10), *(offsetBytes_ptr + 11), // Z offset value
			0, 0, 0, 0, // 1 byte padding
			*(rotationBytes_ptr + 0), *(rotationBytes_ptr + 1), *(rotationBytes_ptr + 2), *(rotationBytes_ptr + 3), // Main axis rotation
			*(rotationBytes_ptr + 4), *(rotationBytes_ptr + 5), *(rotationBytes_ptr + 6), *(rotationBytes_ptr + 7), // Cross axis rotation
			0, 0, 0, 0, // 1 byte padding
			0, 0, 0, 0 // 1 byte padding
		});

		return true;
	}

	virtual bool genPerSceneDataBlock(const Topl_SceneManager* const sMan, std::vector<uint8_t>* bytes) const {
		const uint8_t* matrixBytes = reinterpret_cast<const uint8_t*>(sMan->getCamera()->getProjMatrix()->data());
	
		/* bytes->assign({ 
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
		}); // dummy value, MUST BE 2 BYTES OR GREATER */

		// Assign with camera value
		bytes->assign({
			*(matrixBytes + 0), *(matrixBytes + 1), *(matrixBytes + 2), *(matrixBytes + 3),
			*(matrixBytes + 4), *(matrixBytes + 5), *(matrixBytes + 6), *(matrixBytes + 7),
			*(matrixBytes + 8), *(matrixBytes + 9), *(matrixBytes + 10), *(matrixBytes + 11),
			*(matrixBytes + 12), *(matrixBytes + 13), *(matrixBytes + 14), *(matrixBytes + 15),

			*(matrixBytes + 16), *(matrixBytes + 17), *(matrixBytes + 18), *(matrixBytes + 19),
			*(matrixBytes + 20), *(matrixBytes + 21), *(matrixBytes + 22), *(matrixBytes + 23),
			*(matrixBytes + 24), *(matrixBytes + 25), *(matrixBytes + 26), *(matrixBytes + 27),
			*(matrixBytes + 28), *(matrixBytes + 29), *(matrixBytes + 30), *(matrixBytes + 31),

			*(matrixBytes + 32), *(matrixBytes + 33), *(matrixBytes + 34), *(matrixBytes + 35),
			*(matrixBytes + 36), *(matrixBytes + 37), *(matrixBytes + 38), *(matrixBytes + 39),
			*(matrixBytes + 40), *(matrixBytes + 41), *(matrixBytes + 42), *(matrixBytes + 43),
			*(matrixBytes + 44), *(matrixBytes + 45), *(matrixBytes + 46), *(matrixBytes + 47),

			*(matrixBytes + 48), *(matrixBytes + 49), *(matrixBytes + 50), *(matrixBytes + 51),
			*(matrixBytes + 52), *(matrixBytes + 53), *(matrixBytes + 54), *(matrixBytes + 55),
			*(matrixBytes + 56), *(matrixBytes + 57), *(matrixBytes + 58), *(matrixBytes + 59),
			*(matrixBytes + 60), *(matrixBytes + 61), *(matrixBytes + 62), *(matrixBytes + 63),
		});
	
		return true;
	}
};

struct PixelShader : public Topl_Shader {
	PixelShader(const char* filePath)
		: Topl_Shader(
			SHDR_Fragment, filePath,
			{ Shader_Type("pos", "POSITION", SHDR_float_vec3), Shader_Type("texcoord", "TEXCOORD", SHDR_float_vec2) } // Inputs
		) { }

	virtual bool genPerGeoDataBlock(const Geo_Component* const component, std::vector<uint8_t>* bytes) const override { return false; }
	virtual bool genPerSceneDataBlock(const Topl_SceneManager* const sMan, std::vector<uint8_t>* bytes) const { return false; }
};
