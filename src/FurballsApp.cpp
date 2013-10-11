#include "cinder/app/AppBasic.h"
#include "cinder/ImageIO.h"
#include "cinder/gl/Texture.h"
#include "cinder/Perlin.h"
#include "cinder/Channel.h"
#include "cinder/Vector.h"
#include "cinder/Utilities.h"
#include "cinder/Rand.h"
#include <list>
#include "FurPoint.h"
#include "cinder/gl/TileRender.h"
#include "cinder/Surface.h"
#include "cinder/Url.h"
#include "cinder/params/Params.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class FurballsApp : public AppBasic {
public:
    void prepareSettings(Settings *settings);
	void setup();
    void mouseMove( MouseEvent event );
    void mouseUp ( MouseEvent event );
    void mouseDown ( MouseEvent event );
    void mouseDrag ( MouseEvent event );
    void keyDown( KeyEvent event );
	void update();
	void draw();
    Perlin mPerlin;
    list<FurPoint> mParticles;
    bool mIsDrawing;
    Surface mTexture;
    Vec2i mMousePos;
    float mStepX, mStepY, mStepZ;
	params::InterfaceGlRef mParams;
};

void FurballsApp::keyDown(KeyEvent event){
    if(event.getChar() == 's'){
        string filename = "Dropbox/public/cinder/";
        filename.append(toString(randInt(100000)));
        filename.append(".jpg");
        writeImage(ci::getHomeDirectory() / filename, copyWindowSurface());
    }
    if(event.getChar() == 'c'){
        gl::clear(Color(0.0f, 0.0f, 0.0f));
    }
}

void FurballsApp::prepareSettings(Settings *settings) {
    settings->setWindowSize(1280, 720);
    settings->setFrameRate(60.0f);
}

void FurballsApp::setup() {
    mIsDrawing = false;
    mPerlin = Perlin();
    mPerlin.setOctaves(2);
    gl::clear(Color(0.0f, 0.0f, 0.0f));
    mTexture = loadImage( loadUrl(Url("http://img.ffffound.com/static-data/assets/6/d5f1bbd28cee87195cbc84bd4988c35501d16e3a_m.jpg") ));
    mMousePos = Vec2i( getWindowCenter() );
    mStepX = 0.001f;
    mStepY = 0.001f;
    mStepZ = 0.001f;
    mParams = params::InterfaceGl::create( "Furballs", Vec2i( 300, 100 ) );
    mParams->addParam( "stepX", &mStepX, "step=0.0001 precision=4");
    mParams->addParam( "stepY", &mStepY, "step=0.0001 precision=4");
    mParams->addParam( "stepZ", &mStepZ, "step=0.0001 precision=4");
}

void FurballsApp::mouseDown( MouseEvent event) {
    mIsDrawing = true;
}

void FurballsApp::mouseUp( MouseEvent event) {
    mIsDrawing = false;
}

void FurballsApp::mouseMove( MouseEvent event ) {
    mMousePos = event.getPos();
}

void FurballsApp::mouseDrag( MouseEvent event ) {
    mMousePos = event.getPos();
}

void FurballsApp::update() {
    if( mIsDrawing ) {
        Vec2f velocity = Vec2f::zero();
        float lifetime = randFloat( 20.0f, 1000.0f );
        float radius = randFloat( 1.0f, 5.0f );
        float randX = randInt( 0, mTexture.getWidth() );
        float randY = randInt( 0, mTexture.getHeight() );
        Vec2i randLoc( randX, randY );
        ColorA8u c = mTexture.getPixel( randLoc );
        mParticles.push_back( FurPoint( mMousePos, velocity, lifetime, radius, c ) );
    }
}

void FurballsApp::draw() {
    for(list<FurPoint>::iterator p = mParticles.begin(); p != mParticles.end(); ){
        if(p->isDead) {
            p = mParticles.erase( p );
        }
        else {
            for(int i = 0; i < 100; i ++){
                p->update( mPerlin, mStepX, mStepY, mStepZ );
                p->draw();
            }
            ++p;
        }
    }
    mParams->draw();
    
}

CINDER_APP_BASIC( FurballsApp, RendererGl )
