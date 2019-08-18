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
        SlitScanProcessor(Direction _direction, float _timeDisplacement, int _numDivisions);
        int setDirection(Direction _direction);
        int setTimeDisplacement(float _timeDisplacement);
        int setNumDivisions(int _numDivisions);
        int processFile(char* filename);
    virtual ~SlitScanProcessor();
    private:
        int calculateDivisionSize(int width, int height);
        int initFrameVector();
        int incrementFrameVector();
        Mat generateOutFrame(int width, int height);
        VideoCapture input;
        VideoWriter output;
        Direction direction;
        float timeDisplacement;
        int numDivisions;
        std::vector<Mat> frame_list;
        int divisionPixelSize;
};