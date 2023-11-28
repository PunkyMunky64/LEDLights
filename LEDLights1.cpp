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
void test_ShaderLED();

void main() {
    test_TickLED();
}

float velocity(float) {
    return 1.0;
}
float isqLaw(float) {
    return 100;
}
float redshift(float) {
    return 1.0;
}
Colors::RGBu8 missleColor(float) {
    return Colors::RGBu8(255, 0, 0);
}
float missleProbability(float dt, float) {
    return dt / 1000;
}

void test_TickLED() {
    constexpr int leds = 900;
    u8 data[leds][3];
    Colors::HSVu8 n(0, 255, 255);
    Colors::HSVu8 m(135, 200, 200);
    srand(time(NULL));
    
    TickLEDs controller = TickLEDs((u8*)&data, leds, Blending::add);
    MissleStarter controller2((u8*)&data, leds, missleProbability, missleColor, isqLaw, redshift, velocity);
    //MovingSource point1 = MovingSource(0.2, Colors::RGBu8(255, 0 ,0), 900.0, 9000.0, 450.0, 200, 200, leds);
    //MovingSource point2 = MovingSource(0.3, Colors::RGBu8(0, 255, 0), 900.0, 9000.0, 450.0, 200, 200, leds);
    MovingSource* point3 = new MovingSource(0.4, Colors::RGBu8(0, 0, 255), 900.0, 0.0, 4500.0, 450.0, 200, 200, leds);
    //controller.add_entity(&point1);
    //controller.add_entity(&point2);
    controller.add_entity(point3);
    LEDGraphics g(leds, 3, (u8*)&data, 800, 800);
    g.set_custom_configuration(square_i_cosine_lambda(leds), square_i_sine_lambda(leds), square_partition_size_lambda(leds));
    g.bind_to_active_gfx();
    controller.set_active();
    auto thr = std::thread(TickLEDs::run_active);
    Gfx::run_active(0, nullptr);
}

class Instance : public ShaderLEDs {
public:
    Colors::RGBu8 poll(float time_ms, int i) override {
        return Colors::RGBu8(250, 0, 0);
    }
    explicit Instance(u8* stream, int led_count) : ShaderLEDs(stream, led_count) {}
};

void test_ShaderLED() {
    constexpr int leds = 900;
    u8 data[leds][3];
    auto cont = ConstantLight(Colors::RGBu8(255, 120, 2), (u8*)&data, leds);
    class Instance : public ShaderLEDs {
    public:
        Colors::RGBu8 poll(float time_ms, int i) override {
            //return Colors::RGBu8(abs((int)((time_ms/5)+(i * 510 / 900)) % 510 - 255), 0, 0);
            return Colors::HSVu8(abs(((int)(i * 358 / 900 + time_ms / 10) % 358 - 179)), 255, 255).to_rgb();
        }
        explicit Instance(u8* stream, int led_count) : ShaderLEDs(stream, led_count) {}
    };
    Instance inst((u8*)&data, leds);
    ShaderLEDs* controller = &inst;
    LEDGraphics g(leds, 3, (u8*)&data, 800, 800);
    g.set_custom_configuration(square_i_cosine_lambda(leds), square_i_sine_lambda(leds), square_partition_size_lambda(leds));
    g.bind_to_active_gfx();
    controller->set_active();
    auto thr = std::thread(ShaderLEDs::run_active);
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
