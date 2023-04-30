# gcc flag
CXX:=g++
CXXFLAGS:=-Wall -Wextra -std=c++17

#opencv

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

# ircs_ss
IRCS_SS_SRC=src/ircs_ss.cpp \
src/Buffer.cpp \
src/Log.cpp \
src/Image.cpp \
src/Socket.cpp \
src/IRRenderer.cpp


#=========================Application=====================
.PHONY: all
all: bin/ircs_ss bin/ircs_cs
	@echo "build all"


bin/ircs_ss: ${IRCS_SS_SRC}
	${CXX} ${CXXFLAGS} ${OPENCV_INCLUDE}  -I src/vendor ${IRCS_SS_SRC} ${OPENCV_LDFLAGS} -o bin/ircs_ss
bin/ircs_cs: src/ircs_cs.cpp src/Buffer.cpp src/Log.cpp src/Image.cpp src/Socket.cpp
	${CXX} ${CXXFLAGS}  -I src/vendor src/ircs_cs.cpp src/Buffer.cpp src/Log.cpp src/Image.cpp src/Socket.cpp -o bin/ircs_cs


#========================test==============================
.PHONY: test
test: bin/json_test bin/read_res_test bin/opencv_test
	@echo "=============test start==============="
	./bin/json_test
	./bin/read_res_test
	./bin/opencv_test
	@echo "=============test stop================"
bin/json_test: src/test/json_test.cpp
	${CXX} ${CXXFLAGS} -o bin/json_test -I src/vendor src/test/json_test.cpp
	
bin/read_res_test: src/test/read_res_test.cpp
	${CXX} ${CXXFLAGS} -o bin/read_res_test -I src/vendor src/test/read_res_test.cpp


bin/opencv_test: src/test/opencv_test.cpp
	${CXX} ${CXXFLAGS} ${OPENCV_INCLUDE}  src/test/opencv_test.cpp ${OPENCV_LDFLAGS} -o bin/opencv_test 

#========================clean=============================
.PHONY: clean
clean:
	rm -rf bin/*