EXAMPLES_DIR=/home/tiennm/working/web-sdk/examples_insight #change it in your PC

TARGET   = MentalCommandDemo$${TARGET_POSTFIX}
TEMPLATE = app
DESTDIR  = $${EXAMPLES_DIR}/bin

DEPENDPATH += $${EXAMPLES_DIR}
DEPENDPATH += $${EXAMPLES_DIR}/include
DEPENDPATH += $${EXAMPLES_DIR}/lib

INCLUDEPATH += $$DEPENDPATH

LIBS += -L$${EXAMPLES_DIR}/lib/x64 -ledk -ledk_utils_linux -liomp5 -lcrypto -lssl

HEADERS += \
    ../../examples_insight/example4/MentalCommandControl.h \
    ../../examples_insight/example4/Socket.h

SOURCES += \
    ../../examples_insight/example4/MentalCommandControl.cpp \
    ../../examples_insight/example4/MentalCommandDemo.cpp \
    ../../examples_insight/example4/Socket.cpp
