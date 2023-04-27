all: ircs_ss ircs_cs
	@echo "build all"

ircs_ss: src/ircs_ss.cpp src/Buffer.cpp src/Log.cpp src/Image.cpp
	g++ -o ircs_ss -I src/vendor src/ircs_ss.cpp src/Buffer.cpp src/Log.cpp src/Image.cpp
ircs_cs: src/ircs_cs.cpp src/Buffer.cpp src/Log.cpp src/Image.cpp
	g++ -o ircs_cs -I src/vendor src/ircs_cs.cpp src/Buffer.cpp src/Log.cpp src/Image.cpp

test: json_test read_res_test
	./json_test
	./read_res_test
json_test: src/test/json_test.cpp
	g++ -o json_test -I src/vendor src/test/json_test.cpp
	
read_res_test: src/test/read_res_test.cpp
	g++ -o read_res_test -I src/vendor src/test/read_res_test.cpp