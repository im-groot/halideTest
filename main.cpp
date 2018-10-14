#include <iostream>
#include "box_filter_integral.h"
#include "box_filter.h"
#include "HalideBuffer.h"
#include "halideUtility.h"

int main(int argc, char **argv) {

    Buffer<uint8_t > src = imread("./gray.png");

    Halide::Runtime::Buffer<uint8_t > input(src.width(), src.height()), output(src.width(), src.height());
    input = *src.get();

    {
        int error = 0;
        for (int i = 0; i < 10; ++i)
        {
            auto start = cv::getTickCount();
            error = box_filter(input, 7, output);
            auto end = cv::getTickCount();
            if (error) {
                printf("Halide returned an error: %d¥n", error);
                return -1;
            }
            std::cout << "width:" << output.width() << " height:" << output.height() << std::endl;
            std::cout << "processing time:" << (end- start) * 1000 / cv::getTickFrequency()  << "[ms]" << std::endl;
            std::cout << std::endl;
        }

        cv::Mat outputMat(src.height(), src.width(), CV_8U);
        for (int y = 0; y < outputMat.rows; ++y)
        {
            for (int x = 0; x < outputMat.cols; ++x)
            {
                outputMat.at<uint8_t>(y,x) = output(x,y);
            }
        }
        cv::imwrite("box_filter.png", outputMat);
    }

    {
        int error = 0;
        for (int i = 0; i < 10; ++i)
        {
            auto start = cv::getTickCount();
            error = box_filter_integral(input, 7, output);
            auto end = cv::getTickCount();
            if (error) {
                printf("Halide returned an error: %d¥n", error);
                return -1;
            }
            std::cout << "width:" << output.width() << " height:" << output.height() << std::endl;
            std::cout << "processing time:" << (end- start) * 1000 / cv::getTickFrequency()  << "[ms]" << std::endl;
            std::cout << std::endl;
        }

        cv::Mat outputMat(src.height(), src.width(), CV_8U);
        for (int y = 0; y < outputMat.rows; ++y)
        {
            for (int x = 0; x < outputMat.cols; ++x)
            {
                outputMat.at<uint8_t>(y,x) = output(x,y);
            }
        }
        cv::imwrite("box_filter_integral.png", outputMat);
    }

    // Everything worked!
    printf("Success!\n");

    return 0;
}