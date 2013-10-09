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
    list<FurPoint> points;
    bool drawing = false;
    Surface texture;
    Vec2i mouseLoc;
    float stepX, stepY, stepZ;
	params::InterfaceGlRef	mParams;
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
    mPerlin = Perlin();
    gl::clear(Color(0.0f, 0.0f, 0.0f));
    texture = loadImage( loadUrl(Url("http://img.ffffound.com/static-data/assets/6/08616aac741d9fc92c9f0e823ffbe6505b700c25_m.gif") ));
    mouseLoc = Vec2i( getWindowCenter() );
    stepX = 0.001f;
    stepY = 0.001f;
    stepZ = 0.001f;
    mParams = params::InterfaceGl::create( "Furballs", Vec2i( 300, 100 ) );
    mParams->addParam( "stepX", &stepX, "step=0.001 precision=6");
    mParams->addParam( "stepY", &stepY, "step=0.001 precision=6");
    mParams->addParam( "stepZ", &stepZ, "step=0.001 precision=6");
}

void FurballsApp::mouseDown( MouseEvent event) {
    drawing = true;
}

void FurballsApp::mouseUp( MouseEvent event) {
    drawing = false;
}

void FurballsApp::mouseMove( MouseEvent event ) {
    mouseLoc = event.getPos();
}

void FurballsApp::mouseDrag( MouseEvent event ) {
    mouseLoc = event.getPos();
}

void FurballsApp::update() {
    if( drawing ) {
        Vec2f velocity = Vec2f::zero();
        float lifetime = randFloat( 20.0f, 1000.0f );
        float radius = randFloat( 1.0f, 5.0f );
        float randX = randInt( 0, texture.getWidth() );
        float randY = randInt( 0, texture.getHeight() );
        Vec2i randLoc( randX, randY );
        ColorA8u c = texture.getPixel( randLoc );
        points.push_back( FurPoint( mouseLoc, velocity, lifetime, radius, c ) );
    }
}

void FurballsApp::draw() {
    for(list<FurPoint>::iterator p = points.begin(); p != points.end(); ++p){
        if(!p->isDead) {
            for(int i = 0; i < 1000; i++){
                (*p).update(mPerlin,stepX,stepY,stepZ);
            p->draw();
            }
        }
    }
    mParams->draw();

}

CINDER_APP_BASIC( FurballsApp, RendererGl )
