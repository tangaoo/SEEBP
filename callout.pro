QT += charts

HEADERS += \
    seebp.h

SOURCES += \
    main.cpp \
    seebp.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/charts/callout
INSTALLS += target
