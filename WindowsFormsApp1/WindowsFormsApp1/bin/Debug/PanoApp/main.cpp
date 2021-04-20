#include <Snitcher.hpp>

int main() {
    sv::snitcher::PanoSnitcher check(512,254);
    const cv::Mat add = sv::snitcher::Read(R"(C:\Users\Oladushek\Desktop\image2.jpg)");
    check.putPhotoInPano(add,1);
    sv::snitcher::Write(R"(C:\Users\Oladushek\Desktop\image1.jpg)",check.getMatrix());
    return 0;
}
