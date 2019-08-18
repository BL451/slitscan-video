#include "SlitScanProcessor.h"


SlitScanProcessor::SlitScanProcessor(Direction _direction, float _timeDisplacement, int _numDivisions){
    direction = _direction;
    timeDisplacement = _timeDisplacement;
    numDivisions = _numDivisions;
}


SlitScanProcessor::~SlitScanProcessor(){
    printf("SEE YA\n");
}


int SlitScanProcessor::setDirection(Direction _direction){
    direction = _direction;
    return 1;
}


int SlitScanProcessor::setTimeDisplacement(float _timeDisplacement){
    timeDisplacement = _timeDisplacement;
    return 1;
}


int SlitScanProcessor::setNumDivisions(int _numDivisions){
    numDivisions = _numDivisions;
    return 1;
}


int SlitScanProcessor::processFile(char* filename){
    VideoCapture input(filename);
    if (!input.isOpened()){
        return -1;
    }
    int frame_width = input.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = input.get(CAP_PROP_FRAME_HEIGHT);
    double fps = input.get(CAP_PROP_FPS);
    std::string output_name = "SLITSCAN_";
    output_name.append(filename);
    printf("%d, %d, %f\n", frame_width, frame_height, fps);
    VideoWriter output(output_name, VideoWriter::fourcc('m', 'p', '4', 'v'), fps, Size(frame_width, frame_height), true);
    Mat frame;
    int i = 0;
    for (;;){
        if (!input.read(frame)){
            break;
        }
        printf("\rWriting frame %d", ++i);
        fflush(stdout);
        output.write(frame);
    }
    input.release();
    output.release();
    return 0;
}


int main(int argc, char** argv) 
{
    SlitScanProcessor Processor(UP, 0.5, 100);
    int success = Processor.processFile(argv[1]);
    return 0; 
}