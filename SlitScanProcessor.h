#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
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

        Direction direction;
        float timeDisplacement;
        int numDivisions;
};