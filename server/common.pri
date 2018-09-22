WITH_CUDA{
    DEFINES+=WITH_CUDA
}
#opengl give better performance on video show?
WITH_OPENGL{
    DEFINES+=WITH_OPENGL
}
unix:{#currently we only support win and linux
    DEFINES+=IS_UNIX
}else{
    DEFINES+=IS_WIN
}
CONddFIG +=c++11ss

message("common.pri")
