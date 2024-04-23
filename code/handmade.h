#if !defined(HANDMADE_H)

struct game_offscreen_buffer
{
  void *Memory;
  int Width;
  int Height;
  int Pitch;
};

internal void GameUpdateAndRencer(game_offscreen_buffer *Buffer, int BlueOffset, int GreenOffset);

#define HANDMADE_H
#endif
