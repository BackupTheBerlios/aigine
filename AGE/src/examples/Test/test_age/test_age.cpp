#include "../../../AGE.h"	// Windows

HWND		hWnd;
WNDCLASSEX 	wndclass;



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) 
{	
	MSG msg;	
	static char	szAppName[] = "AppName";				
	wndclass.cbSize = sizeof(wndclass);	
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
	wndclass.lpfnWndProc = WndProc;	
	wndclass.cbClsExtra = 0;	
	wndclass.cbWndExtra = 0;	
	wndclass.hInstance = hInst;	
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);	
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	
	wndclass.lpszMenuName = NULL;	
	wndclass.lpszClassName = szAppName;	
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);	
	RegisterClassEx(&wndclass);	

	if (!(hWnd=CreateWindowEx(NULL, szAppName,                  
				"DirectX9 Demo Application",                  
				WS_OVERLAPPEDWINDOW | WS_VISIBLE,                  
				GetSystemMetrics(SM_CXSCREEN)/2-250,                  
				GetSystemMetrics(SM_CYSCREEN)/2-187,                  
				500, 300, NULL, NULL, hInst, NULL)))		
		return 0;	
}