cmake_minimum_required(VERSION 3.0)

include_directories(
    include
    /usr/include/hobot
    deps_aarch64/gflags/include
    deps_aarch64/glog/include
    deps_aarch64/hlog/include
    deps_aarch64/eigen/include
    deps_aarch64/fmt/include
    deps_aarch64/nlohmann/include
    deps_aarch64/opencv/include
    deps_aarch64/openssl/include
    deps_aarch64/protobuf/include
)

link_directories(
    /usr/local/lib
    /usr/lib/aarch64-linux-gnu
    /usr/hobot/lib
    deps_aarch64/gflags/lib
    deps_aarch64/glog/lib
    deps_aarch64/hlog/lib
    deps_aarch64/fmt/lib

)

find_package(pybind11 REQUIRED)

SET(LINK_libs hbucp dnn gflags hlog fmt ${os_base_lib} )

add_library(model_task MODULE src/model_task_bindings.cpp src/model_task.cpp)
target_link_libraries(model_task PRIVATE pybind11::module ${LINK_libs})

add_executable(run_resnet_nv12 src/main.cc src/model_task.cpp)
target_link_libraries(run_resnet_nv12 ${LINK_libs})

#install(TARGETS run_resnet_nv12 DESTINATION ${RELEASE_BIN_DIR})