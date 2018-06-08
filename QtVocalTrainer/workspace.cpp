#include "workspace.h"
#include "TimeUtils.h"
#include <iostream>
#include "QtUtils/qtutils.h"
#include "zoomcontroller.h"
#include "player.h"
#include "qmlpitchinputreader.h"
#include "OpenGLUtils.h"

using namespace CppUtils;
using namespace std;

static const int PITCH_RADIUS = 3;

static GLuint LoadShadersFromFiles(const char* vertexShaderFileName, const char* fragmentShaderFileName) {
    std::string prefix = ":shaders/";
    QByteArray vertexShader = QtUtils::readAllFromFile((prefix + vertexShaderFileName).data());
    QByteArray fragmentShader = QtUtils::readAllFromFile((prefix + fragmentShaderFileName).data());
    return OpenGLUtils::LoadShaders(vertexShader.data(), fragmentShader.data());
}

Workspace::Workspace(QWidget *parent) : QOpenGLWidget(parent) {
    devicePixelRatio_ = devicePixelRatio();

    // 75 fps
    QtUtils::startRepeatedTimer(this, [=] {
        if (workspaceDrawer.getIntervalsPerSecond() > 0) {
            update();
        }
        return true;
    }, 1000 / 75);

    connect(ZoomController::instance(), &ZoomController::zoomChanged, this, &Workspace::zoomChanged);
    connect(Player::instance(), &Player::isPlayingChanged, this, &Workspace::isPlayingChanged);
    connect(Player::instance(), &Player::sourceChanged, this, &Workspace::playerSourceChanged);
    connect(Player::instance(), &Player::seekChanged, this, &Workspace::seekChanged);

    connect(ZoomController::instance(), &ZoomController::firstPitchChanged, this, &Workspace::firstPitchChanged);

    QmlPitchInputReader* pitchInputReader = QmlPitchInputReader::instance();
    workspaceDrawer.setPitchesCollector(pitchInputReader);

    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
}

void Workspace::firstPitchChanged() {
    workspaceDrawer.setFirstPitchPerfectFrequencyIndex(
            ZoomController::instance()->getFirstPitchPerfectFrequencyIndex());
}

void Workspace::isPlayingChanged(bool isPlaying) {
    if (isPlaying) {
        workspaceDrawer.setIntervalsPerSecond(Player::instance()->getBeatsPerMinute() / 60.0);
    } else {
        workspaceDrawer.setIntervalsPerSecond(0);
    }
}

void Workspace::playerSourceChanged() {
    VxFile *vxFile = new VxFile(Player::instance()->getVxFile());
    renderingQueue.post([=] {
        workspaceDrawer.setVxFile(vxFile);
    });
}

void Workspace::zoomChanged() {
    auto* zoom = ZoomController::instance();
    workspaceDrawer.setIntervalWidth((float)zoom->getIntervalWidth());
    workspaceDrawer.setIntervalHeight((float)zoom->getIntervalHeight());
}

void Workspace::seekChanged(double seek) {
    workspaceDrawer.setHorizontalOffset(seek * workspaceDrawer.getIntervalWidth() *
            workspaceDrawer.getIntervalsPerSecond());
}

void Workspace::initializeGL() {
    QOpenGLWidget::initializeGL();

    std::cout<<"GLSL version = "<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<"\n";

    zoomChanged();
    firstPitchChanged();
    workspaceDrawer.setGridColor({0x8B, 0x89, 0xB6, 0x33});
    workspaceDrawer.setAccentGridColor({0x8B, 0x89, 0xB6, 0x80});
    workspaceDrawer.setPitchGraphColor({0xFF, 0x5E, 0x85, 0xFF});
    workspaceDrawer.setPitchColor({0x6E, 0x7E, 0xC5, 0xFF});
    workspaceDrawer.setPitchRadius(PITCH_RADIUS);
    //workspaceDrawer.setIntervalsPerSecond(3);
    glDisable(GL_DEPTH_TEST);

//    programId = LoadShadersFromFiles("vertexshader.glsl", "fragmentshader.glsl");
}

void Workspace::resizeGL(int w, int h) {
    QOpenGLWidget::resizeGL(w, h);
    workspaceDrawer.resize(w, h, devicePixelRatio_);
}

static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
};

void Workspace::paintGL() {
    QOpenGLWidget::paintGL();
    renderingQueue.process();

//    // This will identify our vertex buffer
//    GLuint vertexbuffer;
//    // Generate 1 buffer, put the resulting identifier in vertexbuffer
//    glGenBuffers(1, &vertexbuffer);
//    // The following commands will talk about our 'vertexbuffer' buffer
//    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//    // Give our vertices to OpenGL.
//    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    // 1st attribute buffer : vertices
//    glEnableVertexAttribArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//    glVertexAttribPointer(
//            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//            3,                  // size
//            GL_FLOAT,           // type
//            GL_FALSE,           // normalized?
//            0,                  // stride
//            (void*)0            // array buffer offset
//    );
//    // Draw the triangle !
//    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
//    glDisableVertexAttribArray(0);
//
//    glUseProgram(programId);
    workspaceDrawer.draw();
}
