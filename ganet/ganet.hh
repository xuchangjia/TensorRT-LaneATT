#pragma once

#include <iostream>
#include <NvInfer.h>
#include <opencv2/core.hpp>

class Logger : public nvinfer1::ILogger {
  public:
    explicit Logger(Severity severity = Severity::kWARNING)
        : reportable_severity(severity) {}

    void log(Severity severity, const char* msg) noexcept {
        if (severity > reportable_severity) {
            return;
        }
        switch (severity) {
            case Severity::kINTERNAL_ERROR:
                std::cerr << "INTERNAL_ERROR: ";
                break;
            case Severity::kERROR:
                std::cerr << "ERROR: ";
                break;
            case Severity::kWARNING:
                std::cerr << "WARNING: ";
                break;
            case Severity::kINFO:
                std::cerr << "INFO: ";
                break;
            default:
                std::cerr << "UNKNOWN: ";
                break;
        }
        std::cerr << msg << std::endl;
    }
    Severity reportable_severity;
};

class GANet {
  public:
    GANet(const std::string& engine_path);
          
    ~GANet();

    void Detect(const cv::Mat& raw_img);

  private:
    void LoadEngine(const std::string& engine_path);
    
    Logger g_logger_;
    cudaStream_t stream_;
    nvinfer1::ICudaEngine* engine_;
    nvinfer1::IExecutionContext* context_;

    void* buffers_[4];
    int buffer_size_[4];
    std::vector<float> image_data_;
    std::vector<float> confidence_map_;
    std::vector<float> offset_map_;
    std::vector<float> error_map_;
};
