# gcc flag
CXX:=g++
CXXFLAGS:=-Wall -Wextra -std=c++17

OPENCV_HOME:=/home/vo17245/program/opencv
OPENCV_INCLUDE:=-I ${OPENCV_HOME}/include/opencv4
OPENCV_LDFLAGS:= -L${OPENCV_HOME}/lib \
-lopencv_stitching \
-lopencv_objdetect \
-lopencv_calib3d \
-lopencv_features2d \
-lopencv_highgui \
-lopencv_imgcodecs \
-lopencv_video \
-lopencv_photo \
-lopencv_ml \
-lopencv_imgproc \
-lopencv_flann  \
-lopencv_core


all: ircs_ss ircs_cs
	@echo "build all"

ircs_ss: src/ircs_ss.cpp src/Buffer.cpp src/Log.cpp src/Image.cpp src/Socket.cpp
	${CXX} ${CXXFLAGS} -o ircs_ss -I src/vendor src/ircs_ss.cpp src/Buffer.cpp src/Log.cpp src/Image.cpp src/Socket.cpp
ircs_cs: src/ircs_cs.cpp src/Buffer.cpp src/Log.cpp src/Image.cpp src/Socket.cpp
	${CXX} ${CXXFLAGS} -o ircs_cs -I src/vendor src/ircs_cs.cpp src/Buffer.cpp src/Log.cpp src/Image.cpp src/Socket.cpp

test: json_test read_res_test bin/opencv_test
	./json_test
	./read_res_test
	./bin/opencv_test
json_test: src/test/json_test.cpp
	${CXX} ${CXXFLAGS} -o json_test -I src/vendor src/test/json_test.cpp
	
read_res_test: src/test/read_res_test.cpp
	${CXX} ${CXXFLAGS} -o read_res_test -I src/vendor src/test/read_res_test.cpp


bin/opencv_test: src/test/opencv_test.cpp
	${CXX} ${CXXFLAGS} ${OPENCV_INCLUDE}  src/test/opencv_test.cpp ${OPENCV_LDFLAGS} -o bin/opencv_test 
