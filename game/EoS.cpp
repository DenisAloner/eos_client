//
// GLSAMPLE.CPP
//  by Blaine Hodge
//

// Includes
#include "Application.h"
#include "Definiton.h"
#include "utils/winlog.h"
#include <windows.h>
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
	Application::instance().initialization(hWnd);
	std::thread GameThread(GameThreadHandler);
	Application::instance().m_timer = new Timer(8, 75);
	std::thread AnimationThread(&Timer::cycle, Application::instance().m_timer);
	Application::instance().m_GUI->MapViewer->m_center.x = Application::instance().m_GUI->MapViewer->m_player->m_cell->x;
	Application::instance().m_GUI->MapViewer->m_center.y = Application::instance().m_GUI->MapViewer->m_player->m_cell->y;
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
			Application::instance().m_GUI->MapViewer->update();
			Application::instance().render();
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
		Application::instance().m_mouse->mouse_wheel(MouseEventArgs(Application::instance().m_mouse->m_position, wParam));
		return 0;
	}
	
	case WM_LBUTTONDOWN:
	{
		Application::instance().m_mouse->mouse_down(MouseEventArgs(Application::instance().m_mouse->m_position, MK_LBUTTON));
		return 0;
	}
	
	case WM_RBUTTONDOWN:
	{
		Application::instance().m_mouse->mouse_down(MouseEventArgs(Application::instance().m_mouse->m_position, MK_RBUTTON));
		return 0;
	}
	
	case WM_LBUTTONUP:
	{
		Application::instance().m_mouse->mouse_up(MouseEventArgs(Application::instance().m_mouse->m_position, MK_LBUTTON));
		return 0;
	}
	
	case WM_RBUTTONUP:
	{
		Application::instance().m_mouse->mouse_up(MouseEventArgs(Application::instance().m_mouse->m_position, MK_RBUTTON));
		return 0;
	}
	
	case WM_MOUSEMOVE:
	{
		if (Application::instance().m_ready)
		{ 
			Application::instance().m_mouse->mouse_move(MouseEventArgs(Application::instance().m_mouse->m_position, wParam));
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
		Application::instance().m_GUI->Timer->Update(time);
		if (time == 15)
		{
			Application::instance().update();
			Application::instance().m_GUI->MapViewer->update();
			Application::instance().m_GUI->DescriptionBox->add_item_control(new GUI_Text("’Ó‰ - " + std::to_string(Application::instance().m_game_turn) + ".", new GUI_TextFormat(10, 19, RGBA_t(0.0, 0.8, 0.0, 1.0))));
			Application::instance().m_game_turn += 1;
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