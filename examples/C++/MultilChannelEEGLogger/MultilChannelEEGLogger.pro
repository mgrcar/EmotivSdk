EXAMPLES_DIR=/home/tiennm/working/web-sdk/examples_insight #change it in your PC

TARGET   = MultiChannelEEGLogger$${TARGET_POSTFIX}
TEMPLATE = app
DESTDIR  = $${EXAMPLES_DIR}/bin

DEPENDPATH += $${EXAMPLES_DIR}
DEPENDPATH += $${EXAMPLES_DIR}/include
DEPENDPATH += $${EXAMPLES_DIR}/lib

INCLUDEPATH += $$DEPENDPATH

LIBS += -L$${EXAMPLES_DIR}/lib/x64 -ledk -ledk_utils_linux -liomp5 -lcrypto -lssl

SOURCES += \
    ../../examples_insight/example11/main.cpp
