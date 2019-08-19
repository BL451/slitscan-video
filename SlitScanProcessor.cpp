#include "SlitScanProcessor.h"


SlitScanProcessor::SlitScanProcessor(int _direction, int _numSlices){
    setDirection(_direction);
    numSlices = _numSlices;
}


SlitScanProcessor::~SlitScanProcessor(){
    input.release();
    output.release();
}


int SlitScanProcessor::setDirection(int _direction){
    switch(_direction){
        case 0: direction = UP; break;
        case 1: direction = RIGHT; break;
        case 2: direction = DOWN; break;
        case 3: direction = LEFT; break;
    }
    return 1;
}


int SlitScanProcessor::setNumSlices(int _numSlices){
    numSlices = _numSlices;
    return 1;
}


int SlitScanProcessor::processFile(char* filename){
    /*
    Primary function for processing a video file.
    Takes the path to the video file as an argument.
    */
    input.open(filename);
    if (!input.isOpened()){
        return -1;
    }
    int frame_width = input.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = input.get(CAP_PROP_FRAME_HEIGHT);
    double fps = input.get(CAP_PROP_FPS);
    std::string output_name = "SLITSCAN_";
    output_name.append(std::to_string(direction));
    output_name.append("_");
    output_name.append(std::to_string(numSlices));
    output_name.append("_");
    output_name.append(filename);
    printf("%d, %d, %f\n", frame_width, frame_height, fps);
    output.open(output_name, VideoWriter::fourcc('m', 'p', '4', 'v'), fps, Size(frame_width, frame_height), true);
    initFrameVector();
    calculateSliceSize(frame_width, frame_height);
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


int SlitScanProcessor::calculateSliceSize(int width, int height){
    /*
    Simple calculation for the size in pixels of a given slice.
    Dependent on direction and width/height of a frame.
    */
    if (direction == UP || direction == DOWN){
        slicePixelSize = (int)ceil((float)height/(float)numSlices);
    }
    else if (direction == LEFT || direction == RIGHT){
        slicePixelSize = (int)ceil((float)width/(float)numSlices);
    }
    else{
        return -1;
    }
    printf("Slice Pixel Size: %d\n", slicePixelSize);
    return 0;
}


int SlitScanProcessor::initFrameVector(){
    /*
    Initialize the frame vector by reading in the number of frames from the
    source video required (numSlices) to make the first frame of our output video.
    */
    frame_list.clear();
    for (int i = 0; i < numSlices; i++){
        Mat frame;
        if (!input.read(frame)){
            printf("numSlices exceeds framecount!\n");
            return -1;
        }
        frame_list.push_back(frame);
    }
    return 0;
}


int SlitScanProcessor::incrementFrameVector(){
    /*
    "Increments" the frame vector forward by one frame by erasing the first
    element and pushing the next frame onto the end. 
    */
    Mat frame;
    if (!input.read(frame)){
        return -1;
    }
    frame_list.erase(frame_list.begin());
    frame_list.push_back(frame);
    return 0;
}


Mat SlitScanProcessor::generateOutFrame(int width, int height){
    /*
    Generates output frame based on the current frames in the frame list,
    taking direction and slice size into account. Note that row access
    is faster than column access, meaning that LEFT and RIGHT directions
    will have longer processing time.
    */
    Mat outframe = Mat::zeros(frame_list[0].size(), frame_list[0].type());
    for (int i = 0; i < frame_list.size(); i++){
        for (int j = 0; j < slicePixelSize; j++){
            try{
                if (direction == DOWN){
                    frame_list[i].row((i*slicePixelSize) + j).copyTo(outframe.row((i*slicePixelSize) + j));
                }
                else if (direction == UP){
                    frame_list[i].row(height - ((i*slicePixelSize) + j)).copyTo(outframe.row(height - ((i*slicePixelSize) + j)));
                }
                else if (direction == RIGHT){
                    frame_list[i].col((i*slicePixelSize) + j).copyTo(outframe.col((i*slicePixelSize) + j));
                }
                else if (direction == LEFT){
                    frame_list[i].col(width - ((i*slicePixelSize) + j)).copyTo(outframe.col(width - ((i*slicePixelSize) + j)));
                }
            }
            catch (Exception){
                break;
            }
        }
    }
    return outframe;
}

/*
int main(int argc, char** argv) 
{
    SlitScanProcessor Processor(UP, 54);
    int success = Processor.processFile(argv[1]);
    return 0; 
}
*/