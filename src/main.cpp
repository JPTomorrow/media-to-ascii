#include <iostream>
#include <ascii_coversion.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

std::vector<ascii::AsciiFrame> output_frames;

std::string get_asset_full_path(const std::string& filename) {
    std::string path = "./assets/";
    return path + filename;
}

int main(int argc, char** argv) {
    std::cout << "arg count: " << argc << std::endl;
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i] << std::endl;
    }

    std::string filepath = get_asset_full_path("test_01.webp");

    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);
    cv::Mat img = cv::imread(filepath, cv::IMREAD_COLOR);


    if (img.empty()) {
        std::cerr << "Error: Could not open or find the image." << std::endl;
        return -1;
    }

    cv::Mat modified_img;
    cv::Size dims(3 * 50, 4 * 50);
    cv::resize(img, modified_img, dims, 0, 0);
    cv::cvtColor(modified_img, modified_img, cv::COLOR_BGR2GRAY);

    // setup frame
    ascii::AsciiFrame frame;
    frame.line_offset = dims.width;

    // possibly more efficient pixel access
    cv::MatIterator_<uchar> it, end;

    for (it = modified_img.begin<uchar>(), end = modified_img.end<uchar>(); it != end; ++it) {
        // Access pixel value: (*it)[0] (Blue), (*it)[1] (Green), (*it)[2] (Red)
        // For grayscale: *it
        const uchar pixel_value = *it;
        const float brightness = static_cast<float>(pixel_value) / 255; // normalize to 0f - 1f
        char ascii_char = ascii::cvt_pixel_to_ascii(brightness);
        frame.data.push_back(ascii_char);
    }

    ascii::console_draw_frame(frame);
    // cv::imshow("test window", modified_img);
    system("pause");
    // cv::waitKey(0);
}