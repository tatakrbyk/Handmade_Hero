#if !defined (WIN32_HANDMADE_H)

struct win32_offscreen_buffer
{
  // Note: Pixels are always 32-bits wide, Memory order  BB GG RR XX
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

struct win32_sound_output
{
  int SamplesPerSecond;
  uint32 RunningSampleIndex;
  int WavePeriod;
  int BytesPerSample;
  int SecondaryBufferSize;
  real32 tSine;
  int LatencySampleCount;
};
#define WIN32_HANDMADE_H
#endif
