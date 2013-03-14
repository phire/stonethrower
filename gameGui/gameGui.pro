QT += opengl

SOURCES += \
    main.cpp \
    glWidget.cpp \
    model.cpp

HEADERS += \
    glWidget.h \
    model.h

OTHER_FILES += \
    vertexShader.vsh \
    fragmentShader.fsh \
    lightingVertexShader.vsh \
    lightingFragmentShader.fsh \
    singleColourVertexShader.vsh \
    singleColourFragmentShader.fsh

RESOURCES += \
    resources.qrc

INCLUDEPATH += ../simulation
LIBS += ../simulation/libsimulation.a
PRE_TARGETDEPS += ../simulation/libsimulation.a
