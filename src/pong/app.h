#pragma once

#include <stdint.h>
#include <windows.h>

typedef uint32_t u32;

#include "render.h"

namespace App {

static bool running = true;

/**
 * @brief Window procedure callback function for handling window messages.
 * 
 * @param window A handle to the window.
 * @param message The message to be processed.
 * @param wparam Additional message information.
 * @param lparam Additional message information.
 * @return The result of the message processing.
 */
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

/**
 * @brief Initializes the window class.
 * 
 * @param instance A handle to the instance of the application.
 * @param wc A pointer to a WNDCLASS structure.
 */
void
init_window(HINSTANCE instance, WNDCLASS* wc)
{
  wc->hInstance = instance;
  wc->lpfnWndProc = window_callback;
  wc->style = CS_HREDRAW | CS_VREDRAW;
  wc->lpszClassName = "PongWindowClass";

  RegisterClass(wc);
}

/**
 * @brief Creates the application window.
 * 
 * @param instance A handle to the instance of the application.
 * @param wc A pointer to a WNDCLASS structure.
 * @param window A reference to a handle to the window.
 */
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
