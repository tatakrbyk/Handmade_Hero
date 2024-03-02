# Handmade Hero
Following the Handmade Hero streams by Casey Muratori. [Handmade Hero](https://handmadehero.org/).
Almost all code here is authored by Casey Muratori.

## Days
- [Day2: Opening a Win32 Window](#day-2-opening-a-win32-window)
- [Day 3: Allocating a Back Buffer](#day-3-allocating-a-back-buffer)
- [Day 4: Animating the Back Buffer](#day-4-animating-the-back-buffer)


### Day 2: Opening a Win32 Window

- `WNDCLASS`, `RegisterClass`
- `GetModuleHandle`
- `OutputDebugString`
- `DefWindowProc`
- `CreateWindowEx`
- `GetMessage`, `TranslateMessage`, `DispatchMessage`
- `BeginPaint`, `EndPaint`, `PatBlt`

### Day 3: Allocating a Back Buffer

- `Bitmap`
- `GetClientRect`
- `CreateDIBSection`
- `StretchDIBits`
- `DeleteObject`
- `CreateCompatibleDC`

### Day 4: Animating the Back Buffer

- `VirtualFree`, `VirtualProtect`
- Set `biHeight` to negative value so we the image origin if top-left
- Render a simple gradient. Each pixel has a value of form `0xXXRRGGBB`
- use `PeekMessage` instead of `GetMessage`, because it doesn't block
- `GetDC`, `ReleaseDC`


### Day 5: Windows Graphics Review

- `Stack Overflow debug` 
- Use `win32_offscreen_buffer` to bundle all global variables
- Create the back buffer just once, move it out of `WM_SIZE`