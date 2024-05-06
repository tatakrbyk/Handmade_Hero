
#include "handmade.h"

internal void
GameOutputSound(game_sound_output_buffer *SoundBuffer, int ToneHz)
{
  local_persist real32 tSine;
  int16 ToneVolume = 3000;
  int WavePeriod = SoundBuffer->SamplesPerSecond/ToneHz;
  
  int16 *SampleOut = SoundBuffer->Samples;
  for(int SampleIndex = 0; SampleIndex < SoundBuffer->SampleCount; ++SampleIndex)
  {
    // TODO : Draw this out for people
	  
    real32 SineValue = sinf(tSine);
    int16 SampleValue = (int16)(SineValue * ToneVolume);
    *SampleOut++ = SampleValue;
    *SampleOut++ = SampleValue;

    tSine += 2.0f * Pi32 * 1.0f /(real32)WavePeriod;
  
  }
}
internal void
RenderWeirdGradient(game_offscreen_buffer *Buffer, int BlueOffset, int GreenOffset)
{
  uint8 *Row = (uint8 *)Buffer->Memory;

  for(int y = 0; y < Buffer->Height; ++y)
  {
    uint32 *Pixel = (uint32*)Row;

    for(int x = 0; x < Buffer->Width; ++x)
    {
      uint8 Green = (y + GreenOffset);
      uint8 Blue = (x + BlueOffset);

      *Pixel++ = ((Green << 8) | Blue);
    }
    Row += Buffer->Pitch;
  }
}

internal void
GameUpdateAndRender(game_input *Input, game_offscreen_buffer *Buffer,
		     game_sound_output_buffer *SoundBuffer)
{
  local_persist int XOffset = 0;
  local_persist int YOffset = 0;
  local_persist int ToneHz = 256;

  game_controller_input *Input0 = &Input->Controllers[0];
  if(Input0 ->IsAnalog)
  {
    // NOTE: Use analog movement
    YOffset = (int)4.0f*(Input0->EndX);
    ToneHz = 256 + (int)(128.0f*(Input0->EndY));
  }
  if(Input0->Down.EndedDown)
  {
    YOffset += 1;
  }
  GameOutputSound(SoundBuffer, ToneHz);
  RenderWeirdGradient(Buffer, XOffset, YOffset);
}
