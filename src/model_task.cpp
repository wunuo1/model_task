#include "model_task.hpp"
#define ALIGN(value, alignment) (((value) + ((alignment)-1)) & ~((alignment)-1))
#define ALIGN_32(value) ALIGN(value, 32)

int prepare_tensor(std::vector<hbDNNTensor> &input_tensor,
                   std::vector<hbDNNTensor> &output_tensor,
                   hbDNNHandle_t dnn_handle) {
  int input_count = 0;
  int output_count = 0;
  HB_CHECK_SUCCESS(hbDNNGetInputCount(&input_count, dnn_handle),
                   "hbDNNGetInputCount failed");
  HB_CHECK_SUCCESS(hbDNNGetOutputCount(&output_count, dnn_handle),
                   "hbDNNGetOutputCount failed");
  // std::cout<<input_count<<std::endl;
  for (int i = 0; i < input_count; i++) {
    hbDNNTensor input;
    HB_CHECK_SUCCESS(
        hbDNNGetInputTensorProperties(&input.properties, dnn_handle, i),
        "hbDNNGetInputTensorProperties failed");

    int input_memSize = input.properties.alignedByteSize;
    HB_CHECK_SUCCESS(hbSysAllocCachedMem(&input.sysMem[0], input_memSize),
                     "hbSysAllocCachedMem failed");
    input_tensor.push_back(input);
    // std::reverse(input_tensor.begin(), input_tensor.end());
  }

  for (int i = 0; i < output_count; i++) {
    hbDNNTensor output;
    HB_CHECK_SUCCESS(
        hbDNNGetOutputTensorProperties(&output.properties, dnn_handle, i),
        "hbDNNGetOutputTensorProperties failed");
    int output_memSize = output.properties.alignedByteSize;
    HB_CHECK_SUCCESS(hbSysAllocCachedMem(&output.sysMem[0], output_memSize),
                     "hbSysAllocCachedMem failed");
    output_tensor.push_back(output);
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
  prepare_tensor(input_tensors_, output_tensors_, dnn_handle_);
  //load input
  for(int i = 0; i < input_count_; i++){
    float *data_dst = reinterpret_cast<float *>(input_tensors_[i].sysMem->virAddr);
    for(int j = 0; j < input[i].size(); j++){
      *data_dst = input[i][j];
      // *data_dst = 0;
      data_dst++;
    }
    hbSysFlushMem(&input_tensors_[i].sysMem[0], HB_SYS_MEM_CACHE_CLEAN);
  }
  // std::cout<<input_tensors_[0].sysMem[0].phyAddr<<std::endl;
  // std::cout<<input_tensors_[1].sysMem->virAddr<<std::endl;
  // std::cout<<input_tensors_.data()<<std::endl;
  // for(int i = 0; i < input_count_; i++){
  //   for(int j = 0; j < input[i].size(); j++){
  //     std::cout<<*(reinterpret_cast<float *>(input_tensors_[i].sysMem[0].virAddr) + j)<<std::endl;
  //   }
  // }

  hbDNNTaskHandle_t task_handle{nullptr};
  hbDNNTensor *output = &(output_tensors_[0]);
  std::vector<std::vector<float>> results;
  std::vector<float> result;
  {
    // generate task handle
    hbDNNInferCtrlParam infer_ctrl_param;
    HB_DNN_INITIALIZE_INFER_CTRL_PARAM(&infer_ctrl_param);
    hbDNNInfer(&task_handle, &output, input_tensors_.data(), dnn_handle_, &infer_ctrl_param);
    
    hbDNNWaitTaskDone(task_handle, 0);
    for(int i = 0; i < output_count_; i++){
      hbSysFlushMem(&output_tensors_[i].sysMem[0], HB_SYS_MEM_CACHE_INVALIDATE);
      float* data = reinterpret_cast<float *>(output[i].sysMem[0].virAddr);
      
      int dimensionSize = 1;
      int d_index = 0;
      while(output[i].properties.validShape.dimensionSize[d_index] != 0){
        dimensionSize *= output[i].properties.validShape.dimensionSize[d_index];
        d_index++;
      }

      for (auto j = 0; j < dimensionSize; j++) {
        float score = data[j];
        // std::cout<<score<<std::endl;
        result.push_back(score);
      }
      results.push_back(result);
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