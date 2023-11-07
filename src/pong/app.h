#pragma once

#include <stdint.h>
#include <windows.h>

typedef uint32_t u32;

#include "render.h"

namespace App {

static bool running = true;

LRESULT CALLBACK
window_callback(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  LRESULT result = 0;

  switch (message) {
    case WM_DESTROY:
      running = false;
      break;
    case WM_SIZE:
      Render::calc_buffer(window);
      break;
    default:
      result = DefWindowProc(window, message, wparam, lparam);
  }

  return result;
}

void
init_window(HINSTANCE instance, WNDCLASS* wc)
{
  wc->hInstance = instance;
  wc->lpfnWndProc = window_callback;
  wc->style = CS_HREDRAW | CS_VREDRAW;
  wc->lpszClassName = "PongWindowClass";

  RegisterClass(wc);
}

void
create_window(HINSTANCE& instance, WNDCLASS* wc, HWND& window)
{
  window = CreateWindow(wc->lpszClassName,
                        "Pong Game",
                        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        1280,
                        950,
                        0,
                        0,
                        instance,
                        0);
}

} // namespace App
