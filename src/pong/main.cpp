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

  HDC hdc = GetDC(window);

  while (App::running) {

    MSG message;
    while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
      TranslateMessage(&message);
      DispatchMessage(&message);
    }

    // Render::clear_screen(0x000081);
    Render::clear_screen(0xababab);

    // Render::pattern(0xffffff, 800, 800);
    Render::rect_center(20, 20, 0x00000f);
    Render::rect_vertical(50, 50, 250, 0x0000ff);
    Render::rect_vertical(-50, 50, 250, 0x0000ff);
    Render::rect_horizontal(50, 250, 50, 0x0000ff);
    Render::rect_horizontal(-50, 250, 50, 0x0000ff);

    Render::render(hdc);
  }
}
