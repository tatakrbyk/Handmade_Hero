# Handmade Hero
Following the Handmade Hero streams by Casey Muratori. [Handmade Hero](https://handmadehero.org/).
Almost all code here is authored by Casey Muratori.

## Days
- [Day2: Opening a Win32 Window](#day-2-opening-a-win32-window)
- [Day 3: Allocating a Back Buffer](#day-3-allocating-a-back-buffer)
- [Day 4: Animating the Back Buffer](#day-4-animating-the-back-buffer)
- [Day 5: Windows Graphics Review](#day-5-windows-graphics-review)
- [Day 6: Gamepad and Keyboard Input](#day-6-gamepad-and-keyboard-input)
- [Day 7: Initializing DirectSound](#day-7-initializing-directsound)
- [Day 8: Writing a Square Wave to DirectSound](#day-8-writing-a-square-wave-to-directsound)
- [Day 9: Variable-Pitch Sine Wave Output](#day-9-variable-pitch-sine-wave-output)
- [Day 10: QueryPerformanceCounter and RDTSC](#day-10-queryperformancecounter-and-rdtsc)
- [Day 11: The Basics of Platform API Design](#day-11-the-basics-of-platform-api-design)
- [Day 12: Platform-Independent Sound Output](#day-12-platform-independent-sound-output)
- [Day 13: Platform-Independent User Input](#day-13-platform-independent-user-input)

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

### Day 6: Gamepad and Keyboard Input

- `XInput`, `XInputGetState`, `XInputSetState`, `XUSER_MAX_COUNT`
- Loading windows functions ourselves
- `LoadLibrary`, `GetProcAddress`
- `WM_SYSKEYUP`, `WM_SYSKEYDOWN`, `WM_KEYUP`, `WM_KEYDOWN`
- Get IsDown and WasDown status from LParam


### Day 7: Initializing DirectSound

- `DirectSoundCreate`, `SetCooperativeLevel`, `CreateSoundBuffer`, `SetFormat`


### Day 8: Writing a Square Wave to DirectSound

- `Lock`, `Unlock`, `GetCurrentPosition`, `Play`

### Day 9: Variable-Pitch Sine Wave Output

- `sinf`
- `win32_sound_output`, `Win32FillSoundBuffer`
- `tSine`, `LatencySampleCount`
- We need to handle xinput deadzone in the future

### Day 10: QueryPerformanceCounter and RDTSC

- `QueryPerformanceCounter`, `LARGE_INTEGER`, `QuyerPerformanceFrequency`
- `wsprintf`, `__rdtsc`

### Day 11: The Basics of Platform API Design

- Treat our game as a service, rather than the operating system.


### Day 12: Platform-Independent Sound Output

- Move sound rendering logic to handmade.cpp

### Day 13: Platform-Independent User Input

- Define `game_input`, `game_controller_input`, `game_button_state`
- Store OldInput and NewInput and do ping-pang at end of every frame
- Define `ArrayCount` macro