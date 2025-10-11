import libmodel_task
import numpy as np
import onnxruntime
def main():

    model = libmodel_task.ModelTask()
    model_path = "xxx.bin"
    loaded_data1 = np.fromfile("xxx.npy", dtype=np.float32)
    loaded_data2 = np.fromfile("xxx.npy", dtype=np.float32)
    # loaded_data1 = loaded_data1.reshape(1, 39)
    # loaded_data2 = loaded_data2.reshape(1, 10, 39)

    model.ModelInit(model_path)
    loaded_data_list = [loaded_data2, loaded_data1]
    model_result = model.ModelInfer(loaded_data_list)
    model_result = np.array(model_result)
    print(model_result)

if __name__ == "__main__":
    main()