 
#include <windows.h>
#include <stdint.h>
#include <Xinput.h>

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
};

struct win32_window_dimension
{
  int Width;
  int Height;
};

global_variable bool GlobalRunning;
global_variable win32_offscreen_buffer GlobalBackBuffer;

//NOTE: XInputGetState
#define X_INPUT_GET_STATE(name) DWORD WINAPI name( DWORD dwUserIndex, XINPUT_STATE* pState)
typedef X_INPUT_GET_STATE(x_input_get_state);
X_INPUT_GET_STATE(XInputGetStateStub)
{
	return 0;
}
global_variable x_input_get_state *XInputGetState_ = XInputGetStateStub;

//NOTE: XINputSetState
#define X_INPUT_SET_STATE(name) DWORD WINAPI name( DWORD dwUserIndex, XINPUT_VIBRATION* pVibration )
typedef X_INPUT_SET_STATE(x_input_set_state);
X_INPUT_SET_STATE(XInputSetStateStub)
{
	return 0;
}
global_variable x_input_set_state *XInputSetState_ = XInputSetStateStub;


// this works because XInput was included before

#define XInputGetState XInputGetState_
#define XInputSetState XInputSetState_

internal void Win32LoadXInput()
{
  HMODULE XInputLibrary = LoadLibraryA("xinput1_3.dll");
  if(XInputLibrary)
    {
      XInputGetState = (x_input_get_state*)GetProcAddress(XInputLibrary,"XInputGetState");
      XInputSetState = (x_input_set_state*)GetProcAddress(XInputLibrary, "XInputSetState");
    }
}
internal win32_window_dimension
Win32GetWindowDimension(HWND Window)
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
  int BytesPerPixel = 4;

  Buffer.Info.bmiHeader.biSize = sizeof(Buffer.Info.bmiHeader);
  Buffer.Info.bmiHeader.biWidth = Buffer.Width;
  Buffer.Info.bmiHeader.biHeight = -Buffer.Height; // negative is top-down, positive is bottom-up
  Buffer.Info.bmiHeader.biPlanes = 1;
  Buffer.Info.bmiHeader.biBitCount = 32;
  Buffer.Info.bmiHeader.biCompression = BI_RGB;

  int BitmapMemorySize = (Buffer.Width * Buffer.Height) * BytesPerPixel;
  Buffer.Memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

  Buffer.Pitch = Width * BytesPerPixel;
}

internal void
Win32DisplayBufferInWindow( const win32_offscreen_buffer &Buffer,HDC DeviceContext,
			    int WindowWidth, int WindowHeight)
{
  // Place in the window
  StretchDIBits(DeviceContext,
		 0, 0, WindowWidth, WindowHeight,
		 0, 0, Buffer.Width, Buffer.Height,
		 Buffer.Memory,
		 &Buffer.Info,
		 DIB_RGB_COLORS,
		 SRCCOPY);
}


// DIB = Device Independent Bitmap

LRESULT CALLBACK MainWindowCallback(HWND Window,
	UINT   Message,
	WPARAM WParam,
	LPARAM LParam)
{
	LRESULT Result = 0;

	switch (Message)
	{
		case WM_SIZE:
		{
			//OutputDebugStringA("WM_SIZE\n");
		} break;

		case WM_CLOSE:
		{
			//TODO: handle this witha a message to the user?
			GlobalRunning = false;
		} break;

		case WM_ACTIVATEAPP:
		{
			OutputDebugStringA("WM_ACTIVATEAAPP\n");
		} break;

		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			uint32 VKCode = WParam;
			bool WasDown = (LParam & (1 << 30)) != 0;
			bool IsDown = (LParam & (1 << 31)) == 0;

			if (WasDown != IsDown)
			{
				if (VKCode == 'W')
				{
				}
				else if (VKCode == 'A')
				{
				}
				else if (VKCode == 'S')
				{
				}
				else if (VKCode == 'D')
				{
				}
				else if (VKCode == 'Q')
				{
				}
				else if (VKCode == 'E')
				{
				}
				else if (VKCode == VK_UP)
				{
				}
				else if (VKCode == VK_LEFT)
				{
				}
				else if (VKCode == VK_DOWN)
				{
				}
				else if (VKCode == VK_RIGHT)
				{
				}
				else if (VKCode == VK_ESCAPE)
				{
				}
				else if (VKCode == VK_SPACE)
				{
					OutputDebugStringA("ESCAPE - ");
					OutputDebugStringA(WasDown ? "Was DOWN - " : "Was UP   - ");
					OutputDebugStringA(IsDown ? "Is  DOWN\n" : "Is UP\n");
				}
				//else
				//{
				//	Result = DefWindowProc(Window, Message, WParam, LParam);
				//}
			}
			//else
			//{
			//	Result = DefWindowProc(Window, Message, WParam, LParam);
			//}
		} break;

		case WM_DESTROY:
		{
			//TODO: handle this with a error - recreate window?
			GlobalRunning = false;
		} break;

		case WM_PAINT:
		{
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(Window, &Paint);
			win32_window_dimension Dimension = Win32GetWindowDimension(Window);
			Win32DisplayBufferInWindow(GlobalBackBuffer, DeviceContext, 
				Dimension.Width, Dimension.Height);
			EndPaint(Window, &Paint);

		} break;

		default:
		{
			//		OutputDebugStringA("default\n");
			Result = DefWindowProc(Window, Message, WParam, LParam);
		} break;
	}

	return Result;
}		    
	
int CALLBACK
WinMain(HINSTANCE Instance,
	HINSTANCE PrevInstance,
	LPSTR CommandLine,
	int ShowCode)
{
  Win32LoadXInput();
  WNDCLASS WindowClass = {}; // zero initialize

  Win32ResizeDIBSection(GlobalBackBuffer, 1280, 720);
  
  WindowClass.style = CS_HREDRAW | CS_VREDRAW; // always redraw when resize
  WindowClass.lpfnWndProc = MainWindowCallback;
  WindowClass.hInstance = Instance;
  WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
  //WindowClass.hIcon; // Todo: add Icon
  //WindowClass.lpszMenuName = ; // create like File, Ed,t Options, Help 
  WindowClass.lpszClassName = "HandmadeHeroWindowClass";

  if(RegisterClassA(&WindowClass))
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
      HDC DeviceContext = GetDC(Window);
      
      GlobalRunning = true;
      int Xoffset = 0;
      int Yoffset = 0;
      
      while (GlobalRunning){
	
	MSG Message;
	while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
	{
	  if(Message.message == WM_QUIT)
	  {
	    GlobalRunning = false;
	  }
	  TranslateMessage(&Message);
	  DispatchMessage(&Message);
	}

	for(DWORD ControllerIndex = 0; ControllerIndex < XUSER_MAX_COUNT; ++ControllerIndex)
	{
	  XINPUT_STATE ControllerState;
	  if(XInputGetState(ControllerIndex, &ControllerState) == ERROR_SUCCESS)
	  {
	    // Note(Tata): This controller is plugged in

	    XINPUT_GAMEPAD *Pad = &ControllerState.Gamepad;

	    bool Up = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
            bool Down = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
            bool Left = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
            bool Right = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
            bool Start = (Pad->wButtons & XINPUT_GAMEPAD_START);
            bool Back = (Pad->wButtons & XINPUT_GAMEPAD_BACK);
            bool LeftShoulder = (Pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
            bool RightShoulder = (Pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
            bool AButton = (Pad->wButtons & XINPUT_GAMEPAD_A);
            bool BButton = (Pad->wButtons & XINPUT_GAMEPAD_B);
            bool XButton = (Pad->wButtons & XINPUT_GAMEPAD_X);
            bool YButton = (Pad->wButtons & XINPUT_GAMEPAD_Y);

	    int16 StickX = Pad->sThumbLX;
	    int16 StickY = Pad->sThumbLY;

	    int16 StickRX = Pad->sThumbRX;

	    if(AButton)
	      Yoffset +=2;
	    
	  }
	  else
	  {
	    // Note : The controller is not available
	  }
	}
        RenderWeirdGradiend(GlobalBackBuffer, Xoffset, Yoffset);

	win32_window_dimension Dimension = Win32GetWindowDimension(Window);
	
	Win32DisplayBufferInWindow(GlobalBackBuffer, DeviceContext,
				   Dimension.Width, Dimension.Height);

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
