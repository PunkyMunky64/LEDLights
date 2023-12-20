#define PI 3.14159265358979323846
#define rad(x) ((x) * PI / 180.0)
#define degree(x) ((x) * 180.0 / PI)
#define fract(x) ((x) - floor(x))

float mod2pi(float x);
float modnpipi(float x);

float mod2pi(float x) {
    return fract(x / 2. / PI) * 2. * PI;
}

float modnpipi(float x) {
    return fract((x - PI) / 2. / PI) * 2. * PI - PI;
}