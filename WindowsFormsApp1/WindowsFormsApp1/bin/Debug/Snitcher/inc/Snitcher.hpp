//
// Created by Oladushek on 3/17/2021.
//
#pragma once
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <JsonPipe.hpp>
#include <fstream>
//инфа с камер

namespace sv::snitcher{
     void Write(const std::filesystem::path& whereWrite, const cv::Mat &dst);
     cv::Mat Read(const std::filesystem::path& whatRead);
    class PanoSnitcher{
    public:
        PanoSnitcher(int heigt, int weight);
        void rotateImage(const cv::_OutputArray &src, int i);
        void putPhotoInPano(cv::Mat src, int i);
        cv::Mat getMatrix();


    private:
        void readSettings();
        bool check(double theta, double betta);
    private:
        struct camerInfo {
            double roll{0};
            double pitch{0};
            double yaw{0};
            double a11{0};
            double a12{0};
            double a13{0};
            double a14{0};
            double a21{0};
            double a22{0};
            double a23{0};
            double a24{0};
            double a31{0};
            double a32{0};
            double a33{0};
            double a34{0};
            double k0{0};
            double k1{0};
            double k2{0};
            double p1{0};
            double p2{0};
            double Fu{0};
            double Fv{0};
            double Ku0{0};
            double Kv0{0};
        };
        camerInfo parsed[6];
        cv::Mat dst;
        int m_height{0};
        int m_weight{0};
    };
}
