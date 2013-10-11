#include "FurPoint.h"
#include "cinder/Vector.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Perlin.h"
#include "cinder/Rand.h"

FurPoint::FurPoint(Vec2f _pos, Vec2f _vel, float _lifeTime, float _radius, ColorA8u c) {
    position = _pos;
    velocity = _vel;
    lifetime = _lifeTime;
    radius = _radius;
    age = 0.0f;
    isDead = false;
    noiseVec = Vec2f::zero();
    kuler = c;
}

void FurPoint::update(const Perlin &perlin, float stepX, float stepY, float stepZ){
    if (1.0f - age / lifetime <= 0.0f) isDead = true;
    if (position.x < 0 || position.x > getWindowWidth()) isDead = true;
    if (position.y < 0 || position.y > getWindowHeight()) isDead = true;
    
    noiseFloat = perlin.fBm( position.x * stepX, position.y * stepY, getElapsedFrames() * stepZ );
    noiseVec.x = cos(((noiseFloat) * M_PI_2) * 10.0f);
    noiseVec.y = sin(((noiseFloat) * M_PI_2) * 10.0f);
    velocity += noiseVec;
    velocity *= 0.5f;
    position += velocity;
    age++;
}

void FurPoint::draw(){
    float r = ci::lmap(age, 0.1f, lifetime, 1.0f, radius);
    gl::enable(GL_LINE_SMOOTH);
    gl::color(kuler);
    gl::drawSolidCircle(position, r);
}