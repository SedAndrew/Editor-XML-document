#-------------------------------------------------
#
# Project created by QtCreator 2019-03-19T18:14:39
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Editor-XML-document
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += PRO_FILE_PWD=$$sprintf("\"\\\"%1\\\"\"", $$_PRO_FILE_PWD_)

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

#QMAKE_CXXFLAGS += -std=c++14

DEFINES += PRO_FILE_PWD=$$sprintf("\"\\\"%1\\\"\"", $$_PRO_FILE_PWD_)
DEFINES += PRO_DIR_SETTING=$$sprintf("\"\\\"%1/settings\\\"\"", $$_PRO_FILE_PWD_)

SOURCES += \
        document.cpp \
        main.cpp \
        mainwindow.cpp \
        element.cpp \
        models/itemdelegatepaint.cpp \
        models/table/tablemodel.cpp \
        models/tree/treemodel.cpp \
        models/treeitemdelegatepaint.cpp \
        session.cpp \
        widgets/addelementform.cpp \
        widgets/controllingsessions.cpp \
        widgets/findform.cpp \
        widgets/tabwidget.cpp \
        widgets/treemodelcompleter.cpp \
        widgets/workspace.cpp \
        widgets/sidebar.cpp \
        widgets/buttondividspace.cpp \
        xmlbuilder.cpp

HEADERS += \
        document.h \
        mainwindow.h \
        element.h \
        models/itemdelegatepaint.h \
        models/table/tablemodel.h \
        models/tree/treemodel.h \
        models/treeitemdelegatepaint.h \
        session.h \
        widgets/addelementform.h \
        widgets/controllingsessions.h \
        widgets/findform.h \
        widgets/lableclicked.h \
        widgets/tabwidget.h \
        widgets/treemodelcompleter.h \
        widgets/workspace.h \
        widgets/sidebar.h \
        myaction.h \
        widgets/buttondividspace.h \
        xmlbuilder.h

FORMS   += \
        mainui.ui \
        widgets/addelementform.ui \
        widgets/controllingsessions.ui \
        widgets/findform.ui \
        widgets/workspaceui.ui \
        widgets/sidebar.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
