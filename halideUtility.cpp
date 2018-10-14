//
// Created by hrttsh-mac on 2018/10/12.
//

#include "halideUtility.h"

//utility function with OpenCV
void convertMat2Halide(cv::Mat& src, Buffer<uint8_t>& dest)
{
    const int ch = src.channels();
    if (ch == 1)
    {
        for (int j = 0; j < src.rows; j++)
        {
            for (int i = 0; i < src.cols; i++)
            {
                dest(i, j) = src.at<uchar>(j, i);
            }
        }
    }
    else if (ch == 3)
    {
        for (int j = 0; j < src.rows; j++)
        {
            for (int i = 0; i < src.cols; i++)
            {
                dest(i, j, 0) = src.at<uchar>(j, 3 * i);
                dest(i, j, 1) = src.at<uchar>(j, 3 * i + 1);
                dest(i, j, 2) = src.at<uchar>(j, 3 * i + 2);
            }
        }
    }
}

Buffer<uint8_t> imread(cv::String name)
{
    cv::Mat a = cv::imread(name, -1);
    std::cout << "width:" << a.cols << std::endl;
    std::cout << "height:" << a.rows << std::endl;
    std::cout << "channnel:" << a.channels() << std::endl;
    if (a.empty()) std::cout << name << " is empty" << std::endl;

    if (a.channels() == 1)
    {
        Buffer<uint8_t> ret(a.cols, a.rows);
        convertMat2Halide(a, ret);

        return ret;
    }
    else if (a.channels() == 3)
    {
        Buffer<uint8_t> ret(a.cols, a.rows, a.channels());
        convertMat2Halide(a, ret);

        return ret;
    }

}

void convertHalide2Mat(const Buffer<uint8_t>& src, cv::Mat& dest)
{
    if (dest.empty()) dest.create(cv::Size(src.width(), src.height()), CV_MAKETYPE(CV_8U, src.channels()));
    const int ch = dest.channels();
    if (ch == 1)
    {
        for (int j = 0; j < dest.rows; j++)
        {
            for (int i = 0; i < dest.cols; i++)
            {
                dest.at<uchar>(j, i) = src(i, j);
            }
        }
    }
    else if (ch == 3)
    {
        for (int j = 0; j < dest.rows; j++)
        {
            for (int i = 0; i < dest.cols; i++)
            {
                dest.at<uchar>(j, 3 * i + 0) = src(i, j, 0);
                dest.at<uchar>(j, 3 * i + 1) = src(i, j, 1);
                dest.at<uchar>(j, 3 * i + 2) = src(i, j, 2);
            }
        }
    }
}

void convertHalide2Mat(const Buffer<uint16_t>& src, cv::Mat& dest)
{
    if (dest.empty()) dest.create(cv::Size(src.width(), src.height()), CV_MAKETYPE(CV_16U, src.channels()));
    const int ch = dest.channels();
    if (ch == 1)
    {
        for (int j = 0; j < dest.rows; j++)
        {
            for (int i = 0; i < dest.cols; i++)
            {
                dest.at<uint16_t >(j, i) = src(i, j);
            }
        }
    }
    else if (ch == 3)
    {
        for (int j = 0; j < dest.rows; j++)
        {
            for (int i = 0; i < dest.cols; i++)
            {
                dest.at<uint16_t>(j, 3 * i + 0) = src(i, j, 0);
                dest.at<uint16_t>(j, 3 * i + 1) = src(i, j, 1);
                dest.at<uint16_t>(j, 3 * i + 2) = src(i, j, 2);
            }
        }
    }
}

void imwrite(cv::String name, const Buffer<uint8_t>& src)
{
    cv::Mat a(cv::Size(src.width(), src.height()), CV_MAKETYPE(CV_8U, src.channels()));
    convertHalide2Mat(src, a);
    cv::imwrite(name, a);
}

void imwrite16(cv::String name, const Buffer<uint16_t>& src)
{
    cv::Mat a(cv::Size(src.width(), src.height()), CV_MAKETYPE(CV_16U, src.channels()));
    convertHalide2Mat(src, a);
    cv::imwrite(name, a);
}

void imshow(cv::String name, const Buffer<uint8_t>& src)
{
    cv::Mat a(cv::Size(src.width(), src.height()), CV_MAKETYPE(CV_8U, src.channels()));
    convertHalide2Mat(src, a);
    cv::imshow(name, a);
}

void imshow16(cv::String name, const Buffer<int16_t>& src, double offset, double scale)
{
    cv::Mat a(cv::Size(src.width(), src.height()), CV_MAKETYPE(CV_8U, src.channels()));

    const int ch = a.channels();
    if (ch == 1)
    {
        for (int j = 0; j < a.rows; j++)
        {
            for (int i = 0; i < a.cols; i++)
            {
                a.at<uchar>(j, i) = cv::saturate_cast<uchar>(scale*src(i, j) + offset);
            }
        }
    }
    else if (ch == 3)
    {
        for (int j = 0; j < a.rows; j++)
        {
            for (int i = 0; i < a.cols; i++)
            {

                a.at<uchar>(j, 3 * i + 0) = cv::saturate_cast<uchar>(scale*src(i, j, 0) + +offset);
                a.at<uchar>(j, 3 * i + 1) = cv::saturate_cast<uchar>(scale*src(i, j, 1) + +offset);
                a.at<uchar>(j, 3 * i + 2) = cv::saturate_cast<uchar>(scale*src(i, j, 2) + +offset);
            }
        }
    }

    cv::imshow(name, a);
}

void guiAlphaBlend(Buffer<uint8_t>& src1, Buffer<uint8_t>& src2, cv::String name)
{
    cv::Mat s1(cv::Size(src1.width(), src1.height()), CV_MAKETYPE(CV_8U, src1.channels()));
    cv::Mat s2(cv::Size(src1.width(), src1.height()), CV_MAKETYPE(CV_8U, src1.channels()));
    convertHalide2Mat(src1, s1);
    convertHalide2Mat(src2, s2);

    cv::namedWindow(name);
    int a = 0; cv::createTrackbar("alpha", name, &a, 100);
    int key = 0;
    while (key != 'q')
    {
        cv::Mat show;
        cv::addWeighted(s1, 1.0 - a / 100.0, s2, a / 100.0, 0.0, show);
        cv::imshow(name, show);
        key = cv::waitKey(1);
    }
    cv::destroyWindow(name);
}