
SOURCES += $$PWD/wind/WindSim.cpp \
$$PWD/wind/rtGetInf.cpp \
$$PWD/wind/rtGetNan.cpp \
$$PWD/wind/windsim_dk1_main.cpp \
$$PWD/wind/windsim_dk1_main_emxAPI.cpp \
$$PWD/wind/windsim_dk1_main_emxutil.cpp \
$$PWD/wind/rt_nonfinite.cpp


DEFINES += _bool_h
INCLUDEPATH += "$$PWD/wind"



unix {

macx {

DEFINES += "_MacOSX"

#LIBS += /Users/fmk/lib/libLapack.a
#LIBS += /Users/fmk/lib/libBlas.a
#LIBS += /usr/local/llvm-gcc/lib/libgfortran.a
LIBS += -llapack
LIBS += -lblas


INCLUDEPATH += .include "./ops"
#INCLUDEPATH += "$(HOME)/OpenSees/DEVELOPER/core"

} else {


DEFINES += "_LINUX"


INCLUDEPATH += "/apps/rappture/dev/include"
INCLUDEPATH += ./include "./ops"

LIBS += -llapack
LIBS += -lblas
LIBS += -lgfortran

SRC += ./ops/Domain.cpp

}
}

win32 {


INCLUDEPATH += C:\Progra~1\Tcl\include
INCLUDEPATH += $$(HOME)\OpenSees\DEVELOPER\core
INCLUDEPATH += "./ops"
LIBS += c:\Users\SimCenter\OpenSees\Win32\lib\release\lapack.lib
LIBS += c:\Users\SimCenter\OpenSees\Win32\lib\release\blas.lib
LIBS += c:\Users\SimCenter\OpenSees\Win32\lib\libifcoremt.lib
LIBS += c:\Users\SimCenter\OpenSees\Win32\lib\libirc.lib
LIBS += c:\Users\SimCenter\OpenSees\Win32\lib\ifconsol.lib
LIBS += c:\Users\SimCenter\OpenSees\Win32\lib\libifport.lib
LIBS += c:\Users\SimCenter\OpenSees\Win32\lib\libmmt.lib
# NOTE THAT THE OpenSees libs have to be created with /MD as opposed to /MT as runtime library
# this is specified in C++ -> Code Generation -> RunTime Library
# this is because Qt must be built with this option as they have memory issues if built /MT


CONFIG += static

QMAKE_LFLAGS_DEBUG += /FORCE:MULTIPLE
QMAKE_LFLAGS_DEBUG += /NODEFAULTLIB:"libc.lib"
QMAKE_LFLAGS_DEBUG += /NODEFAULTLIB:"libcpmt.lib"

QMAKE_LFLAGS_RELEASE += /FORCE:MULTIPLE
QMAKE_LFLAGS_RELEASE += /NODEFAULTLIB:"libc.lib"
QMAKE_LFLAGS_RELEASE += /NODEFAULTLIB:"libcpmt.lib"
#LIBPATH += "C:\Program Files (x86)\Intel\Composer XE\compiler\lib\intel64"


} else {

}


               
