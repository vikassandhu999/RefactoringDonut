#include<stdio.h>
#include<math.h>
#include<string.h>

static char *const shade_chars = ".,-~:;=!*#$@";

int main() {
    const int buffer_size = 1760;
    const char space = ' ';
    const char new_line = '\n';
    float A = 0;
    float B = 0;
    char buffer[buffer_size];
    float z[buffer_size];
    printf("\x1b[2J");
    for (;;) {
        memset(buffer, space, buffer_size);
        memset(z, 0, buffer_size * 4);
        double delta_theta = 0.07;
        double delta_phi = 0.02;
        double TWO_PI = 6.28;
        for (float theta = 0; TWO_PI > theta; theta += delta_theta) {
            for (float phi = 0; TWO_PI > phi; phi += delta_phi) {
                float sin_phi = sin(phi);
                float cos_phi = cos(phi);
                float cos_theta = cos(theta);
                float sin_theta = sin(theta);
                float sin_A =sin(A);
                float cos_A = cos(A);
                float cos_B = cos(B);
                float sin_B = sin(B);
                float h = cos_theta + 2;
                float D = 1 / (sin_phi * h * sin_A + sin_theta * cos_A + 5);
                float t = sin_phi * h * cos_A - sin_theta * sin_A;
                int x = 40 + 30 * D * (cos_phi * h * cos_B - t * sin_B);
                int y = 12 + 15 * D * (cos_phi * h * sin_B + t * cos_B);
                int o = x + 80 * y;
                int N =  8 * ((sin_theta * sin_A - sin_phi * cos_theta * cos_A) * cos_B - sin_phi * cos_theta * sin_A - sin_theta * cos_A - cos_phi * cos_theta * sin_B);
                if (22 > y && y > 0 && x > 0 && 80 > x && D > z[o]) {
                    z[o] = D;
                    buffer[o] = shade_chars[N > 0 ? N : 0];
                }
            }
        }
        printf("\x1b[H");
        for (int k = 0; buffer_size >= k; k++)
            putchar(k % 80 ? buffer[k] : new_line);
        A += 0.04;
        B +=0.02;
    }
}
