#pragma once

#include <stdint.h>
#include <windows.h>

typedef uint32_t u32;

#include "renderer.h"

namespace App {

static bool running = true;

LRESULT CALLBACK
main_window_callback(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
  LRESULT result = 0;

  switch (message) {
    case WM_CLOSE:
    case WM_DESTROY:
      running = false;
      break;
    case WM_SIZE:
      Renderer::calc_window_buffer(window);
      break;
    default:
      result = DefWindowProc(window, message, wparam, lparam);
  }

  return result;
}

void
init_window(HINSTANCE& instance, WNDCLASS* wc)
{
  wc->hInstance = instance;
  wc->style = CS_HREDRAW | CS_VREDRAW;
  wc->lpfnWndProc = main_window_callback;
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
                        720,
                        0,
                        0,
                        instance,
                        0);
}

} // namespace App