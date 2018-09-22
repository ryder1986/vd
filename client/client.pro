#-------------------------------------------------
#
# Project created by QtCreator 2018-07-12T11:30:37
#
#-------------------------------------------------
#greaterThan(QT_MAJOR_VERSION, 4) QT += widgets
#QT       += core  network

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SRC_PATH =../server
#CONFIG+=WITH_CUDA
#WITH_CUDA{
#    DEFINES+=WITH_CUDA
#}
##opengl give better performance on video show?
#WITH_OPENGL{
#    DEFINES+=WITH_OPENGL
#}
#unix:{#currently we only support win and linux
#    DEFINES+=IS_UNIX
#}else{
#    DEFINES+=IS_WIN
#}
#CONFIG +=c++11
include($$SRC_PATH/common.pri)

QT += widgets network
TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    playerwidget.cpp

HEADERS  += mainwindow.h \
    playerwidget.h
FORMS    += mainwindow.ui

HEADERS  += $$SRC_PATH/tool.h
INCLUDEPATH +=$$SRC_PATH/cppjson/include
INCLUDEPATH +=$$SRC_PATH/

HEADERS +=$$SRC_PATH/cppjson/include/json/reader.h \
$$SRC_PATH/cppjson/include/json/writer.h \
$$SRC_PATH/cppjson/include/json/value.h
SOURCES += $$SRC_PATH/tool.cpp
SOURCES += $$SRC_PATH/cppjson/json_reader.cpp $$SRC_PATH/cppjson/json_writer.cpp  $$SRC_PATH/cppjson/json_value.cpp
SOURCES += $$SRC_PATH/videosource.cpp
unix{
LIBS+=  -lopencv_core -lopencv_highgui -lopencv_objdetect \
-lopencv_imgproc -lopencv_ml -lopencv_highgui -lopencv_video -lopencv_videostab -lpthread  -lX11 -lopencv_imgcodecs  -lopencv_videoio #
LIBS+=-lavformat -lavcodec  -lavutil -lswresample
}else{
message(build win32)
INCLUDEPATH+=C:\opencv3.2\opencv\build\include
debug{
LIBS+=-LC:\opencv3.2\opencv\build\x64\vc14\lib -lopencv_world320d
}
release{
LIBS+=-LC:\opencv3.2\opencv\build\x64\vc14\lib -lopencv_world320
}

}



