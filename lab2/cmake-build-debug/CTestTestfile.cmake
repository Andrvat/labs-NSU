# CMake generated Testfile for 
# Source directory: C:/template/lab2
# Build directory: C:/template/lab2/cmake-build-debug
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(testlab2.exe "C:/template/lab2/test/testlab2.exe" "./lab2")
set_tests_properties(testlab2.exe PROPERTIES  DEPENDS "lab2" _BACKTRACE_TRIPLES "C:/template/modules/test_runner.cmake;23;add_test;C:/template/modules/test_runner.cmake;0;;C:/template/modules/common_lab.cmake;3;include;C:/template/modules/common_lab.cmake;0;;C:/template/lab2/CMakeLists.txt;9;include;C:/template/lab2/CMakeLists.txt;0;")
