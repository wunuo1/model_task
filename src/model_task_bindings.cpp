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
        .def("ModelInfer", [](ModelTask& self, py::list input_list) {
            // 创建 std::vector<std::vector<float>> 来存储数据
            std::vector<std::vector<float>> data;

            // 遍历 Python 中的列表，转换为 C++ 的 vector
            for (auto item : input_list) {
                py::array_t<float> inner_array = item.cast<py::array_t<float>>();
                py::buffer_info buf_info = inner_array.request();
                float* ptr = static_cast<float*>(buf_info.ptr);

                std::vector<float> inner_vector(ptr, ptr + buf_info.size);
                data.push_back(inner_vector);
            }

            // 调用 ModelTask 的 ModelInfer 方法，返回 std::vector<std::vector<float>>
            std::vector<std::vector<float>> result = self.ModelInfer(data);
            
            // 创建一个 py::list 用来返回二维列表
            py::list result_list;
            for (const auto& inner_vec : result) {
                py::list inner_list;
                for (const auto& value : inner_vec) {
                    inner_list.append(value);
                }
                result_list.append(inner_list);
            }

            return result_list;
        });

}
