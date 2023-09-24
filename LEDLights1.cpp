#include <iostream>
#include "graphics.h"
#include "LEDGraphics.h"
#include "colors.h"
#include "blending.h"
#include "TickLEDs.h"
#include "LightPoint.h"
#include <functional>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

typedef unsigned char u8;

void test();
void test_LEDGfx();
void test_TickLED();


void main() {
    test_TickLED();
}

void test_TickLED()  {
    u8 data[900][3];
    Colors::HSVu8 n(0, 255, 255);
    Colors::HSVu8 m(135, 200, 200);
    srand(time(NULL));
    
    TickLEDs controller = TickLEDs((u8*)&data, 900, [](std::vector<Colors::RGBu8> cols) {
        //TODO:
        //Simplify all this sorta code into a "recursive associative binary" function handler,
        //And an optimized version on the abstraction side.
        if (cols.size() == 0) {
            return Colors::RGBu8(0, 0, 0);
        }
        else {
            auto active = cols[0];
            for (int i = 1; i < cols.size(); i++) {
                active = Blending::add_asymptotic(active, cols[i]);
            }
        }
        });
    //LightPoint(std::function<float(float elapsed, int location)> velocity, std::function<float(float dist)> intensity_distance, std::function<Colors::RGBu8(float elapsed, int location)> color, std::function <Colors::RGBu8(Colors::RGBu8, float multiplier)> intensity, int left_max_radius, int right_max_radius);
    std::function<float(float elapsed, int location)> velocity = [](float elapsed, int location) {
        return 1.0;
    };
    std::function<float(float dist)> intensity_distance = [](float dist) {
        return 1.0 - dist * 0.2;
    };
    std::function<Colors::RGBu8(float elapsed, int location)> color = [](float elapsed, int location) {
        return Colors::RGBu8(255, 0, 0);
    };
    std::function <Colors::RGBu8(Colors::RGBu8, float multiplier)> intensity = [](Colors::RGBu8 color, float multiplier) {
        return Colors::RGBu8(color.r * multiplier, color.g * multiplier, color.b * multiplier);
    };
    LightPoint point1 = LightPoint(
        velocity,
        intensity_distance,
        color,
        intensity
        ,5,5,10);
    controller.add_entity(&point1);
    LEDGraphics g(900, 3, (u8*)&data, 800, 800);

    //Square config function:
    //Only works for square, not rectangle

    g.set_custom_configuration(square_i_cosine_lambda(900), square_i_sine_lambda(900), square_partition_size_lambda(900));
    g.bind_to_active_gfx();
    Gfx::run_active(0, nullptr);
}

void test_LEDGfx() {
    u8 data[900][3];
    Colors::HSVu8 n(0, 255, 255);
    Colors::HSVu8 m(135, 200, 200);
    srand(time(NULL));
    for (int i = 0; i < 900; i++) {
        //if (i % 45 > 30) {
            //auto x = Blending::add_asymptotic(n.to_rgb(), m.to_rgb());
            //x.save_to_pointer((u8*)&data + i * 3);
        //}
        //else if (i % 45 > 15) {
            //auto x = Blending::add(n.to_rgb(), m.to_rgb());
            //x.save_to_pointer((u8*)&data + i * 3);
        //}
        //else {
            n.to_rgb().save_to_pointer((u8*)&data + i * 3);
        //}
        if (!(rand() % 20)) n.h++; //1 in 20 chance ish
        if (n.h > 179) {
            n.h = 0;
        }
        //n.s++;
    }
    LEDGraphics g(900, 3, (u8*)&data, 800, 800);

    //Square config function:
    //Only works for square, not rectangle


    g.set_custom_configuration(square_i_cosine_lambda(900), square_i_sine_lambda(900), square_partition_size_lambda(900));
    g.bind_to_active_gfx();
    Gfx::run_active(0, nullptr);
}

void test_display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);        

    glBegin(GL_QUADS);              
    glColor3f(1.0f, 0.0f, 0.0f);    
    glVertex2f(-1.0f, -0.5f);       
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(-0.5f, 0.5f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(-0.0f, -0.5f);   
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(-0.5f, 0.5f);
    glEnd();

    glFlush();  // Render now
}

void test()
{
    Gfx disp(800, 800);
    disp.set_display_func(test_display);
    disp.set_active();
    Gfx::run_active(0, nullptr);
}
