TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(deployment.pri)
qtcAddDeployment()

TARGET = $${TARGET}$${TARGET_POSTFIX}
OBJECTS_DIR = $${EXAMPLES_ROOT}/build_$${QT_ARCH}/$${TARGET}
MOC_DIR = $$OBJECTS_DIR/.moc
RCC_DIR = $$OBJECTS_DIR/.qrc
UI_DIR = $$OBJECTS_DIR/.u

PROJECT_DIR = $${PWD}/../GyroData
SOURCES += $${PROJECT_DIR}/main.cpp
OTHER_FILES+=$${PROJECT_DIR}/CMakeLists.txt

win32{
    msvc{
        contains(QT_ARCH,i386) {
            LIBS += -lglut32
        }else:contains(QT_ARCH,x86_64) {
            LIBS += -lglut64
        }
    }

}else:ios {
}else:macx{
    LIBS += -framework OpenGL
    LIBS += -framework GLUT
}else:unix{
    LIBS += -L/usr/lib/x86_64-linux-gnu/mesa -lGL
    LIBS += -L/usr/lib/x86_64-linux-gnu -lGLU
    LIBS+=-lglut
}

#message ("GYRO LIB $${LIBS}")
#message ("GYRO DESTDIR $${DESTDIR}")
#message ("GYRO OUT_PWD $${OUT_PWD}")
