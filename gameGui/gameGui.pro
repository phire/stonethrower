
QT += opengl

SOURCES += \
    main.cpp \
    glWidget.cpp \
    model.cpp \
    building.cpp

HEADERS += \
    glWidget.h \
    model.h \
    building.h

OTHER_FILES += \
    vertexShader.vsh \
    fragmentShader.fsh \
    lightingVertexShader.vsh \
    lightingFragmentShader.fsh \
    singleColourVertexShader.vsh \
    singleColourFragmentShader.fsh \
    resources/car.obj

RESOURCES += \
    resources.qrc

INCLUDEPATH += ../simulation
LIBS += ../simulation/libsimulation.a
PRE_TARGETDEPS += ../simulation/libsimulation.a

unix {
    macx {
    }
    else {
       LIBS += -lGLU
    }
}

QMAKE_CXXFLAGS += -std=c++11
