#include <pybind11/pybind11.h>
#include <pybind11/stl.h> 
#include <pybind11/numpy.h>
#include <vector>
#include "model_task.hpp"

namespace py = pybind11;

PYBIND11_MODULE(libmodel_task, m) {
    py::class_<ModelTask>(m, "ModelTask")
        .def(py::init<>())
        .def("ModelInit", &ModelTask::ModelInit)
        // .def("LoadFile", &ModelTask::LoadFile)
        // .def("ModelInfer", &ModelTask::ModelInfer)
        .def("ModelInfer", [](ModelTask& self, py::array_t<float> input_array) {
            // 获取 numpy 数组的 buffer 信息
            py::buffer_info buf_info = input_array.request();
            float* ptr = static_cast<float*>(buf_info.ptr);

            // 创建 std::vector 来存储数据
            std::vector<float> data(ptr, ptr + buf_info.size);

            // 调用 ModelTask 的 ModelInfer 方法
            std::vector<float> result = self.ModelInfer(data);
            return py::array_t<float>(result.size(), result.data());
        });

}
