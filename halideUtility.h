//
// Created by hrttsh-mac on 2018/10/12.
//

#ifndef HALIDETEST_HALIDEUTILITY_H
#define HALIDETEST_HALIDEUTILITY_H

#include "Halide.h"
#include "opencv2/opencv.hpp"

using namespace Halide;

//中でcv::imreadをコール．
Buffer<uint8_t> imread(cv::String name);

//中でcv::imwriteをコール．
void imwrite(cv::String name, const Buffer<uint8_t>& src);
void imwrite16(cv::String name, const Buffer<uint16_t>& src);

//HalideのbufferからOpenCVのMatへ変換
void convertHalide2Mat(const Buffer<uint8_t>& src, cv::Mat& dest);

//OpenCVのMatからHalideのbufferへ変換
void convertMat2Halide(cv::Mat& src, Buffer<uint8_t>& dest);

//ucharのバッファ表示用
void imshow(cv::String name, const Buffer<uint8_t>& src);

//shortのバッファ表示用．可視化のデータはucharなのでオフセットとスケールで０～２５５へのキャストを調整
void imshow16(cv::String name, const Buffer<int16_t>& src, double offset = 0.0, double scale = 1.0);

//アルファブレンドをして出力を比較デバッグ用関数
void guiAlphaBlend(Buffer<uint8_t>& src1, Buffer<uint8_t>& src2, cv::String name = "alpha blend");


#endif //HALIDETEST_HALIDEUTILITY_H
