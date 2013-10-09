#include "cinder/Vector.h"
#include "cinder/Perlin.h"

using namespace ci;
using namespace ci::app;

class FurPoint{
public:
    Vec2f position, velocity, noiseVec;
    float noiseFloat, lifetime, age, radius;
    bool isDead;
    ColorA8u kuler;
    FurPoint(Vec2f _pos, Vec2f _vel, float _lifeTime, float _radius, ColorA8u c);
    void update(const Perlin &perlin, float stepX, float stepY, float stepZ);
    void draw();
};
