#include<windows.h>
#include<shellapi.h>
#include<stdio.h>

#include "unistd.h"

#include "secure-rfid-login.h"
#include "ArgoRFID_Reader.h"

#define ID_ABOUT       2000
#define ID_EXIT        2001
#define ID_NOTIFY	   2002

void startTray(HWND window);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL ShowPopupMenu( HWND hWnd, POINT *curpos, int wDefaultItem );
void closeTray(HWND window);
void sendNotification();

const char g_szClassName[] = "myWindowClass";
static BOOL g_bModalState = FALSE;
static NOTIFYICONDATA nData;

DWORD WINAPI server_thread( LPVOID lpParam );
DWORD WINAPI serial_thread( LPVOID lpParam );

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
		 case WM_APP:
          switch (lParam) {
   
            case WM_LBUTTONDBLCLK:
              MessageBox( hwnd, TEXT("Hi!"), TEXT("Title"), MB_ICONINFORMATION | MB_OK );
              return 0;
        
            case WM_RBUTTONUP:
              SetForegroundWindow( hwnd );
              ShowPopupMenu(hwnd, NULL, -1 );
              PostMessage( hwnd, WM_APP + 1, 0, 0 );
			  return 0;
			  
			case NIN_BALLOONUSERCLICK:
				 MessageBox( hwnd, TEXT("OK! "), TEXT("Title over baloon event"), MB_ICONINFORMATION | MB_OK );
              return 0;
          }
		  return 0;
		  case WM_COMMAND:
          switch (LOWORD(wParam)) {
            if ( g_bModalState ) { return 1; }
   
            case ID_ABOUT:
              g_bModalState = TRUE;
              MessageBox( hwnd, TEXT("Hi!"), TEXT("Title"), MB_ICONINFORMATION | MB_OK );
              g_bModalState = FALSE;
              return 0;
   
            case ID_EXIT:
              PostMessage( hwnd, WM_CLOSE, 0, 0 );
              return 0;
			case ID_NOTIFY:
				sendNotification();
				return 0;
          }
          return 0;
        case WM_CLOSE:
			closeTray(hwnd);
			DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return DefWindowProc( hwnd, msg, wParam, lParam );;
}

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
	HANDLE sever_Thread,serial_Thread;
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

	ZeroMemory(&wc,sizeof(wc));

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = &WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "The title of my window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

   //ShowWindow(hwnd, nCmdShow);
   UpdateWindow(hwnd);
	startTray(hwnd);
	
	sever_Thread = CreateThread(NULL,0,server_thread,NULL,0,NULL);
	serial_Thread = CreateThread(NULL,0,serial_thread,NULL,0,NULL);


    // Step 3: The Message Loop
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }


	UnregisterClass(g_szClassName,hInstance);
    return Msg.wParam;
}

void startTray(HWND window)
{
	ZeroMemory(&nData,sizeof(nData));
	
	nData.cbSize = sizeof(nData);
	nData.hWnd   = window;
    nData.uID    = 100;
    nData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nData.uCallbackMessage = WM_APP;
	//nData.szTip = "My Program\n";
    //u could load ur own icon here
    nData.hIcon  = LoadIcon(NULL, IDI_APPLICATION);
	
	Shell_NotifyIcon(NIM_ADD,&nData);
}
void closeTray(HWND window)
{
	Shell_NotifyIcon(NIM_DELETE,&nData);
}

void sendNotification()
{
	nData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO | NIF_STATE;
	strcpy(nData.szInfo, "Description here!");
	strcpy(nData.szInfoTitle, "I'm the Title!");
	nData.dwInfoFlags = NIIF_INFO;

	Shell_NotifyIcon(NIM_MODIFY,&nData);

}

BOOL ShowPopupMenu( HWND hWnd, POINT *curpos, int wDefaultItem ) {
   
   
      //ADD MENU ITEMS.------------------------------------------------------------------
      HMENU hPop = CreatePopupMenu();
      if ( g_bModalState ) { return FALSE; }
      InsertMenu( hPop, 0, MF_BYPOSITION | MF_STRING, ID_ABOUT, "About..." );
      InsertMenu( hPop, 2, MF_BYPOSITION | MF_STRING, ID_EXIT , "Exit" );
	  InsertMenu( hPop, 1, MF_BYPOSITION | MF_STRING, ID_NOTIFY , "Notify" );
      
      //CAN DO WITHOUT STUFF.------------------------------------------------------------
      SetMenuDefaultItem( hPop, ID_ABOUT, FALSE );
      SetFocus          ( hWnd );
      SendMessage       ( hWnd, WM_INITMENUPOPUP, (WPARAM)hPop, 0 );
   
      //SHOW POPUP MENU.-----------------------------------------------------------------
      {
        //GET CURSOR POSITION TO CREATE POPUP THERE.-------------------------------------
        POINT pt;
        if (!curpos) {
          GetCursorPos( &pt );
          curpos = &pt;
        }
   
        {
          //DISPLAT MENU AND WAIT FOR SELECTION.-----------------------------------------
          WORD cmd = TrackPopupMenu( hPop, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, curpos->x, curpos->y, 0, hWnd, NULL );
   
          //SEND MESSAGE MAPPED TO SELECTED ITEM.----------------------------------------
          SendMessage( hWnd, WM_COMMAND, cmd, 0 );
        }
      }
   
      DestroyMenu(hPop);
   
      return 0;
    }

DWORD WINAPI server_thread( LPVOID lpParam )
{
	startServerSocket(&nData);
}

DWORD WINAPI serial_thread( LPVOID lpParam )
{
	start_rfid_reader(&nData);
}
