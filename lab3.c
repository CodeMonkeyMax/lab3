
#include "FPToolkit.c"
#include "M2d_matrix_toolsS.c"
#include "max_poly_tools.c"
#include <math.h>
#include <stdio.h>

// initialize vars here
#define MAXPTS 59000
#define MAXPOLYS 57500

double centerpoint[MAXPOLYS][2]; // Definitely keep this, dummy
double x[MAXPOLYS][MAXPTS], y[MAXPOLYS][MAXPTS];
int numpolys;
int psize[MAXPOLYS];

// TODO: I will need to change the format of these variables slightly,
// as they are not quite right for this use case. I think I need to
// get rid of the OBJECTS entirely

// OBJECTIVE: create a poly.
// clicks inside will G_draw a circle of one color,
// clicks outside will G_draw a circle of another color.

int occlude_and_redraw() { return 0; }

// Returns 1 if point p is outside a line on polygon poli. Else 0
int testpoint(double p[2], int poli) {
  // make lines & test lines? probably don't need to make lines, just test
  // points in for loop with my signature "end-case if statement" for
  // cleanliness
  // double lines[MAXPTS / 2][2][2], ymin, ymax, xmin, xmax;
  int size = psize[poli];
  // for each line in poly: check centerpoint
  int j;
  for (int i = 0; i < size; i++) {
    j = i + 1;
    if (i == size - 1) {
      j = 0;
    }
    double dx = (x[poli][j] - x[poli][i]), dy = (y[poli][j] - y[poli][i]);
    if (((dx * p[1] - dy * p[0]) - (dx * y[poli][i] - dy * x[poli][i])) *
            ((dx * centerpoint[poli][1] - dy * centerpoint[poli][0]) -
             (dx * y[poli][i] - dy * x[poli][i])) <
        0) {
      printf(" outside.\n");
      return 1;
    }
  }
  printf(" inside.\n");
  return 0;
}

void run_test() {
  for (int y = 0; y < screen.y; y += 10) {
    for (int x = 0; x < screen.x; x += 10) {
      double point[2] = {x, y};

      if (testpoint(point, 0) == 1) {
        G_rgb(1, 0, 0);
      } else {
        G_rgb(0, 1, 0);
      }

      G_circle(x, y, 2);
    }
  }
}

void findcenterpoint(double out[2], int poli) {
  double s[2] = {0, 0}; // sum
  for (int i = 0; i < psize[poli]; i++) {
    s[0] += x[poli][i];
    s[1] += y[poli][i];
  }
  out[0] = s[0] / psize[poli];
  out[1] = s[1] / psize[poli];
}

int main() {
  double swidth = 450;
  double sheight = 450;
  screen.x = swidth;
  screen.y = sheight;
  G_init_graphics(screen.x, screen.y);
  printf("graphics init success\n");
  G_rgb(0, 0, 0);
  G_clear();

  // double centerpoint[2];
  // draw 1st polygon
  G_rgb(0.8, 0, 0.4);
  psize[0] = click_and_save(x[0], y[0]);
  printf("click_and_save 1 success\n");
  findcenterpoint(centerpoint[0], 0);
  G_fill_circle(centerpoint[0][0], centerpoint[0][1], 3);

  G_rgb(0.8, 0, 0.4);
  float done_area_y = 20;
  G_fill_rectangle(0, 0, 500, done_area_y);

  double in[2];
  do {
    printf("click: ");
    G_wait_click(in);

    if (testpoint(in, 0) == 1) {
      G_rgb(1, 0, 0);
    } else {
      G_rgb(0, 1, 0);
    }

    G_circle(in[0], in[1], 3);
  } while (in[1] > done_area_y);

  // run_test();

  //G_wait_key();

  // draw 2nd polygon
  // numpoints[1] = click_snap_and_save(x[1], y[1]);
  // clip 1st polygon by 2nd polygon & redraw
}
