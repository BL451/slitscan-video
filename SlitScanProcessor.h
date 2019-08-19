#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <math.h>
using namespace cv;

typedef enum Direction{
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3,
} Direction;

class SlitScanProcessor{
    public:
        SlitScanProcessor(int _direction, int _numSlices);
        int setDirection(int _direction);
        int setNumSlices(int _numSlices);
        int processFile(char* filename);
    virtual ~SlitScanProcessor();
    private:
        int calculateSliceSize(int width, int height);
        int initFrameVector();
        int incrementFrameVector();
        Mat generateOutFrame(int width, int height);
        VideoCapture input;
        VideoWriter output;
        Direction direction;
        int numSlices;
        std::vector<Mat> frame_list;
        int slicePixelSize;
};