#include <iostream>
#include "graphics.h"
#include "LEDGraphics.h"
#include "colors.h"
#include "blending.h"
#include "TickLEDs.h"
#include "LedBuildingBlocks.h"
#include <functional>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <thread>

typedef unsigned char u8;

void test();
void test_LEDGfx();
void test_TickLED();

void main() {
    test_TickLED();
}

void test_TickLED() {
    constexpr int leds = 900;
    u8 data[leds][3];
    Colors::HSVu8 n(0, 255, 255);
    Colors::HSVu8 m(135, 200, 200);
    srand(time(NULL));
    
    TickLEDs controller = TickLEDs((u8*)&data, leds, Blending::add);
    MovingSource point1 = MovingSource(0.2, Colors::RGBu8(255, 0 ,0), 125, 200, 200, leds);
    controller.add_entity(&point1);
    LEDGraphics g(leds, 3, (u8*)&data, 800, 800);
    g.set_custom_configuration(square_i_cosine_lambda(leds), square_i_sine_lambda(leds), square_partition_size_lambda(leds));
    g.bind_to_active_gfx();
    controller.set_active();
    auto thr = std::thread(TickLEDs::run_active);
    Gfx::run_active(0, nullptr);
}

void test_ShaderLED() {
    constexpr int leds = 900;
    u8 data[leds][3];
    ShaderLEDs controller = ShaderLEDs((u8*)&data, leds);
    //fix these motherfuckers what the hell is going on with explicit constructors and statics???????
    ConstantLight point1 = ConstantLight(0.2, Colors::RGBu8(255, 0, 0), 125, 200, 200, leds);
    controller.add_entity(&point1);
    LEDGraphics g(leds, 3, (u8*)&data, 800, 800);
    g.set_custom_configuration(square_i_cosine_lambda(leds), square_i_sine_lambda(leds), square_partition_size_lambda(leds));
    g.bind_to_active_gfx();
    controller.set_active();
    auto thr = std::thread(TickLEDs::run_active);
    Gfx::run_active(0, nullptr);
}

void test_LEDGfx() {
    u8 data[900][3];
    Colors::HSVu8 n(0, 255, 255);
    Colors::HSVu8 m(135, 200, 200);
    srand(time(NULL));
    for (int i = 0; i < 900; i++) {
        n.to_rgb().save_to_pointer((u8*)&data + i * 3);
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
