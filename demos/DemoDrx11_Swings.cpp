#include "native_os_def.h"

#include "FileIO.hpp"

#include "Topl_Renderer_Drx11.hpp"

#include "composites/Geo_Component.hpp"
#include "composites/Geo_Construct.hpp"

#include "Chain.hpp"

LRESULT CALLBACK wndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hDC = GetDC(hwnd);
	RECT rect;

	switch (message) {
	case (WM_CREATE): {
	}
	case (WM_PAINT): {
	}
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

// TODO: Move this into .hpp definition

struct VertexShader : public Topl_Shader {
	VertexShader(const char* filePath)
		: Topl_Shader(
			SHDR_Vertex, filePath,
			{ Shader_Type("pos", "POSITION", SHDR_float_vec3) } // Inputs
		) {  }

	virtual bool genPerGeoDataBlock(const Geo_Component* component, std::vector<uint8_t>* bytes) override {
		return true; // Indicates that an implementation exists
	}
};

struct PixelShader : public Topl_Shader {
	PixelShader(const char* filePath)
		: Topl_Shader(
			SHDR_Fragment, filePath,
			{ Shader_Type("pos", "POSITION", SHDR_float_vec3), Shader_Type("flatColor", "COLOR0", SHDR_uint) } // Inputs
		) { }

	virtual bool genPerGeoDataBlock(const Geo_Component* component, std::vector<uint8_t>* bytes) override {
		return false; // Indicates that an implementation is absent
	}
};

// Entry Point

int main(int argc, char** argv) {

	WNDCLASS wndClass = { 0 };
	// wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hInstance = GetModuleHandle(NULL);
	wndClass.lpfnWndProc = wndProc;
	wndClass.lpszClassName = "Default Class";
	RegisterClass(&wndClass);

	HWND wndWindow = CreateWindow(
		"Default Class",
		"Team Purple",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1200, 1100,
		NULL, NULL, GetModuleHandle(NULL), NULL
	);

	ShowWindow(wndWindow, 1);
	UpdateWindow(wndWindow);

    Topl_Renderer_Drx11 renderer(wndWindow);

	std::string vertexShaderSrc = getParentDir(argv[0]) + "\\Vertex_Flat.hlsl";
	VertexShader vertexShader = VertexShader(vertexShaderSrc.c_str());
	std::string pixelShaderSrc = getParentDir(argv[0]) + "\\Pixel_Flat.hlsl";
	PixelShader pixelShader = PixelShader(pixelShaderSrc.c_str());

	renderer.setPipeline(&vertexShader, &pixelShader);

	Topl_SceneManager sMan1;

	Geo_Sphere2D sphere = Geo_Sphere2D(0.2f, 12);
	Geo_Component component = Geo_Component((Geo_RenderObj*)&sphere);
	Geo_Chain_Properties chainProps = Geo_Chain_Properties(0.1f);
	Geo_Chain chain = Geo_Chain("chain1", &sMan1, &component, &chainProps, 8);

	renderer.buildScene(&sMan1);

	MSG wndMessage;
	BOOL bRet;

	while (renderer.renderScene(DRAW_Triangles)) {
		// renderer.updateScene(&sMan1);
		// sMan1.resolvePhysics();
		
		// Input processing, check if it works unhinged
		while (PeekMessage(&wndMessage, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&wndMessage);
			DispatchMessage(&wndMessage);
		}
		
		if (wndMessage.message == WM_QUIT) break;

	}

	return 0;
}