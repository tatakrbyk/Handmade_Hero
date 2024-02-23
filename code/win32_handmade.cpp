 
#include <windows.h>

#define internal static
#define local_persist static;
#define global_variable static;

global_variable bool Running;
global_variable BITMAPINFO BitmapInfo;
global_variable void *BitmapMemory;
global_variable HBITMAP BitmapHandle;
global_variable HDC BitmapDeviceContext;

internal void Win32ResizeDIBSection(int Width, int Height)
{
  // TODO: bulletproof this;
  // maybe dont free first, free after then free first it that fails

  if(BitmapHandle)
  {
    DeleteObject(BitmapHandle);
  }

  if(!BitmapDeviceContext)
  {
    // TODO: should we recreate these under certains special circunstances
    BitmapDeviceContext = CreateCompatibleDC(0);
  }

  BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
  BitmapInfo.bmiHeader.biWidth = Width;
  BitmapInfo.bmiHeader.biHeight = Height;
  BitmapInfo.bmiHeader.biPlanes = 1;
  BitmapInfo.bmiHeader.biBitCount = 32;
  BitmapInfo.bmiHeader.biCompression = BI_RGB;

  BitmapHandle = CreateDIBSection(BitmapDeviceContext,
				  &BitmapInfo,
				  DIB_RGB_COLORS,
				  &BitmapMemory,
				  0, 0);
}

internal void Win32UpdateWindow(HDC DeviceContext, int X, int Y, int Width, int Height)
{
  StretchDIBits(DeviceContext,
		 X, Y, Width, Height,
		 X, Y, Width, Height,
		 BitmapMemory,
		 &BitmapInfo,
		 DIB_RGB_COLORS,
		 SRCCOPY);
}

// DIB = Device Independent Bitmap
LRESULT CALLBACK
MainWindowCallback(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam)
{
  LRESULT result = 0;
  switch(Message){
    
    case WM_ACTIVATEAPP:{
      OutputDebugStringA("VM_ACTIVATEAPP\n");
      }break;
    case WM_CLOSE:{
      Running = false;
      }break;
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
      Win32UpdateWindow(DeviceContext, X, Y, Width, Height);
      EndPaint(Window, &Paint);
    }break;
      
    case WM_SIZE:{
      RECT ClientRect;
      GetClientRect(Window, &ClientRect);
      int Width = ClientRect.right - ClientRect.left;
      int Height = ClientRect.bottom - ClientRect.top;
      Win32ResizeDIBSection(Width, Height);
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
      Running = true;
      while (Running){
	
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
