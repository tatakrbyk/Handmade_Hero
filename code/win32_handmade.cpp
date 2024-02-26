 
#include <windows.h>
#include <stdint.h>

#define internal static
#define local_persist static;
#define global_variable static;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

global_variable bool Running;

global_variable BITMAPINFO BitmapInfo;
global_variable void *BitmapMemory;
global_variable int BitmapWidth;
global_variable int BitmapHeight;
global_variable int BytesPerPixel = 4;

internal void
RenderWirdGradiend(int Xoffset, int Yoffset)
{
  int Pitch = BitmapWidth * BytesPerPixel;
  uint8 *Row = (uint8 *)BitmapMemory;

  for(int y = 0; y < BitmapHeight; ++y)
  {
    uint32 *Pixel = (uint32*)Row;

    for(int x = 0; x < BitmapHeight; ++x)
    {
      uint8 g = (y + Yoffset);
      uint8 b = (x + Xoffset);

      /*
	Memory:   BB GG RR XX
	Register: XX RR GG BB
	Pixel (32-bits)
      */

      *Pixel++ = (g << 8) | b;
    }
    Row += Pitch;
  }
}

internal void
Win32ResizeDIBSection(int Width, int Height)
{
  // TODO: bulletproof this;
  // maybe dont free first, free after then free first it that fails

  if(BitmapMemory)
  {
    VirtualFree(BitmapMemory, 0, MEM_RELEASE);
  }

  BitmapWidth = Width;
  BitmapHeight = Height;

  BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
  BitmapInfo.bmiHeader.biWidth = BitmapWidth;
  BitmapInfo.bmiHeader.biHeight = -BitmapHeight; // negative is top-down, positive is bottom-up
  BitmapInfo.bmiHeader.biPlanes = 1;
  BitmapInfo.bmiHeader.biBitCount = 32;
  BitmapInfo.bmiHeader.biCompression = BI_RGB;

  int BitmapMemorySize = (BitmapWidth * BitmapHeight) * BytesPerPixel;
  BitmapMemory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

internal void
Win32UpdateWindow(HDC DeviceContext, RECT *ClientRect,  int X, int Y, int Width, int Height)
{
  int WindowWidth = ClientRect->right - ClientRect->left;
  int WindowHeight = ClientRect->bottom - ClientRect->top;
  StretchDIBits(DeviceContext,
		 X, Y, BitmapWidth, BitmapHeight,
		 X, Y, BitmapWidth, BitmapHeight,
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

      RECT ClientRect;
      GetClientRect(Window, &ClientRect);
      
      Win32UpdateWindow(DeviceContext, &ClientRect, X, Y, Width, Height);
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
    HWND Window = CreateWindowExA(0, //DWORD dwExStyle,
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

    if(Window)
    {
      Running = true;
      int Xoffset = 0;
      int Yoffset = 0;
      
      while (Running){
	
	MSG Message;
	while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
	{
	  if(Message.message == WM_QUIT)
	  {
	    Running = false;
	  }
	  TranslateMessage(&Message);
	  DispatchMessage(&Message);
	}
        
        RenderWirdGradiend(Xoffset, Yoffset);
	HDC DeviceContext = GetDC(Window);
	RECT ClientRect;
	GetClientRect(Window, &ClientRect);
	int WindowWidth = ClientRect.right - ClientRect.left;
	int WindowHeight = ClientRect.bottom - ClientRect.top;
	Win32UpdateWindow(DeviceContext, &ClientRect, 0, 0, WindowWidth, WindowHeight);
	ReleaseDC(Window, DeviceContext);

	++Xoffset;
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
