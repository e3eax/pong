#pragma once

namespace Render {

// struct to hold the buffer data
struct Buffer
{
  void* memory;
  int width;
  int height;
  int size;
  BITMAPINFO bitmap_info;
};

static Buffer buffer;

/**
 * @brief Sets the bitmap information for the buffer.
 */
void
set_bitmap_info()
{
  buffer.bitmap_info.bmiHeader.biSize = sizeof(buffer.bitmap_info.bmiHeader);
  buffer.bitmap_info.bmiHeader.biWidth = buffer.width;
  buffer.bitmap_info.bmiHeader.biHeight = buffer.height;
  buffer.bitmap_info.bmiHeader.biPlanes = 1;
  buffer.bitmap_info.bmiHeader.biBitCount = 32;
  buffer.bitmap_info.bmiHeader.biCompression = BI_RGB;
}

/**
 * @brief Calculates the buffer size based on the window size.
 * 
 * @param window A handle to the window.
 */
void
calc_buffer(HWND& window)
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

  set_bitmap_info();
}

/**
 * @brief Clears the screen by setting all pixels in the buffer to the specified color.
 * 
 * @param color The color to set the pixels to.
 */
void
clear_screen(u32 color)
{
  u32* pixel = (u32*)buffer.memory;

  for (int y = 0; y < buffer.height; y++) {
    for (int x = 0; x < buffer.width; x++) {
      *pixel++ = color;
    }
  }
}

/**
 * @brief Renders the contents of the buffer to the specified device context.
 * 
 * @param dc A handle to the device context.
 */
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

/**
 * @brief Clamps a value to the specified range.
 * 
 * @param min The minimum value of the range.
 * @param val The value to clamp.
 * @param max The maximum value of the range.
 * @return The clamped value.
 */
int
clamp(int min, int val, int max)
{
  if (val < min)
    return min;

  if (val > max)
    return max;

  return val;
}

/**
 * @brief Renders a rectangle with the specified color at the specified position and size.
 * 
 * @param offset_x The x-coordinate of the rectangle.
 * @param offset_y The y-coordinate of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color of the rectangle.
 */
void
render_rect(int offset_x, int offset_y, int width, int height, u32 color)
{
  width = clamp(0, width, buffer.width);
  height = clamp(0, height, buffer.height);
  offset_x = clamp(0, offset_x, buffer.width - width);
  offset_y = clamp(0, offset_y, buffer.height - height);

  for (int y = offset_y; y < height + offset_y; y++) {
    u32* pixel = (u32*)buffer.memory + offset_x + y * buffer.width;
    for (int x = offset_x; x < width + offset_x; x++) {
      *pixel++ = color;
    }
  }
}

/**
 * @brief Renders a rectangle with the specified color at the center of the buffer / screen.
 * 
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color of the rectangle.
 */
void
rect_center(int width, int height, u32 color)
{
  int start_x = buffer.width / 2 - width / 2;
  int start_y = buffer.height / 2 - height / 2;

  render_rect(start_x, start_y, width, height, color);
}

/**
 * @brief Renders a rectangle with the specified color at the center of the y/vertical-axis.
 * 
 * @param offset_x The x-coordinate of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color of the rectangle.
 */
void
rect_vertical(int offset_x, int width, int height, u32 color)
{
  if (offset_x < 0) {
    offset_x = offset_x + buffer.width - width;
  }

  int start_y = buffer.height / 2 - height / 2;

  render_rect(offset_x, start_y, width, height, color);
}

/**
 * @brief Renders a rectangle with the specified color at the center of the x/horizontal-axis.
 * 
 * @param offset_y The y-coordinate of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color of the rectangle.
 */
void
rect_horizontal(int offset_y, int width, int height, u32 color)
{
  if (offset_y < 0) {
    offset_y = offset_y + buffer.height - height;
  }

  int start_x = buffer.width / 2 - width / 2;

  render_rect(start_x, offset_y, width, height, color);
}

/**
 * @brief Renders a pattern of rectangles with alternating colors.
 * 
 * @param color The color of the first rectangle in the pattern.
 * @param max_width The maximum width of the pattern.
 * @param max_height The maximum height of the pattern.
 */
void
pattern(u32 color, int max_width = buffer.width, int max_height = buffer.height)
{
  int width = 100;
  int height = 100;

  int x_loop = max_width / width;
  int y_loop = max_height / height;

  for (int y = 0; y < y_loop; y += 2) {
    for (int x = 0; x < x_loop; x++) {
      if (x % 2 == 0) {
        render_rect(x * width, y * height, width, height, 0x000044);
      } else {
        render_rect(x * width, y * height, width, height, color);
      }
    }
  }

  for (int y = 1; y < y_loop; y += 2) {
    for (int x = 0; x < x_loop; x++) {
      if (x % 2 == 0) {
        render_rect(x * width, y * height, width, height, color);
      } else {
        render_rect(x * width, y * height, width, height, 0x000044);
      }
    }
  }
}

} // namespace Buffer
