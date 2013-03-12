QT += opengl

SOURCES += \
    main.cpp \
    glWidget.cpp

HEADERS += \
    glWidget.h

OTHER_FILES += \
    vertexShader.vsh \
    fragmentShader.fsh \
    lightingVertexShader.vsh \
    lightingFragmentShader.fsh

RESOURCES += \
    resources.qrc
