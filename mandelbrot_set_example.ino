#include <TFT.h>  // Arduino LCD library
#include <time.h>


const uint16_t screen_height = 80;
const uint16_t screen_width = 160;


int8_t color_change = 32;
int16_t current_color = 32;

uint16_t max_mandelbrot_computation_count = 100;


#define cs   6
#define dc   7
#define rst  5
TFT TFTscreen = TFT(cs, dc, rst);


uint16_t get_pixel_color(double x,
                         double y,
                         uint16_t max_mandelbrot_computation_count)
{
  double run_x = x;
  double run_y = y;

  uint16_t i = 0;
  for (i = 0 ; (run_x*run_x) + (run_y*run_y) <= 4 && i < max_mandelbrot_computation_count; i ++)
  {
    double xtemp = run_x*run_x - run_y * run_y + x;
    run_y = 2 * run_x * run_y + y;
    run_x = xtemp;
  }
  if (i < max_mandelbrot_computation_count)
  {
    return 0xFFFF;
  }
  else
  {
    return 0x0000;
  }
}

struct screen_coordinates
{
  double x1;
  double x2;
  double y1;
  double y2;
};

// HERE ALL IS FINE

void draw_set_on_screen(TFT* TFTscreen,
                        const uint16_t screen_height,
                        const uint16_t screen_width, 
                        const struct screen_coordinates Screen_coords,
                        const uint16_t max_mandelbrot_computation_count)
{
  for (int16_t row = 0 ; row < screen_height; row ++)
  {
    const double delta_h = abs(Screen_coords.y1 - Screen_coords.y2) / screen_height;
    const double delta_w = abs(Screen_coords.x1 - Screen_coords.x2) / screen_width;
    for (int16_t cell = 0 ; cell < screen_width; cell ++)
    {
       const double y = Screen_coords.y1 + (delta_h * row);
       const double x = Screen_coords.x1 + (delta_w * cell);

       TFTscreen->drawPixel(cell, 26 + row, get_pixel_color(x, y, max_mandelbrot_computation_count));
    }
  }
}


void setup()
{
  Serial.begin(9600); 

  TFTscreen.begin();
  TFTscreen.background(255, 255, 255); 

    screen_coordinates a = {
    .x1 = -2,
    .x2 = 2,
    .y1 = -1,
    .y2 = 1
  };
  draw_set_on_screen(&TFTscreen, screen_height, screen_width, a, 20);
}

void loop()
{
  double target_x_coord = // 0.25;//
  (-0.34853774148008254 + -0.34831493420245574) / 2.0;
  double target_y_coord = // 0.0; /
  (-0.6065922085831237 + -0.606486596104741) / 2.0;

  screen_coordinates initial_coords = {
    .x1 = -2.0,
    .x2 = 2.0,
    .y1 = -1.0,
    .y2 = 1.0
  };

  double zoom_step = 0.3;

  for (uint16_t i = 1; ; i ++)
  {
    screen_coordinates new_coords = {
      .x1 = initial_coords.x1 + (target_x_coord - initial_coords.x1) * zoom_step,
      .x2 = initial_coords.x2 - (initial_coords.x2 - target_x_coord) * zoom_step,
      .y1 = initial_coords.y1 + (target_y_coord - initial_coords.y1) * zoom_step, 
      .y2 = initial_coords.y2 - (initial_coords.y2 - target_y_coord) * zoom_step, 
    };

    draw_set_on_screen(&TFTscreen, screen_height, screen_width, new_coords, i * 3 + 30);
    initial_coords = new_coords;
  }
}// som_soroka@ukr.net