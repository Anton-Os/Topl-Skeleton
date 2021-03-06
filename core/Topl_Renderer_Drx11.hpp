#include "Topl_Renderer.hpp"

#include <d3dx10.h> // DirectX Renderer Specific
#include <d3dx11.h> // DirectX Renderer Specific
#include <d3dx10math.h> // DirectX Renderer Specific

struct Buffer_Drx11 : public Buffer {
	// Buffer_Drx11() : Buffer() {}
	Buffer_Drx11(ID3D11Buffer* b) : Buffer(){ buffer = b; }
	Buffer_Drx11(unsigned id, enum BUFF_Type t, ID3D11Buffer* b) : Buffer(id, t) { buffer = b; }
	Buffer_Drx11(unsigned id, enum BUFF_Type t, ID3D11Buffer* b, unsigned c) : Buffer(id, t, c) { buffer = b; }

	ID3D11Buffer* buffer; // DirectX specific
};

struct Texture_Drx11 : public Texture {
	Texture_Drx11() : Texture() {}
	Texture_Drx11(unsigned id, enum TEX_Frmt f, enum TEX_Mode m, ID3D11Texture2D* t, ID3D11SamplerState* s, ID3D11ShaderResourceView* r)
		: Texture(id, f, m) {
			texture = t;
			sampler = s;
			resView = r;
		}

	ID3D11Texture2D* texture = nullptr;
	ID3D11SamplerState* sampler = nullptr;
	ID3D11ShaderResourceView* resView = nullptr;
};


struct Topl_Pipeline_Drx11 {
	ID3D11InputLayout* vertexDataLayout; // Move out of here, NEXT IMPLEMENTATION
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3DBlob* vsBuff;
	ID3DBlob* psBuff;
};

class Topl_Renderer_Drx11 : public Topl_Renderer {
public:
	Topl_Renderer_Drx11(HWND hwnd) { init(hwnd); }
	~Topl_Renderer_Drx11();

	void buildScene(const Topl_Scene* scene) override;

#ifdef RASTERON_H
    Rasteron_Image* getFrame() override;
    void genTexture(const Rasteron_Image* image, unsigned id) override;
#endif
private:
	void init(NATIVE_WINDOW hwnd) override;
	void pipeline(const Topl_Shader* vertexShader, const Topl_Shader* fragShader) override;
	void pipeline(topl_shader_cptr vertexShader, topl_shader_cptr fragShader, topl_shader_cptr tessCtrlShader, topl_shader_cptr tessEvalShader, topl_shader_cptr geomShader, topl_shader_cptr compShader) override;
	void update(const Topl_Scene* scene) override;
	void render(void) override;

	Topl_Pipeline_Drx11 m_pipeline;
	ID3D11Buffer* mSceneBlockBuff = nullptr; // Drx11 buffer target for scene block data
	std::vector<Buffer_Drx11> mBuffers;
	std::vector<Texture_Drx11> mTextures;

	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceCtx;
	ID3D11RenderTargetView* m_rtv;
	ID3D11ShaderResourceView* mResourceView;
	ID3D11BlendState* mBlendState;
	ID3D11RasterizerState* mRasterizerState;
};