//todo: it prevents shutdown, also it's persistent in taskbar, so user knows there is a server app is running



//
//#include <tchar.h>
//#include <conio.h>
//#include <windows.h>
//
//typedef BOOL (WINAPI *SBRCREATEFUNC)(HWND,LPCWSTR);
//
//void RegisterShutdownBlockReason() {
//    SBRCREATEFUNC ShutdownBlockReasonCreate;
//    HWND hWnd = GetForegroundWindow();
//    HINSTANCE hinstLib = LoadLibrary(TEXT("user32.dll"));
//    if (hinstLib != NULL) {
//        ShutdownBlockReasonCreate = (SBRCREATEFUNC) GetProcAddress(hinstLib,"ShutdownBlockReasonCreate");
//        if(ShutdownBlockReasonCreate != NULL) {
//          if(!(ShutdownBlockReasonCreate) (hWnd, L"Terminating Communication Sessions")) {
//              printf("\nfailed To Register Reason, failure code: %d\n", GetLastError());
//          } else {
//              printf("\nRegistered Reason\n");
//          }
//        } else {
//            printf("\nCouldn't load ShutdownBlockReasonCreate procedure\n");
//        }
//    } else {
//        printf("\nFailed to LoadLibrary(\"user32.dll\")\n");
//    }
//}
//
//int _tmain(int argc, _TCHAR* argv[]) {
//    RegisterShutdownBlockReason();
//    printf("Type to terminate program.\n");
//        char *sendbuf;
//        sendbuf = (char*) malloc(30);
//            scanf("%29s", sendbuf);
//    return 0;
//};


#define _WIN32_WINNT _WIN32_WINNT_WIN7

#include <windows.h>

BOOL WINAPI ShutdownBlockReasonCreate(
    HWND hWnd,
    LPCWSTR pwszReason
);
typedef short (CALLBACK* ShutdownBlockReasonCreateType)(HWND,LPCWSTR);

char *AppTitle="Win1";
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//int WinMain(HINSTANCE hInst,HINSTANCE,LPSTR,int nCmdShow)
{
    
    HMODULE dllHandle=LoadLibrary("User32");
    ShutdownBlockReasonCreateType ShutdownBlockReasonCreatePtr = NULL;
    ShutdownBlockReasonCreatePtr = (ShutdownBlockReasonCreateType)GetProcAddress(dllHandle,"ShutdownBlockReasonCreate");
    
    
  WNDCLASS wc;
  HWND hwnd;
  MSG msg;

  wc.style=CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc=WindowProc;
  wc.cbClsExtra=0;
  wc.cbWndExtra=0;
  wc.hInstance=hInst;
  wc.hIcon=LoadIcon(NULL,IDI_WINLOGO);
  wc.hCursor=LoadCursor(NULL,IDC_ARROW);
  wc.hbrBackground=(HBRUSH)COLOR_WINDOWFRAME;
  wc.lpszMenuName=NULL;
  wc.lpszClassName=AppTitle;

  if (!RegisterClass(&wc))
    return 0;

  hwnd = CreateWindow(AppTitle,AppTitle,
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,CW_USEDEFAULT,100,100,
    NULL,NULL,hInst,NULL);

  if (!hwnd)
    return 0;

  ///////////////////////////////////////////////////////
    //ShutdownBlockReasonCreate(hwnd,NULL);
  BOOL reslt = ShutdownBlockReasonCreatePtr(GetForegroundWindow(),L"Terminating Communication Sessions");
  ///////////////////////////////////////////////////////////
  ShowWindow(hwnd,nCmdShow);
  UpdateWindow(hwnd);

  while (GetMessage(&msg,NULL,0,0) > 0)
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch (msg)
  {
    case WM_PAINT:
      PAINTSTRUCT ps;
      HDC dc;
      RECT r;
      GetClientRect(hwnd,&r);
      dc=BeginPaint(hwnd,&ps);
      DrawText(dc,"Hello World",-1,&r,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
      EndPaint(hwnd,&ps);
      break;

    case WM_DESTROY:
      PostQuitMessage(0);
      break;

  case WM_QUERYENDSESSION:
      return FALSE;
    break;
  case WM_ENDSESSION:
    break;
    default:
      return DefWindowProc(hwnd, msg, wparam, lparam);
  }
  return 0;
} 
