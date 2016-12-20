EXAMPLES_DIR=/home/tiennm/working/web-sdk/examples_insight #change it in your PC
BOOST_DIR=/home/tiennm/working/building/boost_1_51_0

TARGET   = ProfileManagement$${TARGET_POSTFIX}
TEMPLATE = app
DESTDIR  = $${EXAMPLES_DIR}/bin

DEPENDPATH += $${EXAMPLES_DIR}
DEPENDPATH += $${EXAMPLES_DIR}/include
DEPENDPATH += $${EXAMPLES_DIR}/lib
DEPENDPATH += $${BOOST_DIR}

INCLUDEPATH += $$DEPENDPATH

LIBS += -L$${EXAMPLES_DIR}/lib/x64 -ledk -ledk_utils_linux -liomp5 -lcrypto -lssl

HEADERS += \
    ../../examples_insight/example3/ExternalProfileManager.h \
    ../../examples_insight/example3/ProfileControl.h

SOURCES += \
    ../../examples_insight/example3/ExternalProfileManager.cpp \
    ../../examples_insight/example3/ProfileControl.cpp \
    ../../examples_insight/example3/ProfileManagement.cpp
