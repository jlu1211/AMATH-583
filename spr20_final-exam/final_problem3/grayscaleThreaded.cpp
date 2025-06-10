#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <cstring>
#include <png.h>
#include <cstdint>

//threaded solution - dummy code
void grayscaleThreaded(png_bytep* image, int width, int height, int channels, int numThreads) {
    if (numThreads > height) {
        numThreads = height;
    }

    int baseRows = height / numThreads;
    int extraRows = height % numThreads;
    int rowStart = 0;

    std::vector<std::thread> threads;
    threads.reserve(numThreads);

    for (int i = 0; i < numThreads; ++i) {
        int rows = baseRows + (i < extraRows ? 1 : 0);
        int startRow = rowStart;
        int endRow = startRow + rows;
        rowStart += rows;

        threads.emplace_back([image, width, height, channels, startRow, endRow]() {
            for (int y = startRow; y < endRow; ++y) {
                png_bytep row = image[y];
                for (int x = 0; x < width; ++x) {
                    png_bytep px = &(row[x * channels]);
                    uint8_t gray = static_cast<uint8_t>(0.3 * px[0] + 0.59 * px[1] + 0.11 * px[2]);
                    px[0] = px[1] = px[2] = gray;
                }
            }
        });
        rowStart = endRow;
    }
    for (auto& th : threads) th.join();
}
