QT += charts

HEADERS += \
    callout.h \
    view.h \
    getdata.h

SOURCES += \
    callout.cpp \
    main.cpp\
    view.cpp \
    getdata.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/charts/callout
INSTALLS += target
