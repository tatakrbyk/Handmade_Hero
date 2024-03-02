 
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

struct win32_offscreen_buffer
{
  BITMAPINFO Info;
  void *Memory;
  int Width;
  int Height;
  int Pitch;
  int BytesPerPixel;
};

struct win32_window_dimension
{
  int Width;
  int Height;
};

global_variable bool Running;

global_variable win32_offscreen_buffer GlobalBackBuffer;
/*
global_variable BITMAPINFO BitmapInfo;
global_variable void *BitmapMemory;
global_variable int BitmapWidth;
global_variable int BitmapHeight;
global_variable int BytesPerPixel = 4;
*/

win32_window_dimension Win32GetWindowDimension(HWND Window)
{
  win32_window_dimension Result;
  RECT ClientRect;
  GetClientRect(Window, &ClientRect);
  Result.Width = ClientRect.right - ClientRect.left;
  Result.Height = ClientRect.bottom - ClientRect.top;

  return Result;
}
internal void
RenderWeirdGradiend(const win32_offscreen_buffer &Buffer, int Xoffset, int Yoffset)
{
  uint8 *Row = (uint8 *)Buffer.Memory;

  for(int y = 0; y < Buffer.Height; ++y)
  {
    uint32 *Pixel = (uint32*)Row;

    for(int x = 0; x < Buffer.Width; ++x)
    {
      uint8 Green = (y + Yoffset);
      uint8 Blue = (x + Xoffset);

      /*
	Memory:   BB GG RR XX
	Register: XX RR GG BB
	Pixel (32-bits)
      */

      *Pixel++ = (Green << 8) | Blue;
    }
    Row += Buffer.Pitch;
  }
}

internal void
Win32ResizeDIBSection(win32_offscreen_buffer &Buffer ,int Width, int Height)
{
  // TODO: bulletproof this;
  // maybe dont free first, free after then free first it that fails

  if(Buffer.Memory)
  {
    VirtualFree(Buffer.Memory, 0, MEM_RELEASE);
  }

  Buffer.Width = Width;
  Buffer.Height = Height;
  Buffer.BytesPerPixel = 4;

  Buffer.Info.bmiHeader.biSize = sizeof(Buffer.Info.bmiHeader);
  Buffer.Info.bmiHeader.biWidth = Buffer.Width;
  Buffer.Info.bmiHeader.biHeight = -Buffer.Height; // negative is top-down, positive is bottom-up
  Buffer.Info.bmiHeader.biPlanes = 1;
  Buffer.Info.bmiHeader.biBitCount = 32;
  Buffer.Info.bmiHeader.biCompression = BI_RGB;

  int BitmapMemorySize = (Buffer.Width * Buffer.Height) * Buffer.BytesPerPixel;
  Buffer.Memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

  Buffer.Pitch = Width * Buffer.BytesPerPixel;
}

internal void
Win32DisplayBufferInWindow(HDC DeviceContext, int WindowWidth, int WindowHeight,
			   const win32_offscreen_buffer &Buffer, int X, int Y, int Width, int Height)
{
  StretchDIBits(DeviceContext,
		 X, Y, WindowWidth, WindowHeight,
		 X, Y, Buffer.Width, Buffer.Height,
		 Buffer.Memory,
		 &Buffer.Info,
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

      win32_window_dimension Dimension = Win32GetWindowDimension(Window);
      
      Win32DisplayBufferInWindow(DeviceContext, Dimension.Width, Dimension.Height, GlobalBackBuffer,
			       X, Y, Width, Height);
      EndPaint(Window, &Paint);
    }break;
      
  case WM_SIZE:{ // ?
    /*
      RECT ClientRect;
      GetClientRect(Window, &ClientRect);
      int Width = ClientRect.right - ClientRect.left;
      int Height = ClientRect.bottom - ClientRect.top;
      Win32ResizeDIBSection(Width, Height);*/
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

  Win32ResizeDIBSection(GlobalBackBuffer, 1280, 720);
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
        
        RenderWeirdGradiend(GlobalBackBuffer, Xoffset, Yoffset);
	HDC DeviceContext = GetDC(Window);
	win32_window_dimension Dimension = Win32GetWindowDimension(Window);
	Win32DisplayBufferInWindow(DeviceContext, Dimension.Width, Dimension.Height, GlobalBackBuffer, 0, 0,
				   Dimension.Width, Dimension.Height);
	ReleaseDC(Window, DeviceContext);

	++Xoffset;
	Yoffset +=2;
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
