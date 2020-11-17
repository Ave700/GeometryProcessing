
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT      += core gui opengl
CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DESTDIR = $$PWD/bin

TARGET = QtContourWidg

TEMPLATE = app

INCLUDEPATH += $$PWD/3rdparty/GL/include
INCLUDEPATH += $$PWD/3rdparty/assimp/include
INCLUDEPATH += $$PWD/3rdparty/ImageMagick-7.0.10-Q8/include

SOURCES += \
    3d_math_qt.cpp \
    BasicMeshDataStructures.cpp \
    Camera.cpp \
    Pipeline.cpp \
    contourwidget.cpp \
    lighting_technique.cpp \
    main.cpp \
    mainwindow.cpp \
    mesh.cpp \
    openglwidget.cpp \
    shader.cpp \
    technique.cpp \
    texture.cpp \
    utilities.cpp \
    window.cpp

HEADERS += \
    3d_math_qt.h \
    BasicMeshDataStructures.h \
    Camera.h \
    Pipeline.h \
    colors.h \
    contourwidget.h \
    globals.h \
    lighting_technique.h \
    mainwindow.h \
    mesh.h \
    openglwidget.h \
    shader.h \
    technique.h \
    texture.h \
    utilities.h \
    window.h

FORMS += \
    contourwidget.ui \
    mainwindow.ui


LIBS += opengl32.lib -lglu32

LIBS += -L$$PWD/3rdparty/GL/lib -lglew32d
#LIBS += -L$$PWD/3rdparty/GL/lib -lglui32
LIBS += -L$$PWD/3rdparty/GL/lib -lfreeglut
LIBS += -L$$PWD/3rdparty/assimp/lib -lassimp-vc142-mtd
LIBS += -L$$PWD/3rdparty/ImageMagick-7.0.10-Q8/lib -lCORE_RL_Magick++_
LIBS += -L$$PWD/3rdparty/ImageMagick-7.0.10-Q8/lib -lCORE_RL_MagickCore_
LIBS += -L$$PWD/3rdparty/ImageMagick-7.0.10-Q8/lib -lCORE_RL_MagickWand_

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md \
    resources/downArrow.bmp \
    resources/downArrow.png \
    shaders/SH_default.frag \
    shaders/SH_default.vert \
    shaders/SH_pretty.frag \
    shaders/SH_pretty.fs \
    shaders/SH_pretty.vert \
    shaders/SH_pretty.vs

RESOURCES += \
    downArrow.qrc


