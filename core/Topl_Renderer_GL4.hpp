#include "Topl_Renderer.hpp"

#define GL4_BUFFER_CAPACITY 1024

struct Topl_Data_GL4 {
	GLuint slots[GL4_BUFFER_CAPACITY];
	unsigned slotIndex = 0; // May need to be zero

	std::vector<GLuint> vertexBuffs_3f;
	std::vector<GLuint> indexBuffs_ui;
	std::vector<GLuint> constBuffs_vec3f;
};

#define GL4_VERTEX_ARRAY_CAPACITY 1024

struct Topl_Pipeline_GL4 {
	GLuint vertexDataLayouts[GL4_VERTEX_ARRAY_CAPACITY];
	unsigned layoutIndex = 0;

	GLuint vShader; // Vertex Shader
	GLuint fShader; // Fragment Shader
	GLuint shaderProg;
};

class Topl_Renderer_GL4 : Topl_Renderer {
public:
    Topl_Renderer_GL4(HWND hwnd){ init(hwnd); }
	~Topl_Renderer_GL4();

    // void buildScene(const Topl_SceneGraph* sceneGraph);
    void buildScene(const Topl_SceneGraph* sceneGraph) override;
    void render(void) override;
private:
    void init(NATIVE_WINDOW hwnd) override;
    void createPipeline(void) override;
    // void cleanup(void) override;

    Topl_Pipeline_GL4 m_pipeline;
    Topl_Data_GL4 m_bufferData;

    //NATIVE_GL_CONEXT m_GL4_Ctx;
};