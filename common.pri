PROJECT_PATH = $$PWD
SRC_PATH = $$PWD/src
RELEASE_PATH = $$PWD/release
DEBUG_PATH = $$PWD/debug

OPENCV_PATH = C:\Programs\opencv\RELEASE
#### OPENCV INCLUDE ####
INCLUDEPATH += $$OPENCV_PATH\install\include
LIBS += $$OPENCV_PATH\bin\libopencv_core320.dll
LIBS += $$OPENCV_PATH\bin\libopencv_calib3d320.dll
LIBS += $$OPENCV_PATH\bin\libopencv_imgproc320.dll
LIBS += $$OPENCV_PATH\bin\libopencv_imgcodecs320.dll
LIBS += $$OPENCV_PATH\bin\libopencv_highgui320.dll
