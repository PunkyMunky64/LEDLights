#include <iostream>
#include "graphics.h"
#include "LEDGraphics.h"
#include "colors.h"
#include "blending.h"
#include "TickLEDs.h"
#include "LedBuildingBlocks.h"
#include <functional>
#include "AnimationLib.h"
#include <time.h>       /* time */
#include <thread>

typedef unsigned char u8;

void test();
void test_LEDGfx();
void test_ShaderLED();
void test_TickLED();


void main() {
    test_TickLED();
}

void test_TickLED() {
    constexpr int leds = 900;
    u8 data[leds][3];
    srand(time(NULL));
    auto controller = TickLEDs_Controllers::waves001_001((u8*)data, leds);

    LEDGraphics* g = new LEDGraphics(leds, 3, (u8*)&data, 800, 800);
    g->set_custom_configuration(square_i_cosine_lambda(leds), square_i_sine_lambda(leds), square_partition_size_lambda(leds));
    g->bind_to_active_gfx();
    controller->set_active();
    auto thr = std::thread(TickLEDs::run_active);
    Gfx::run_active(0, nullptr);
};

void test_ShaderLED() {
    constexpr int leds = 900;
    u8 data[leds][3];
    srand(time(NULL));
    auto controller = ShaderLEDs_Controllers::rainbow002_003((u8*)data, leds); //Can be allocated on stack because it's passed to thread which maintains this stack? maybe? //TODO understand

    LEDGraphics* g = new LEDGraphics(leds, 3, (u8*)&data, 800, 800); //Allocate memory for it because this stack is going to be destroyed by glutInit
    g->set_custom_configuration(square_i_cosine_lambda(leds), square_i_sine_lambda(leds), square_partition_size_lambda(leds));
    g->bind_to_active_gfx();
    controller.get()->set_active();
    auto thr = std::thread(ShaderLEDs::run_active);
    Gfx::run_active(0, nullptr);
}

void test_LEDGfx() {
    constexpr int leds = 900;
    u8 data[leds][3];
    Colors::HSVu8 n(0, 255, 255);
    Colors::HSVu8 m(135, 200, 200);
    srand(time(NULL));
    for (int i = 0; i < leds; i++) {
        n.to_rgb().save_to_pointer((u8*)&data + i * 3);
        if (!(rand() % 20)) n.h++; //1 in 20 chance ish
        if (n.h > 179) {
            n.h = 0;
        }
        //n.s++;
    }
    LEDGraphics g(leds, 3, (u8*)&data, 800, 800);

    //Square config function:
    //Only works for square, not rectangle

    g.set_custom_configuration(square_i_cosine_lambda(leds), square_i_sine_lambda(leds), square_partition_size_lambda(leds));
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