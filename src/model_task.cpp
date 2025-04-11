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

    int input_memSize = input[i].properties.alignedByteSize;
    // std::cout<<"input_memSize: "<<input_memSize<<std::endl;
    // std::cout<<input[i].properties.validShape.dimensionSize[0]<<std::endl;
    // std::cout<<input[i].properties.validShape.dimensionSize[1]<<std::endl;
    HB_CHECK_SUCCESS(hbUCPMallocCached(&input[i].sysMem, input_memSize, 0),
                     "hbUCPMallocCached failed");

    // Show how to get input name
    const char *input_name;
    HB_CHECK_SUCCESS(hbDNNGetInputName(&input_name, dnn_handle, i),
                     "hbDNNGetInputName failed");
    LOGI("input[{}] name is {}", i, input_name);
  }

  hbDNNTensor *output = output_tensor;
  for (int i = 0; i < output_count; i++) {
    HB_CHECK_SUCCESS(
        hbDNNGetOutputTensorProperties(&output[i].properties, dnn_handle, i),
        "hbDNNGetOutputTensorProperties failed");
    int output_memSize = output[i].properties.alignedByteSize;
    
    output_memSize = (output_memSize + 31) & ~31;

    HB_CHECK_SUCCESS(hbUCPMallocCached(&output[i].sysMem, output_memSize, 0),
                     "hbUCPMallocCached failed");
    
    // Show how to get output name
    const char *output_name;
    HB_CHECK_SUCCESS(hbDNNGetOutputName(&output_name, dnn_handle, i),
                     "hbDNNGetOutputName failed");
    LOGI("output[{}] name is {}", i, output_name);
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


std::vector<std::vector<float>> ModelTask::ModelInfer(std::vector<std::vector<float>> &input){
  std::vector<hbDNNTensor> input_tensors_;
  std::vector<hbDNNTensor> output_tensors_;
  input_tensors_.resize(input_count_);
  output_tensors_.resize(output_count_);
  prepare_tensor(input_tensors_.data(), output_tensors_.data(), dnn_handle_);


  //load input
  for(int i = 0; i < input_count_; i++){
    float32_t *data_dst = reinterpret_cast<float32_t *>(input_tensors_[i].sysMem.virAddr);
    for(int j = 0; j < input[i].size(); j++){
      *data_dst = input[i][j];
      data_dst++;
    }
    hbUCPMemFlush(&input_tensors_[i].sysMem, HB_SYS_MEM_CACHE_CLEAN);
  }

  hbUCPTaskHandle_t task_handle{nullptr};
  hbDNNTensor *output = output_tensors_.data();
  std::vector<std::vector<float>> results;
  std::vector<float> result;
  {
    // generate task handle
    hbDNNInferV2(&task_handle, output, input_tensors_.data(), dnn_handle_);

    // submit task
    hbUCPSchedParam ctrl_param;
    HB_UCP_INITIALIZE_SCHED_PARAM(&ctrl_param);
    ctrl_param.backend = HB_UCP_BPU_CORE_ANY;
    hbUCPSubmitTask(task_handle, &ctrl_param);

    // wait task done
    hbUCPWaitTaskDone(task_handle, 0);
    for(int i = 0; i < output_count_; i++){
      hbUCPMemFlush(&output_tensors_[i].sysMem, HB_SYS_MEM_CACHE_INVALIDATE);
      float* data = reinterpret_cast<float *>(output[i].sysMem.virAddr);
      
      int dimensionSize = 1;
      int d_index = 0;
      while(output[i].properties.validShape.dimensionSize[d_index] != 0){
        dimensionSize *= output[i].properties.validShape.dimensionSize[d_index];
        d_index++;
      }

      for (auto j = 0; j < dimensionSize; j++) {
        float score = data[j];
        result.push_back(score);
      }
      results.push_back(result);
    }
    
    hbUCPReleaseTask(task_handle);
  }

  for (int i = 0; i < input_count_; i++) {
    hbUCPFree(&(input_tensors_[i].sysMem));
  }
  // free output mem
  for (int i = 0; i < output_count_; i++) {
    hbUCPFree(&(output_tensors_[i].sysMem));
  }
  
  return results;

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
//   float32_t *data_dst = reinterpret_cast<float32_t *>(input_tensors_[0].sysMem.virAddr);
//   for(int i = 0; i < numbers.size(); i++){
//     *data_dst++ = numbers[i];
//     std::cout<<numbers[i]<<" ";
//   }

//   hbUCPMemFlush(&input_tensors_[0].sysMem, HB_SYS_MEM_CACHE_CLEAN);
//   return 0;
// }

// bool ModelTask::LoadInput(std::vector<float> &input){
//   float32_t *data_dst = reinterpret_cast<float32_t *>(input_tensors_[0].sysMem.virAddr);
//   float a = 0;
//   for(int i = 0; i < input.size(); i++){
//     *data_dst++ = input[i];
//     a = a + input[i];
//   }
//   std::cout<<"sum:"<<a<<std::endl;
//   hbUCPMemFlush(&input_tensors_[0].sysMem, HB_SYS_MEM_CACHE_CLEAN);
//   return 0;
// }