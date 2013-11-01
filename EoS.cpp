//
// GLSAMPLE.CPP
//  by Blaine Hodge
//

// Includes
#include <windows.h>
#include "TMap.h"
#include "TMapObject.h"
#include "TGraph.h"
#include "TMapViewer.h"
#include "TGUI.h"
#include "TGUIControlLayer.h"
#include "TGCButton.h"
#include "TClient.h"


// Function Declarations

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void EnableOpenGL(HWND hWnd, HDC * hDC, HGLRC * hRC);
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);
BOOL load_texture(char* filename,GLuint num_tex);

TMap Map;
TPlayer* Player1;
TMapViewer* Viewer;
TGUIControlLayer* Menu;
TGCButton* Button;
TClient* Client;


// WinMain

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
	EnableOpenGL( hWnd, &hDC, &hRC );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,1024,1024,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// program main loop
	glewInit();
	//Map.GenerateLevel();
	Map.GenerateRoom();
	Player1=new TPlayer;
	Map.AddObject(Player1,Map.Items[9][9]);
	Map.AddNewObject(new TTorch(),Map.Items[7][7]);
	Map.AddNewObject(new TTorch(),Map.Items[7][33]);
	Map.AddNewObject(new TTorch(),Map.Items[33][33]);
    Map.AddNewObject(new TTorch(),Map.Items[33][7]);
	Map.AddObject(new TOrc(),Map.Items[14][14]);
	Client=new TClient();
	Viewer=new TMapViewer(Client->GUI);
	Button=new TGCButton();
	Button->Text="EXIT";
	Button->x=924;
	Button->y=974;
    Button->width=100;
	Button->height=50;
	Menu=new TGUIControlLayer(Client->GUI);
	Menu->Items.push_back(Button);
	Viewer->LoadMaptextures(Client->Graph);
	Viewer->Map=&Map;
	Viewer->Player=Player1;
	Client->GUI->Layers.push_back(Menu);
	Client->GUI->Layers.push_back(Viewer);


	int FrameNum=0;

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
			Player1->Sprite=3+FrameNum/10;
			Viewer->Update();
			Client->GUI->Render(Client->Graph);
			SwapBuffers( hDC );
			FrameNum+=1;
			if(FrameNum>39){FrameNum=0;}
			//MessageBox(NULL,"1","1",MB_OK);
		}
		
	}
	
	// shutdown OpenGL
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

	case WM_LBUTTONDOWN:
		TPoint Mouse;
		Mouse=Client->Graph->GetMouseCoordinat(hWnd);
		Client->GUI->Select(Mouse.x,Mouse.y);
		return 0;
		
	case WM_KEYDOWN:
		switch ( wParam )
		{
			
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		case VK_SPACE:
			return 0;
		case VK_UP:
			Map.MoveObject(Player1,Player1->Cell->x,Player1->Cell->y+1);
			return 0;
		case VK_DOWN:
			Map.MoveObject(Player1,Player1->Cell->x,Player1->Cell->y-1);
			return 0;
		case VK_LEFT:
			Map.MoveObject(Player1,Player1->Cell->x-1,Player1->Cell->y);
			return 0;
		case VK_RIGHT:
			Map.MoveObject(Player1,Player1->Cell->x+1,Player1->Cell->y);
			return 0;
			
		}
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
