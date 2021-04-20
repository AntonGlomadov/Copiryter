//
// Created by Oladushek on 3/17/2021.
//

#include <Exception.hpp>
#include "Snitcher.hpp"

namespace sv::snitcher {

    PanoSnitcher::PanoSnitcher(int heigt, int weight) {
        m_height = heigt;
        m_weight = weight;
        dst = cv::Mat(heigt, weight, 16);
        readSettings();
    }

    void PanoSnitcher::rotateImage(const cv::_OutputArray &src, const int i) {
    }

    void PanoSnitcher::readSettings() {
        Json js;
        std::ifstream jsonSettingCam(R"(D:\panostitcher\Untitled-1.json)");
        if (!jsonSettingCam.good()) {
            throw sv::Exception("Json file for settings can not be read", __FILE__, __LINE__);
        }
        jsonSettingCam >> js;

        if (js.size() > sizeof(parsed)) {
            throw sv::Exception("Json file too large", __FILE__, __LINE__);
        }
        size_t index = 0;
        for (auto &item : js) {
            parsed[index].roll = js[index]["roll"];
            parsed[index].pitch = js[index]["pitch"];
            parsed[index].yaw = js[index]["yaw"];
            parsed[index].a11 = js[index]["a11"];
            parsed[index].a12 = js[index]["a12"];

            parsed[index].a13 = js[index]["a13"];

            parsed[index].a14 = js[index]["a14"];

            parsed[index].a21 = js[index]["a21"];
            parsed[index].a22 = js[index]["a22"];
            parsed[index].a23 = js[index]["a23"];
            parsed[index].a24 = js[index]["a24"];
            parsed[index].a31 = js[index]["a31"];
            parsed[index].a32 = js[index]["a32"];
            parsed[index].a33 = js[index]["a33"];
            parsed[index].a34 = js[index]["a34"];
            parsed[index].k0 = js[index]["k0"];
            parsed[index].k1 = js[index]["k1"];
            parsed[index].k2 = js[index]["k2"];
            parsed[index].p1 = js[index]["p1"];
            parsed[index].p2 = js[index]["p2"];
            parsed[index].Fu = js[index]["Fu"];
            parsed[index].Fv = js[index]["Fv"];
            parsed[index].Ku0 = js[index]["Ku0"];
            parsed[index].Kv0 = js[index]["Kv0"];
            ++index;
        }
    }

    void PanoSnitcher::putPhotoInPano(const cv::Mat src, int i) {
        cv::Point_<double> pixel;
        double theta, phi, ro = 11;

        cv::Point3d psph;
        double tmpX;
        double tmpY;
        double tmpZ;

        auto width = dst.size().width;
        auto height = dst.size().height;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {

                theta = 2.0 * CV_PI * (x - double(width) / 2) / width; // -pi to pi
                phi = CV_PI * (y - double(height) / 2) / height;    // -pi/2 to pi/2]

                //проверка на выход изображения за границы своего расположения
                if (i >= 6) {
                    if (phi > 0) {
                        continue;
                    }
                } else {
                    if (check(theta, parsed[i].pitch)) {
                        continue;
                    }
                }

                //           переход от к сферических к трехмерным

                psph.x = ro * cos(phi) * sin(theta);
                psph.y = ro * sin(phi);
                psph.z = ro * cos(phi) * cos(theta);


                //поворот перв

                tmpX = psph.x;
                tmpY = psph.y;
                tmpZ = psph.z;

                psph.x = tmpX * parsed[i].a11 + tmpY * parsed[i].a12 + tmpZ * parsed[i].a13 + parsed[i].a14;
                psph.y = tmpX * parsed[i].a21 + tmpY * parsed[i].a22 + tmpZ * parsed[i].a23 - parsed[i].a24;
                psph.z = tmpX * parsed[i].a31 + tmpY * parsed[i].a32 + tmpZ * parsed[i].a33 + parsed[i].a34;

                //переход из пространства в плоскость
                if (psph.z == 0) {
                    pixel.x = 0;
                    pixel.y = 0;
                } else {
                    pixel.x = psph.x / psph.z;
                    pixel.y = psph.y / psph.z;
                }

                //убираем дисторсию
                auto r = pixel.x * pixel.x + pixel.y * pixel.y;
                tmpX = pixel.x;
                tmpY = pixel.y;
                pixel.x = tmpX * (1 + parsed[i].k0 * pow(r, 2) + parsed[i].k1 * pow(r, 4) + parsed[i].k2 * pow(r, 6)) +
                          2 * parsed[i].p1 * tmpY * tmpX +
                          parsed[i].p2 * (pow(r, 2) + 2 * tmpX * tmpX);
                pixel.y = tmpY * (1 + parsed[i].k0 * pow(r, 2) + parsed[i].k1 * pow(r, 4) + parsed[i].k2 * pow(r, 6)) +
                          parsed[i].p1 * (r * r - 2 * tmpY * tmpY) + 2 * parsed[i].p2 * tmpY * tmpX;

                //переходим в положение камеры
                pixel.x = float(pixel.x * parsed[i].Fu + parsed[i].Ku0);
                pixel.y = float(pixel.y * parsed[i].Fv + parsed[i].Kv0);


                if (((pixel.y > 0) && (pixel.y < src.size().height - 1)) &&
                    ((pixel.x > 90) && (pixel.x < src.size().width - 90))) {
                    dst.at<cv::Vec3b>(cv::Point(x, y)) = src.at<cv::Vec3b>(pixel);

                }
            }
        }
    }
    bool PanoSnitcher::check(double theta, double betta) {
        double a = -CV_PI / 4 + betta;
        if (a > -CV_PI) {
            if (a < theta && theta < a + CV_PI / 2)
                return false;
        } else if (a <= -CV_PI) {
            if (a <= theta && theta <= a + CV_PI / 2)
                return false;
            if (a + 2 * CV_PI < theta && theta <= a + 2 * CV_PI + CV_PI / 2)
                return false;
        }
        return true;
    }

    cv::Mat PanoSnitcher::getMatrix() {
        return dst;
    }



    void Write(const std::filesystem::path &whereWrite, const cv::Mat &dst) {
        cv::imwrite(whereWrite.string(), dst);
    }

    cv::Mat Read(const std::filesystem::path &whatRead) {
        cv::Mat src = cv::imread(whatRead.string());
        if (src.empty()) {
            throw sv::Exception("File not readed", __FILE__, __LINE__);
        }
        return src;

    }
}