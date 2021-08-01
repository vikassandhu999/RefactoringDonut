#include<stdio.h>
#include<math.h>
#include<string.h>

static char *const shade_chars = ".,-~:;=!*#$@";
static char *const clear_screen_chars = "\x1b[2J";
static char *const set_cursor_to_start_chars = "\x1b[H";
const char space = ' ';
const char new_line = '\n';

struct Screen {
    int height;
    int width;
    char *buffer;
};

void display_frame(struct Screen screen) {
    int buffer_size = screen.width * screen.height;
    printf("%s", set_cursor_to_start_chars);
    for (int k = 0; buffer_size >= k; k++)
        putchar(k % screen.width ? screen.buffer[k] : new_line);
}

void load_next_frame(struct Screen screen,float A, float B, float *z) {
    int width = screen.width;
    int height = screen.height;
    float delta_theta = 0.07;
    float delta_phi = 0.02;
    float TWO_PI = 6.28;
    for (float theta = 0; TWO_PI > theta; theta += delta_theta) {
        for (float phi = 0; TWO_PI > phi; phi += delta_phi) {
            float sin_phi = sin(phi);
            float cos_phi = cos(phi);
            float cos_theta = cos(theta);
            float sin_theta = sin(theta);
            float sin_A = sin(A);
            float cos_A = cos(A);
            float cos_B = cos(B);
            float sin_B = sin(B);
            float h = cos_theta + 2;
            float D = 1 / (sin_phi * h * sin_A + sin_theta * cos_A + 5);
            float t = sin_phi * h * cos_A - sin_theta * sin_A;
            int x = width / 2 + 30 * D * (cos_phi * h * cos_B - t * sin_B);
            int y = (height/2 + 1) + 15 * D * (cos_phi * h * sin_B + t * cos_B);
            int o = x + width * y;
            int N = 8 * ((sin_theta * sin_A - sin_phi * cos_theta * cos_A) * cos_B - sin_phi * cos_theta * sin_A -
                         sin_theta * cos_A - cos_phi * cos_theta * sin_B);
            if (height > y && y > 0 && x > 0 && width > x && D > z[o]) {
                z[o] = D;
                screen.buffer[o] = shade_chars[N > 0 ? N : 0];
            }
        }
    }
}

int main() {
    float A = 0;
    float B = 0;
    double delta_A = 0.04;
    double delta_B = 0.02;
    struct Screen screen;
    size_t width = 80;
    size_t height = 22;
    size_t buffer_size = width * height;
    screen.width = width;
    screen.height = height;
    char buffer[buffer_size];
    screen.buffer = buffer;
    float z[buffer_size];
    printf("%s", clear_screen_chars);
    while (1) {
        memset(buffer, space, buffer_size);
        memset(z, 0, buffer_size * 4);
        load_next_frame(screen,A, B,z);
        display_frame(screen);
        A += delta_A;
        B += delta_B;
    }
}
