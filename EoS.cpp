//
// GLSAMPLE.CPP
//  by Blaine Hodge
//

// Includes
#include <windows.h>
#include "Application.h"
#include "Definiton.h"
#include <thread>
#include <chrono>
#include <string>
// Function Declarations

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void EnableOpenGL(HWND hWnd, HDC * hDC, HGLRC * hRC);
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);
BOOL load_texture(char* filename,GLuint num_tex);

// WinMain

void GameThreadHandler();
BOOL SetClientRect(HWND hWnd, int x, int y);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int iCmdShow)
{
	WNDCLASS wc;
	HWND hWnd;
	HDC hDC;
	HGLRC hRC;
	MSG msg;
	BOOL quit = FALSE;

	// register window class
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Explorers of Saarum";
	RegisterClass( &wc );
	
	// create main window
	hWnd = CreateWindow( 
		"Explorers of Saarum", "Explorers of Saarum", 
		WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
		0, 0, 1024, 1024,
		NULL, NULL, hInstance, NULL );
	// enable OpenGL for the window
	SetClientRect(hWnd, 1024, 1024);
	EnableOpenGL( hWnd, &hDC, &hRC );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,1024,1024,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// program main loop
	glewInit();
	//Map.GenerateLevel();
	Application::Instance().Initialization(hWnd);
	std::thread GameThread(GameThreadHandler);
	Application::Instance().m_timer = new Timer(8, 75);
	std::thread AnimationThread(&Timer::cycle, Application::Instance().m_timer);
	Application::Instance().GUI->MapViewer->center.x = Application::Instance().GUI->MapViewer->Player->Cell->x;
	Application::Instance().GUI->MapViewer->center.y = Application::Instance().GUI->MapViewer->Player->Cell->y;
	while ( !quit )
	{
		
		// check for messages
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )  )
		{
			
			// handle or dispatch messages
			if ( msg.message == WM_QUIT ) 
			{
				quit = TRUE;
			} 
			else 
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			
		} 
		else 
		{
			//Player1->Sprite=2+FrameNum;
			//Map.MoveObject(Player1,Player1->x+1,Player1->y+1);
			Application::Instance().GUI->MapViewer->Update();
			{
				Application::Instance().UnderCursor(MouseEventArgs(Application::Instance().mouse->Position, 0));
			}
			Application::Instance().Render();
			SwapBuffers( hDC );

			//MessageBox(NULL,"1","1",MB_OK);
		}
		
	}
	
	// shutdown OpenGL
	GameThread.detach();
	AnimationThread.detach();
	DisableOpenGL( hWnd, hDC, hRC );
	// destroy the window explicitly
	DestroyWindow( hWnd );
	return msg.wParam;
	
}

// Window Procedure

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		return 0;

	case WM_CLOSE:
		PostQuitMessage( 0 );
		return 0;
	
	case WM_DESTROY:
		return 0;

	case WM_MOUSEWHEEL:
	{
		Application::Instance().mouse->MouseWheel(MouseEventArgs(Application::Instance().mouse->Position, wParam));
		return 0;
	}
	
	case WM_LBUTTONDOWN:
	{
		Application::Instance().mouse->MouseDown(MouseEventArgs(Application::Instance().mouse->Position, MK_LBUTTON));
		return 0;
	}
	
	case WM_RBUTTONDOWN:
	{
		Application::Instance().mouse->MouseDown(MouseEventArgs(Application::Instance().mouse->Position, MK_RBUTTON));
		return 0;
	}
	
	case WM_LBUTTONUP:
	{
		Application::Instance().mouse->MouseUp(MouseEventArgs(Application::Instance().mouse->Position, MK_LBUTTON));
		return 0;
	}
	
	case WM_RBUTTONUP:
	{
		Application::Instance().mouse->MouseUp(MouseEventArgs(Application::Instance().mouse->Position, MK_RBUTTON));
		return 0;
	}
	
	case WM_MOUSEMOVE:
	{
		if (Application::Instance().Ready)
		{ 
			Application::Instance().mouse->MouseMove(MouseEventArgs(Application::Instance().mouse->Position, wParam));
		}
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
			Application::Instance().KeyPress(wParam);
			return 0;
		case VK_DOWN:
			Application::Instance().KeyPress(wParam);
			return 0;
		case VK_LEFT:
			Application::Instance().KeyPress(wParam);
			return 0;
		case VK_RIGHT:
			Application::Instance().KeyPress(wParam);
			return 0;
		}
		return 0;

	case WM_CHAR:
		Application::Instance().KeyPress(wParam);
		return 0;

	case WM_SETCURSOR:
		SetCursor(NULL);
		return 0;
	
	default:
		return DefWindowProc( hWnd, message, wParam, lParam );	
	}
	
}

// Enable OpenGL

void EnableOpenGL(HWND hWnd, HDC * hDC, HGLRC * hRC)
{
	PIXELFORMATDESCRIPTOR pfd;
	int format;
	
	// get the device context (DC)
	*hDC = GetDC( hWnd );
	
	// set the pixel format for the DC
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	format = ChoosePixelFormat( *hDC, &pfd );
	SetPixelFormat( *hDC, format, &pfd );
	
	// create and enable the render context (RC)
	*hRC = wglCreateContext( *hDC );
	wglMakeCurrent( *hDC, *hRC );
	
}

// Disable OpenGL

void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( hRC );
	ReleaseDC( hWnd, hDC );
}

void GameThreadHandler()
{
	int time = 1;
	while (true)
	{
		if (time == 1)
		{
			time = 15;
		}
		else
		{
			time -= 1;
		}
		Application::Instance().GUI->Timer->Update(time);
		if (time == 15)
		{
			Application::Instance().Update();
			Application::Instance().GUI->MapViewer->Update();
			Application::Instance().GUI->DescriptionBox->AddItemControl(new GUI_Text("’Ó‰ - " + std::to_string(Application::Instance().GameTurn) + ".", new GUI_TextFormat(10, 19, TColor(0.0, 0.8, 0.0, 1.0))));
			Application::Instance().GameTurn += 1;
		}
		std::chrono::milliseconds Duration(250);
		std::this_thread::sleep_for(Duration);
	}
}

BOOL SetClientRect(HWND hWnd, int x, int y)
{
	RECT rect = { 0, 0, x, y }, rect2;
	AdjustWindowRectEx(&rect, GetWindowLong(hWnd, GWL_STYLE), (BOOL)GetMenu(hWnd), GetWindowLong(hWnd, GWL_EXSTYLE));
	GetWindowRect(hWnd, &rect2);
	return MoveWindow(hWnd, rect2.left, rect2.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
}