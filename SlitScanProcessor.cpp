#include "SlitScanProcessor.h"


SlitScanProcessor::SlitScanProcessor(Direction _direction, int _numDivisions){
    direction = _direction;
    numDivisions = _numDivisions;
}


SlitScanProcessor::~SlitScanProcessor(){
    input.release();
    output.release();
}


int SlitScanProcessor::setDirection(Direction _direction){
    direction = _direction;
    return 1;
}


int SlitScanProcessor::setNumDivisions(int _numDivisions){
    numDivisions = _numDivisions;
    return 1;
}


int SlitScanProcessor::processFile(char* filename){
    input.open(filename);
    if (!input.isOpened()){
        return -1;
    }
    int frame_width = input.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = input.get(CAP_PROP_FRAME_HEIGHT);
    double fps = input.get(CAP_PROP_FPS);
    std::string output_name = "SLITSCANLEFT_";
    output_name.append(filename);
    printf("%d, %d, %f\n", frame_width, frame_height, fps);
    output.open(output_name, VideoWriter::fourcc('m', 'p', '4', 'v'), fps, Size(frame_width, frame_height), true);
    initFrameVector();
    calculateDivisionSize(frame_width, frame_height);
    int k = 0;
    for (;;){
        printf("\rWriting frame %d", ++k);
        fflush(stdout);
        output.write(generateOutFrame(frame_width, frame_height));
        if (incrementFrameVector() == -1){
            break;
        }
    }
    return 0;
}


int SlitScanProcessor::calculateDivisionSize(int width, int height){
    if (direction == UP || direction == DOWN){
        divisionPixelSize = (int)ceil((float)height/(float)numDivisions);
    }
    else if (direction == LEFT || direction == RIGHT){
        divisionPixelSize = (int)ceil((float)width/(float)numDivisions);
    }
    else{
        return -1;
    }
    printf("Division Pixel Size: %d\n", divisionPixelSize);
    return 0;
}


int SlitScanProcessor::initFrameVector(){
    frame_list.clear();
    for (int i = 0; i < numDivisions; i++){
        Mat frame;
        if (!input.read(frame)){
            printf("numDivisions exceeds framecount!\n");
            return -1;
        }
        frame_list.push_back(frame);
    }
    return 0;
}


int SlitScanProcessor::incrementFrameVector(){
    Mat frame;
    if (!input.read(frame)){
        return -1;
    }
    frame_list.erase(frame_list.begin());
    frame_list.push_back(frame);
    return 0;
}


Mat SlitScanProcessor::generateOutFrame(int width, int height){
    Mat outframe = Mat::zeros(frame_list[0].size(), frame_list[0].type());
    for (int i = 0; i < frame_list.size(); i++){
        for (int j = 0; j < divisionPixelSize; j++){
            try{
                if (direction == DOWN){
                    frame_list[i].row((i*divisionPixelSize) + j).copyTo(outframe.row((i*divisionPixelSize) + j));
                }
                else if (direction == UP){
                    frame_list[i].row(height - ((i*divisionPixelSize) + j)).copyTo(outframe.row(height - ((i*divisionPixelSize) + j)));
                }
                else if (direction == RIGHT){
                    frame_list[i].col((i*divisionPixelSize) + j).copyTo(outframe.col((i*divisionPixelSize) + j));
                }
                else if (direction == LEFT){
                    frame_list[i].col(width - ((i*divisionPixelSize) + j)).copyTo(outframe.col(width - ((i*divisionPixelSize) + j)));
                }
            }
            catch (Exception){
                break;
            }
        }
    }
    return outframe;
}


int main(int argc, char** argv) 
{
    SlitScanProcessor Processor(LEFT, 108);
    int success = Processor.processFile(argv[1]);
    return 0; 
}