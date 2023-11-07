#pragma once

namespace Renderer {

struct Buffer
{
  int size;
  int width;
  int height;
  void* memory;

  BITMAPINFO bitmap_info;
};

static Buffer buffer;

void
calc_window_buffer(HWND& window)
{

  RECT rect;
  GetClientRect(window, &rect);

  buffer.width = rect.right - rect.left;
  buffer.height = rect.bottom - rect.top;
  buffer.size = buffer.width * buffer.height * sizeof(u32);

  if (buffer.memory)
    VirtualFree(buffer.memory, 0, MEM_RELEASE);

  buffer.memory =
    VirtualAlloc(0, buffer.size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

  buffer.bitmap_info.bmiHeader.biSize = sizeof(buffer.bitmap_info.bmiHeader);
  buffer.bitmap_info.bmiHeader.biWidth = buffer.width;
  buffer.bitmap_info.bmiHeader.biHeight = buffer.height;
  buffer.bitmap_info.bmiHeader.biPlanes = 1;
  buffer.bitmap_info.bmiHeader.biBitCount = 32;
  buffer.bitmap_info.bmiHeader.biCompression = BI_RGB;
}

void
clear_screen(u32 color)
{
  u32* pixel = (u32*)buffer.memory;

  for (int y = 0; y < buffer.height; y++) {
    for (int x = 0; x < buffer.width; x++) {
      *pixel++ = y * x;
    }
  }
}

void
render(HDC& dc)
{
  StretchDIBits(dc,
                0,
                0,
                buffer.width,
                buffer.height,
                0,
                0,
                buffer.width,
                buffer.height,
                buffer.memory,
                &buffer.bitmap_info,
                DIB_RGB_COLORS,
                SRCCOPY);
}

} // namespace App