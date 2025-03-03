#include "model_task.hpp"
#define ALIGN(value, alignment) (((value) + ((alignment)-1)) & ~((alignment)-1))
#define ALIGN_32(value) ALIGN(value, 32)

static int prepare_tensor(hbDNNTensor *input_tensor, hbDNNTensor *output_tensor,
                   hbDNNHandle_t dnn_handle) {
  int input_count = 0;
  int output_count = 0;
  hbDNNGetInputCount(&input_count, dnn_handle);
  
  hbDNNGetOutputCount(&output_count, dnn_handle);

  /** Tips:
   * For input memory size in most cases:
   * *   input_memSize = input[i].properties.alignedByteSize
   * but here for dynamic stride of y and uv，alignedByteSize is not fixed
   * For output memory size:
   * *   output_memSize = output[i].properties.alignedByteSize
   */
  hbDNNTensor *input = input_tensor;
  for (int i = 0; i < input_count; i++) {
    HB_CHECK_SUCCESS(
        hbDNNGetInputTensorProperties(&input[i].properties, dnn_handle, i),
        "hbDNNGetInputTensorProperties failed");

    /** Tips:
     * For input tensor, usually need to pad the input data according to stride obtained from properties.
     * but here for dynamic stride of y and uv，user needs to specify a value which should be 32 bytes aligned for the -1 position in stride.
     * */
    auto dim_len = input[i].properties.validShape.numDimensions;
    
    for (int32_t dim_i = dim_len - 1; dim_i >= 0; --dim_i) {
      if (input[i].properties.stride[dim_i] == -1) {
        auto cur_stride =
            input[i].properties.stride[dim_i + 1] *
            input[i].properties.validShape.dimensionSize[dim_i + 1];
        input[i].properties.stride[dim_i] = ALIGN_32(cur_stride);
      }
    }

    int input_memSize = input[i].properties.stride[0] *
                        input[i].properties.validShape.dimensionSize[0];
    // std::cout<<input[i].properties.validShape.dimensionSize[0]<<std::endl;
    // std::cout<<input[i].properties.validShape.dimensionSize[1]<<std::endl;
    HB_CHECK_SUCCESS(hbSysAllocCachedMem(&input[i].sysMem[0], input_memSize),
                     "hbSysAllocCachedMem failed");
  }

  hbDNNTensor *output = output_tensor;
  for (int i = 0; i < output_count; i++) {
    HB_CHECK_SUCCESS(
        hbDNNGetOutputTensorProperties(&output[i].properties, dnn_handle, i),
        "hbDNNGetOutputTensorProperties failed");
    int output_memSize = output[i].properties.alignedByteSize;
    // std::cout<<output[i].properties.validShape.dimensionSize[0]<<std::endl;
    // std::cout<<output[i].properties.validShape.dimensionSize[1]<<std::endl;
    HB_CHECK_SUCCESS(hbSysAllocCachedMem(&output[i].sysMem[0], output_memSize),
                     "hbUCPMallocCached failed");
  }
  return 0;
}

ModelTask::ModelTask(){

}

ModelTask::~ModelTask(){
  // Step6: release resources
  {
    // free input mem

    // release model
    hbDNNRelease(packed_dnn_handle_);
  }
}
bool ModelTask::ModelInit(std::string &model_path){
    {
        auto modelFileName = model_path.c_str();
        HB_CHECK_SUCCESS(
            hbDNNInitializeFromFiles(&packed_dnn_handle_, &modelFileName, 1),
            "hbDNNInitializeFromFiles failed");
        HB_CHECK_SUCCESS(hbDNNGetModelNameList(&model_name_list_, &model_count_,
                                            packed_dnn_handle_),
                        "hbDNNGetModelNameList failed");
        HB_CHECK_SUCCESS(
            hbDNNGetModelHandle(&dnn_handle_, packed_dnn_handle_, model_name_list_[0]),
            "hbDNNGetModelHandle failed");
    }

  // Step2: prepare input and output tensor
  {
    HB_CHECK_SUCCESS(hbDNNGetInputCount(&input_count_, dnn_handle_),
                     "hbDNNGetInputCount failed");
    HB_CHECK_SUCCESS(hbDNNGetOutputCount(&output_count_, dnn_handle_),
                     "hbDNNGetOutputCount failed");
  }
}


std::vector<float> ModelTask::ModelInfer(std::vector<float> &input){
  std::vector<hbDNNTensor> input_tensors_;
  std::vector<hbDNNTensor> output_tensors_;
  input_tensors_.resize(input_count_);
  output_tensors_.resize(output_count_);
  prepare_tensor(input_tensors_.data(), output_tensors_.data(), dnn_handle_);


  //load input
  float *data_dst = reinterpret_cast<float *>(input_tensors_[0].sysMem[0].virAddr);
  float a = 0;
  for(int i = 0; i < input.size(); i++){
    *data_dst = input[i];
    data_dst++;
  }
  data_dst = reinterpret_cast<float *>(input_tensors_[0].sysMem[0].virAddr);
  for(int i = 0; i < input.size(); i++){
    a = a + *data_dst;
    data_dst++;
  }
  // std::cout<<"sum:"<<a<<std::endl;
  hbSysFlushMem(&input_tensors_[0].sysMem[0], HB_SYS_MEM_CACHE_CLEAN);
  hbDNNTaskHandle_t task_handle{nullptr};
  hbDNNTensor *output = &(output_tensors_[0]);
  std::vector<float> result;
  {
    // generate task handle
    hbDNNInferCtrlParam infer_ctrl_param;
    HB_DNN_INITIALIZE_INFER_CTRL_PARAM(&infer_ctrl_param);
    hbDNNInfer(&task_handle, &output, input_tensors_.data(), dnn_handle_, &infer_ctrl_param);

    hbDNNWaitTaskDone(task_handle, 0);

    hbSysFlushMem(&output_tensors_[0].sysMem[0], HB_SYS_MEM_CACHE_INVALIDATE);
    float* data = reinterpret_cast<float *>(output->sysMem[0].virAddr);
    for (auto i = 0; i < output[0].properties.validShape.dimensionSize[1]; i++) {
      float score = data[i];
      result.push_back(score);
    }

    hbDNNReleaseTask(task_handle);
  }

  for (int i = 0; i < input_count_; i++) {
    hbSysFreeMem(&(input_tensors_[i].sysMem[0]));
  }
  // free output mem
  for (int i = 0; i < output_count_; i++) {
    hbSysFreeMem(&(output_tensors_[i].sysMem[0]));
  }
  
  return result;

}

// bool ModelTask::LoadFile(std::string &file_path){
//   std::ifstream file(file_path, std::ios::binary);
//   if (!file) {
//       std::cerr << "Failed to open the file!" << std::endl;
//       return 1;
//   }
//   file.seekg(0, std::ios::end);
//   size_t size = file.tellg();
//   file.seekg(0, std::ios::beg);

//   // 假设文件中包含多个整数（int）
//   std::vector<float> numbers(size / sizeof(float));

//   // 读取文件中的数据到 vector
//   file.read(reinterpret_cast<char*>(numbers.data()), size);

//   // 检查读取是否成功
//   if (!file) {
//       std::cerr << "Failed to read the file!" << std::endl;
//       return 1;
//   }

//   // for (auto num : numbers) {
//   //     std::cout << num << " ";
//   // }

//   file.close();
//   float *data_dst = reinterpret_cast<float *>(input_tensors_[0].sysMem.virAddr);
//   for(int i = 0; i < numbers.size(); i++){
//     *data_dst++ = numbers[i];
//     std::cout<<numbers[i]<<" ";
//   }

//   hbUCPMemFlush(&input_tensors_[0].sysMem, HB_SYS_MEM_CACHE_CLEAN);
//   return 0;
// }

// bool ModelTask::LoadInput(std::vector<float> &input){
//   float *data_dst = reinterpret_cast<float *>(input_tensors_[0].sysMem.virAddr);
//   float a = 0;
//   for(int i = 0; i < input.size(); i++){
//     *data_dst++ = input[i];
//     a = a + input[i];
//   }
//   std::cout<<"sum:"<<a<<std::endl;
//   hbUCPMemFlush(&input_tensors_[0].sysMem, HB_SYS_MEM_CACHE_CLEAN);
//   return 0;
// }