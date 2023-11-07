#include "app.h"

int WINAPI
WinMain(HINSTANCE instance,
        HINSTANCE prev_instance,
        LPSTR cmd_line,
        int cmd_show)
{
  HWND window = {};
  WNDCLASS window_class = {};

  App::init_window(instance, &window_class);
  App::create_window(instance, &window_class, window);

  HDC dc = GetDC(window);

  while (App::running) {

    MSG message;

    while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
      TranslateMessage(&message);
      DispatchMessage(&message);
    }

    Renderer::clear_screen(0xaa00aa);

    Renderer::render(dc);
  }
}