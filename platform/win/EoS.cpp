// Includes
#include <windows.h>
#include <string>
#include "platform/win/Renderer.h"
#include "game/Application.h"
#include "game/MouseController.h"
#include "game/GraphicalController.h"
#include "game/utils/winlog.h"

// Function Declarations

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void EnableGraphics(HWND hWnd, HDC &hDC);
void DisableGraphics(HWND hWnd, HDC hDC);
BOOL SetClientRect(HWND hWnd, int x, int y);

// WinMain

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int iCmdShow)
{
	WNDCLASS wc;
	HWND hWnd;
	HDC hDC;
	MSG msg;
	BOOL quit = FALSE;

	// initialize logging
	FLAGS_logtostderr = true;
	FLAGS_colorlogtostderr = true;
	google::InitGoogleLogging("EoS");
	std::shared_ptr<WinDebugSink> sink(new WinDebugSink());
	google::AddLogSink(sink.get());

	// register window class
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Explorers of Saarum";
	RegisterClass(&wc);
	
	const dimension_t window_size(1024, 1024);

	rectangle_t window_rect((GetSystemMetrics(SM_CXFULLSCREEN)-window_size.w)/2, (GetSystemMetrics(SM_CYFULLSCREEN) - window_size.w) / 2,window_size);

	// create main window
	
	hWnd = CreateWindow(
		("Explorers of Saarum"), "Explorers of Saarum", 
		WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
		window_rect.x, window_rect.y, window_rect.right(), window_rect.bottom(),
		NULL, NULL, hInstance, NULL);

	// enable OpenGL for the window
	SetClientRect(hWnd, window_size.w, window_size.h);
	EnableGraphics(hWnd, hDC);
	//Map.GenerateLevel();
	Renderer renderer(hDC, window_size);
	renderer.start();

	// message handling loop in main thread
	BOOL bRet;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// handle the error and possibly exit
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
   
 	// shutdown OpenGL
	renderer.stop();
	DisableGraphics(hWnd, hDC);
	// destroy the window explicitly
	DestroyWindow(hWnd);
	return msg.wParam;
}

// Window Procedure

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	struct Extractor
	{
		static position_t position(HWND hWnd, LPARAM lParam)
		{
			POINT mouse_pos;
			mouse_pos.x = LOWORD(lParam);
			mouse_pos.y = HIWORD(lParam);
			return position_t(mouse_pos.x, mouse_pos.y);
		}

		static int wheel_delta(WPARAM wParam)
		{
			return GET_WHEEL_DELTA_WPARAM(wParam);
		}

		static int key_pressed(WPARAM wParam)
		{
			if ((wParam & MK_LBUTTON) == MK_LBUTTON)
				return mk_left;
			else if ((wParam & MK_RBUTTON) == MK_RBUTTON)
				return mk_right;
			else if ((wParam & MK_MBUTTON) == MK_MBUTTON)
				return mk_middle;
			else
				return mk_none;
		}
	};

	switch (message)
	{
	case WM_CREATE:
		return 0;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	
	case WM_DESTROY:
		return 0;

	case WM_MOUSEWHEEL:
	{
		if (Application::instance().m_ready)
			Application::instance().m_mouse->mouse_wheel(MouseEventArgs(Extractor::position(hWnd, lParam), mk_none, Extractor::wheel_delta(wParam)));
		return 0;
	}
	
	case WM_LBUTTONDOWN:
	{
		if (Application::instance().m_ready)
			Application::instance().m_mouse->mouse_down(MouseEventArgs(Extractor::position(hWnd, lParam), mk_left));
		return 0;
	}
	
	case WM_RBUTTONDOWN:
	{
		if (Application::instance().m_ready)
			Application::instance().m_mouse->mouse_down(MouseEventArgs(Extractor::position(hWnd, lParam), mk_right));
		return 0;
	}
	
	case WM_LBUTTONUP:
	{
		if (Application::instance().m_ready)
			Application::instance().m_mouse->mouse_up(MouseEventArgs(Extractor::position(hWnd, lParam), mk_left));
		return 0;
	}
	
	case WM_RBUTTONUP:
	{
		if (Application::instance().m_ready)
			Application::instance().m_mouse->mouse_up(MouseEventArgs(Extractor::position(hWnd, lParam), mk_right));
		return 0;
	}
	
	case WM_MOUSEMOVE:
	{
		if (Application::instance().m_ready)
			Application::instance().m_mouse->mouse_move(MouseEventArgs(Extractor::position(hWnd, lParam), Extractor::key_pressed(wParam)));
		return 0;
	}

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		case VK_SPACE:
			return 0;
		case VK_UP:
			Application::instance().key_press(wParam);
			return 0;
		case VK_DOWN:
			Application::instance().key_press(wParam);
			return 0;
		case VK_LEFT:
			Application::instance().key_press(wParam);
			return 0;
		case VK_RIGHT:
			Application::instance().key_press(wParam);
			return 0;
		}
		return 0;

	case WM_CHAR:
		Application::instance().key_press(wParam);
		return 0;

	case WM_SETCURSOR:
		SetCursor(NULL);
		return 0;
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);	
	}
	
}

void EnableGraphics(HWND hWnd, HDC &hDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	int format;
	
	// get the device context (DC)
	hDC = GetDC(hWnd);
	
	// set the pixel format for the DC
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	format = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, format, &pfd);
}

void DisableGraphics(HWND hWnd, HDC hDC)
{
	ReleaseDC(hWnd, hDC);
}

BOOL SetClientRect(HWND hWnd, int x, int y)
{
	RECT rect = { 0, 0, x, y }, rect2;
	AdjustWindowRectEx(&rect, GetWindowLong(hWnd, GWL_STYLE), (BOOL)GetMenu(hWnd), GetWindowLong(hWnd, GWL_EXSTYLE));
	GetWindowRect(hWnd, &rect2);
	return MoveWindow(hWnd, rect2.left, rect2.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
}