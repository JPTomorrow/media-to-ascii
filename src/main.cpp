#include <iostream>
#include <ascii_coversion.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <Windows.h>

std::string get_asset_full_path(const std::string& filename) {
    std::string path = "./assets/";
    return path + filename;
}

bool is_image_format(const std::string& filepath) {
    std::array<std::string, 5> formats = { ".png", ".jpg", ".bmp", ".webp", ".jpeg" };
    for (const auto& format : formats) {
        if (filepath.ends_with(format)) return true;
    }
    return false;
}

bool is_video_format(const std::string& filepath) {
    std::array<std::string, 5> formats = { ".mp4" };
    for (const auto& format : formats) {
        if (filepath.ends_with(format)) return true;
    }
    return false;
}

void clear_console_lines(const int num_lines) {
    for (int i = 0; i < num_lines; ++i) {
        std::cout << "\x1b[1A"; // Move cursor up one line
        std::cout << "\x1b[2K"; // Clear entire line
    }
}

void process_image(const cv::Mat &img, int console_width) {

    cv::Mat modified_img;
    cv::Size dims(console_width, (static_cast<float>(img.rows) / static_cast<float>(img.cols)) * static_cast<float>(console_width));
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
}

void process_video(cv::VideoCapture &vid, int console_width) {
    std::vector<ascii::AsciiFrame> output_frames;

    cv::Mat img;
    while (true) {
        vid >> img; // Read the next frame into the 'frame' Mat

        if (img.empty()) { // Check if the frame is empty (end of video or error)
            std::cout << "End of video or frame read error." << std::endl;
            break;
        }

        cv::Mat modified_img;
        cv::Size dims(console_width, (static_cast<float>(img.rows) / static_cast<float>(img.cols)) * static_cast<float>(console_width));
        cv::resize(img, modified_img, dims, 0, 0);
        cv::cvtColor(modified_img, modified_img, cv::COLOR_BGR2GRAY);

        // setup frame
        ascii::AsciiFrame frame;
        frame.line_offset = dims.width;

        // possibly more efficient pixel access
        cv::MatIterator_<uchar> it, end;
        for (it = modified_img.begin<uchar>(), end = modified_img.end<uchar>(); it != end; ++it) {
            const uchar pixel_value = *it;
            const float brightness = static_cast<float>(pixel_value) / 255; // normalize to 0f - 1f
            char ascii_char = ascii::cvt_pixel_to_ascii(brightness);
            frame.data.push_back(ascii_char);
        }

        output_frames.push_back(frame);
    }

    for (const auto& frame : output_frames) {
        int clear_line_cnt = frame.data.size() / frame.line_offset;
        // std::cout << clear_line_cnt << "\n";
        clear_console_lines(clear_line_cnt);
        ascii::console_draw_frame(frame);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

int main(int argc, char** argv) {
    // const std::string filepath = get_asset_full_path("test_01.webp");
    const std::string filepath = get_asset_full_path("test_vid.mp4");

    // setup silent logging and console font
    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

    int console_width = 0;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    console_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    consoleWidth = w.ws_col;
#endif

    if (is_image_format(filepath)) {
        cv::Mat img = cv::imread(filepath, cv::IMREAD_COLOR);

        if (img.empty()) {
            std::cerr << "Error: Could not open or find the image." << std::endl;
            return -1;
        }
        process_image(img, console_width);
    }
    else if (is_video_format(filepath)) {
        cv::VideoCapture cap(filepath);

        if (!cap.isOpened()) {
            std::cerr << "Error: Could not open video file." << std::endl;
            return -1;

        }

        process_video(cap, console_width);
    }
    else {
        std::cerr << "Error: Could not open the file. check the format." << std::endl;
        return -1;
    }
    // cv::imshow("test window", modified_img);
    // cv::waitKey(0);
    system("pause");
    return 0;
}