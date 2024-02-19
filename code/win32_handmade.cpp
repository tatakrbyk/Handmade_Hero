 
#include <windows.h>

LRESULT CALLBACK
MainWindowCallback(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam)
{
  LRESULT result = 0;
  switch(Message){
    
    case WM_ACTIVATEAPP:{
      OutputDebugStringA("VM_ACTIVATEAPP\n");
      }break;
      //case VM_CLOSE:{
      //OutputDebugStringA("VM_CLOSE\n");
      // }break;
    case WM_DESTROY:{
      OutputDebugStringA("VM_DESTROY\n");
      }break;
    
    case WM_PAINT:{
      PAINTSTRUCT Paint;
      HDC DeviceContext = BeginPaint(Window, &Paint);
      int X = Paint.rcPaint.left;
      int Y = Paint.rcPaint.top;
      int Width= Paint.rcPaint.right - Paint.rcPaint.left;
      int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
      static DWORD Operation = WHITENESS;
      PatBlt(DeviceContext, X, Y, Width, Height, Operation);
      if (Operation == WHITENESS)
      {
	Operation = BLACKNESS;
      }
      else
      {
	Operation = WHITENESS;
      }
      EndPaint(Window, &Paint);
    }break;
    case WM_SIZE:{
      OutputDebugStringA("VM_SIZE\n");
      }break;
    default:{
      result = DefWindowProcA(Window, Message, wParam, lParam);
    } break;
  }
  return result;
}		    
	
int CALLBACK
WinMain(HINSTANCE Instance,
	HINSTANCE PrevInstance,
	LPSTR CommandLine,
	int ShowCode)
{
  WNDCLASS WindowClass = {}; // zero initialize
  
  WindowClass.style = CS_HREDRAW | CS_VREDRAW; // always redraw when resize
  WindowClass.lpfnWndProc = MainWindowCallback;
  WindowClass.hInstance = Instance;
  WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
  //WindowClass.hIcon; // Todo: add Icon
  //WindowClass.lpszMenuName = ; // create like File, Ed,t Options, Help 
  WindowClass.lpszClassName = "HandmadeHeroWindowClass";

  if(RegisterClass(&WindowClass))
  {
    HWND WindowHandle = CreateWindowEx(0, //DWORD dwExStyle,
			WindowClass.lpszClassName, //LPCWSTR lpClassName,
			"Handmade Hero", //LPCWSTR lpWindowName,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,//DWORD dwStyle,
			CW_USEDEFAULT, //int X,
			CW_USEDEFAULT, //int Y,
			CW_USEDEFAULT, //int nWidth,
			CW_USEDEFAULT, //int nHeight,
			0,//HWND hWndParent,
			0, //HMENU hMenu,
			Instance, //HINSTANCE hInstance,
			0);//LPVOID lpParam 

    if(WindowHandle)
    {
      for(;;){
	
	MSG Message;
	BOOL MessageResult = GetMessage(&Message, 0, 0, 0);
	if(MessageResult > 0)
	{
	  TranslateMessage(&Message);
	  DispatchMessage(&Message);
	}
	else
	{
	  break;
	}
      }
    }
    
    else
    {
      // TODO(Taha): // Logging
    }
				    
  }   
  else
  {
    // Todo(Taha): Logging
  }
  return 0;
}
