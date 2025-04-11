import libmodel_task
import numpy as np

def main():

    model = libmodel_task.ModelTask()
    model_path = "xxxxx"
    loaded_data = np.fromfile("xxxxx.bin", dtype=np.float32)
    model.ModelInit(model_path)

    loaded_data_list = [loaded_data]
    model_result = model.ModelInfer(loaded_data_list)
    model_result = np.array(model_result)



if __name__ == "__main__":
    main()