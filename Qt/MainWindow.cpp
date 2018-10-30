#include "MainWindow.h"

#include <QOpenGLWidget>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QQuickItem>
#include <QQmlContext>
#include <QScreen>
#include <QFileDialog>
#include <QMenuBar>
#include <QMacCocoaViewContainer>
#include <QSizePolicy>
#include <QPushButton>

#include "QmlCppBridge.h"
#include "VxApp.h"
#include "PlaybackBounds.h"
#include "Algorithms.h"
#include "PortAudioUtils.h"
#include "SelectMicrophoneDialog.h"
#include "AppSettings.h"
#include <QScrollBar>

constexpr int YARD_STICK_HEIGHT = static_cast<int>(WorkspaceDrawer::YARD_STICK_HEIGHT);
constexpr int HEADER_WITH_SUBHEADER_HEIGHT = 75 + 75 - YARD_STICK_HEIGHT;
constexpr int VERTICAL_SCROLL_WIDTH = 11;
constexpr int BEATS_IN_TACT = 4;
constexpr int MINIMUM_WINDOW_WIDTH = 700;
constexpr double MINIMUM_WINDOW_HEIGHT_RATIO = 0.6;
constexpr int LYRICS_HEIGHT = 53;

#ifdef __APPLE__
#define IS_APPLE true
#else
#define IS_APPLE false
#endif

using namespace CppUtils;
using std::cout;
using std::endl;

MainWindow::MainWindow() :
        BaseMainWindow(QColor::fromRgb(197, 206, 248))
{
    QWidget *centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    AppSettings settings;
    float inputVolume = settings.getInputVolume();
    float outputVolume = settings.getOutputVolume();
    
    // Workspace
#ifdef __APPLE__
    workspaceWidget = new MetalWorkspaceWidget(this);
#else
    workspaceWidget = new OpenGLWorkspaceWidget(this);
#endif
    workspaceWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Lyrics
    lyricsWidget = createQQuickWidget("qrc:/qml/Lyrics.qml");
    lyricsWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    lyricsWidget->setFixedHeight(LYRICS_HEIGHT);
    lyricsWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Header
    QQuickWidget *headerWidget = createQQuickWidget("qrc:/qml/HeaderWithSubHeader.qml");
    headerWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    headerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    headerWidget->setFixedHeight(HEADER_WITH_SUBHEADER_HEIGHT);
    headerWithSubheader = headerWidget->rootObject();
    header = headerWithSubheader->findChild<QQuickItem*>("header");
    assert(header);

    AudioInputManager *audioInputManager = MainController::instance()->getAudioInputManager();
    
    header->setProperty("inputVolume", inputVolume);
    onInputVolumeChanged(inputVolume);
    
    header->setProperty("outputVolume", outputVolume);
    onOutputVolumeChanged(outputVolume);
    
    audioInputManager->addAudioInputLevelMonitor([=] (double level) {
        header->setProperty("microphoneLevel", level);
    });

    // Setup layouts
    auto *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(workspaceWidget);
    mainLayout->addWidget(lyricsWidget);
    centralWidget->setLayout(mainLayout);

	// Scrollbar
    verticalScrollBar = new QScrollBar(IS_APPLE ? nullptr : centralWidget);
#ifdef __APPLE__
    virticalScrollBarNativeWrap = workspaceWidget->addSubWidget(verticalScrollBar);
#endif

    setupMenus();
}

void MainWindow::onOutputVolumeChanged(float value) {
    AppSettings settings;
    settings.setOutputVolume(value);
    AudioInputManager *audioInputManager = MainController::instance()->getAudioInputManager();
    audioInputManager->setOutputVolume(value);
}

void MainWindow::onInputVolumeChanged(float value) {
    AppSettings settings;
    settings.setInputVolume(value);
    AudioInputManager *audioInputManager = MainController::instance()->getAudioInputManager();
    audioInputManager->setInputVolume(value);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    const int width = event->size().width();

    int scrollBarHeight = workspaceWidget->height() - YARD_STICK_HEIGHT - 2;
#ifdef __APPLE__
    virticalScrollBarNativeWrap->setFixedHeight(scrollBarHeight);
#endif
    verticalScrollBar->setFixedHeight(scrollBarHeight);

    int y;
    if (!IS_APPLE) {
        y = verticalScrollBar->mapToParent(workspaceWidget->pos()).y() + YARD_STICK_HEIGHT + 1;
    } else {
        y = YARD_STICK_HEIGHT + 2;
    }
    int x = width - verticalScrollBar->width();
#ifdef __APPLE__
    virticalScrollBarNativeWrap->move(x, y);
#else
    verticalScrollBar->move(x, y);
#endif
}

void MainWindow::setupMenus() {
    QMenu* fileMenu = menuBar()->addMenu("File");
    QAction* openAction = fileMenu->addAction("Open...");
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::onFileOpen);

    QMenu* editMenu = menuBar()->addMenu("Edit");
    QAction* microphoneAction = editMenu->addAction("Select Microphone");
    connect(microphoneAction, &QAction::triggered, this, &MainWindow::onSelectMicrophone);
}

void MainWindow::onFileOpen() {
    QString fileName = QFileDialog::getOpenFileName(
            this, "Select .mvx file for signing", "", "Mvx files(*.mvx);; All files(*)");

    //QString fileName = "/Users/Semyon/Downloads/torero.mvx";
    if (!fileName.isEmpty()) {
        VxApp::instance()->getPlayer()->setSource(fileName);
    }
}

void MainWindow::onSelectMicrophone() {
    (new SelectMicrophoneDialog(this, cpp))->show();
}

void MainWindow::setBoundsSelectionEnabled(bool enabled) {
    MainController::instance()->getPlaybackBoundsSelectionController()->setBoundsSelectionEnabled(enabled);
}

void MainWindow::setShowLyrics(bool value) {
    lyricsWidget->setVisible(value);
}