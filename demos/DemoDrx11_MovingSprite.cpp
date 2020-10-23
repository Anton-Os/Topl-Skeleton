#include "native_os_def.h"

#include "FileIO.hpp"
#include "Geo_Constructs.hpp"

#include "Topl_Renderer_Drx11.hpp"

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

void resolvePhysicsCallback() {
	int i = 1 + 1;
}


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

	MSG wndMessage;
	BOOL bRet;

    Topl_Renderer_Drx11 renderer(wndWindow);

	std::string vertexShaderSrc = getParentDir(argv[0]) + "\\Vertex_MostBasic.hlsl";
	Topl_Shader vertexShader(SHDR_Vertex, vertexShaderSrc.c_str());
	std::string fragmentShaderSrc = getParentDir(argv[0]) + "\\Pixel_MostBasic.hlsl";
	Topl_Shader fragmentShader(SHDR_Fragment, fragmentShaderSrc.c_str());

	renderer.createPipeline(&vertexShader, &fragmentShader);

	Topl_SceneManager sMan1;

	// Geo_Humanoid humanoid1("humanoid1", &sMan1);
	Geo_Humanoid humanoid2("humanoid2", &sMan1);
	humanoid2.move(&sMan1, Eigen::Vector3f(0.9f, 0.3, 0.0)); // Moving humanoid

	renderer.buildScene(&sMan1);

	//timerCallback resolvePhysicsCallback = &Topl_SceneManager::resolvePhysics;
	//Timer_DiscreteEvent physicsLoop(1.0, &sMan1.resolvePhysics);
	while (renderer.renderScene(DRAW_Triangles)) {
		renderer.updateScene(&sMan1);

		sMan1.resolvePhysics();
		// physicsLoop.update();
		// Process input and other things, on successful rendering
	}

	return 0;
}