cmake_minimum_required(VERSION 3.0)

include_directories(
    include
)

find_package(pybind11 REQUIRED)

SET(LINK_libs dnn gflags fmt ${os_base_lib})

add_library(model_task MODULE src/model_task_bindings.cpp src/model_task.cpp)
#add_library(model_task MODULE src/model_task.cpp)
target_link_libraries(model_task PRIVATE pybind11::module ${LINK_libs})

#add_executable(run_resnet_nv12 src/main.cc src/model_task.cpp)
#target_link_libraries(run_resnet_nv12 ${LINK_libs})

#install(TARGETS run_resnet_nv12 DESTINATION ${RELEASE_BIN_DIR})