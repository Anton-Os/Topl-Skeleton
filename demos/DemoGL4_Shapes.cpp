#include "native_os_def.h"

#include "FileIO.hpp"
#include "Geometry/Geometry_Constructs.hpp"

#include "Topl_Renderer_GL4.hpp"

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


    Topl_Renderer_GL4 renderer(wndWindow);

	std::string vertexShaderSrc = getParentDir(argv[0]) + "\\VertexShader.glsl"; // Make unix fix
	Topl_Shader vertexShader(SHDR_Vertex, vertexShaderSrc.c_str());
	std::string fragmentShaderSrc = getParentDir(argv[0]) + "\\FragShader.glsl"; // Make unix fix
	Topl_Shader fragmentShader(SHDR_Fragment, fragmentShaderSrc.c_str());

	renderer.createPipeline(&vertexShader, &fragmentShader);

	Topl_SceneManager sGraph1; // REMOVE LATER
	// Geo_RandShapes randShapes(&sGraph1);
	Geo_CircleUp circleUp("prefix", &sGraph1);

	renderer.buildScene(&sGraph1);

	while ( renderer.renderScene(DRAW_Triangles)) {
		circleUp.updateSceneManager(&sGraph1);
		renderer.updateScene(&sGraph1);
		// Process input and other things, on successful rendering
	}

	return 0;
}