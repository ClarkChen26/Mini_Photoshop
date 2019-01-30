#include <filter.h>
#include <source.h>
#include <sink.h>
#include <logging.h>

void
Filter::SetInput(const Image *input1) {
    updated = false;
    Sink::SetInput(input1);
}

void
Filter::SetInput2(const Image *input2) {
    updated = false;
    Sink::SetInput2(input2);
}

void
Filter::Update()
{
    if (input1 == NULL) {
        char msg[1024];
        sprintf(msg, "%s: no input1!\n", SinkName());
        DataFlowException e(SinkName(), msg);
        throw e;
    }

    if (input2 == NULL && GetNumberofInputs() == 2) {
        char msg[1024];
        sprintf(msg, "%s: no input2!\n", SinkName());
        DataFlowException e(SinkName(), msg);
        throw e;
    }

    if (updated) {
        return;
    }
    updated = true;

    if (input1 != NULL)
    {   
        char msg[128];
        sprintf(msg, "%s: about to update input1\n", SourceName());
        Logger::LogEvent(msg);
        try {
            input1 -> Update();
        } catch (DataFlowException &exception) {
            updated = false;
            throw exception;
        }
        sprintf(msg, "%s: done updating input1\n", SourceName());
        Logger::LogEvent(msg);
    }
    
    if (GetNumberofInputs() == 2)
    {
        if (input2 != NULL)
        {
            char msg[128];
            sprintf(msg, "%s: about to update input2\n", SourceName());
            Logger::LogEvent(msg);
            try {
                input2 -> Update();
            } catch (DataFlowException &exception) {
                updated = false;
                throw exception;
            }
            sprintf(msg, "%s: done updating input2\n", SourceName());
            Logger::LogEvent(msg);
        }
    }
    char msg[1024];
    sprintf(msg, "%s: about to execute\n", SourceName());
    Logger::LogEvent(msg);
    Execute();
    sprintf(msg, "%s: done executing\n", SourceName());
    Logger::LogEvent(msg);
}

void
Shrinker::Execute()
{   
    Image *output = GetOutput();

    output -> ResetSize(input1->GetWidth()/2, input1->GetHeight()/2);
    for(int i = 0; i < output->GetWidth(); i++){ 
        for(int j = 0; j < output->GetHeight(); j++){
            image -> GetPixel()[j * output->GetWidth() + i] = input1->GetPixel()[(2 * j) * input1->GetWidth() + (2 * i)];
        }    
    }    
}

void
LRCombine::Execute()
{   
    Image *output = GetOutput();

    if (input1->GetHeight() != input2->GetHeight())
    {
        char msg[1024];
        sprintf(msg, "%s: heights must match: %d, %d\n", SinkName(), input1->GetHeight(),
                input2->GetHeight());
        DataFlowException e(SinkName(), msg);
        throw e;
    }
    else
    {
        output -> ResetSize(input1->GetWidth() + input2->GetWidth(), input1->GetHeight());
        for(int i = 0; i < input1->GetHeight(); i++){
            for(int j = 0; j < input1->GetWidth(); j++){
                image -> GetPixel()[i * (input1->GetWidth() + input2->GetWidth()) + j] = input1 ->GetPixel()[i * input1->GetWidth() + j];
            }
            for (int k = input1->GetWidth(); k < (input1->GetWidth() + input2->GetWidth()); k++){
                image -> GetPixel()[i * (input1->GetWidth() + input2->GetWidth()) + k] = input2->GetPixel()[i * input2->GetWidth() + k - input1->GetWidth()];
            }    
        }
    }
}

void
TBCombine::Execute()
{
    Image *output = GetOutput();

    if (input1->GetWidth() != input2->GetWidth())
    {
        char msg[1024];
        sprintf(msg, "%s: heights must match: %d, %d\n", SinkName(), input1->GetWidth(),
                input2->GetWidth());
        DataFlowException e(SinkName(), msg);
        throw e;
    }
    else
    {
        output -> ResetSize(input1->GetWidth(), input1->GetHeight() + input2->GetHeight());
        for(int i = 0; i < input1->GetWidth(); i++){
            for(int j = 0; j < input1->GetHeight(); j++){
                image -> GetPixel()[j * input1->GetWidth() + i] = input1->GetPixel()[j * input1->GetWidth() + i];
            }
            for (int k = 0; k < input2->GetHeight(); k++){
                image -> GetPixel()[(k+input1->GetHeight()) * input2->GetWidth() + i] = input2->GetPixel()[k * input2->GetWidth() + i];
            }    
        }
    }
}


void
Blender::Execute()
{   
    Image *output = GetOutput();

    if (input1->GetWidth() != input2->GetWidth() || input1->GetHeight() != input2->GetHeight())
    {
        char msg[1024];
        sprintf(msg, "%s: width and height must match: %d, %d\n", SinkName(), input1->GetWidth(),
                input2->GetWidth());
        DataFlowException e(SinkName(), msg);
        throw e;
    }

    if (factor > 1 || factor < 0){
        char msg[1024];
        sprintf(msg, "Invalid factor for Blender: %f\n", factor);
        DataFlowException e(SinkName(), msg); 
        throw e;
    }
    
    output -> ResetSize(input1->GetWidth(), input1->GetHeight());
    for(int i = 0; i < input1->GetWidth(); i++){
        for(int j = 0; j < input1->GetHeight(); j++){
            image -> GetPixel()[j * input1->GetWidth() + i].r = input1 -> GetPixel()[j * input1->GetWidth() + i].r * factor + input2->GetPixel()[j * input1->GetWidth() + i].r * (1 - factor);
            image -> GetPixel()[j * input1->GetWidth() + i].g = input1 -> GetPixel()[j * input1->GetWidth() + i].g * factor + input2->GetPixel()[j * input1->GetWidth() + i].g * (1 - factor);
            image -> GetPixel()[j * input1->GetWidth() + i].b = input1 -> GetPixel()[j * input1->GetWidth() + i].b * factor + input2->GetPixel()[j * input1->GetWidth() + i].b * (1 - factor);
        }
    }
}

void
CheckSum::OutputCheckSum(const char *filename)
{
    FILE *f = fopen(filename, "w");
    unsigned char red = 0;
    unsigned char green = 0;
    unsigned char blue = 0;
    for (int i = 0; i < input1->GetWidth(); i++){
        for (int j = 0; j < input1->GetHeight(); j++){
            red += input1->GetPixel()[j * input1->GetWidth() + i].r;
            green += input1->GetPixel()[j * input1->GetWidth() + i].g;
            blue += input1->GetPixel()[j * input1->GetWidth() + i].b;
        }
    }        
    fprintf(f, "CHECKSUM: %d, %d, %d\n", red, green, blue);
    fclose(f);
}

Color::Color(int width, int height, unsigned char r, unsigned char g, unsigned char b)
{
    Image *output = GetOutput();
    output -> ResetSize(width, height);
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            image -> GetPixel()[j * width + i].r = r;
            image -> GetPixel()[j * width + i].g = g;
            image -> GetPixel()[j * width + i].b = b;
        }
    }
}

void
Color::Execute() {
    return;
}

void
Color::Update() {
    Execute();
}

void
Mirror::Execute()
{
    Image *output = GetOutput();

    output -> ResetSize(input1->GetWidth(), input1->GetHeight());
    for (int i = 0; i < input1->GetWidth(); i++){
        for (int j = 0; j < input1->GetHeight(); j++){
            image -> GetPixel()[j * output->GetWidth() + i] = input1->GetPixel()[j * input1->GetWidth() + (input1->GetWidth() - i - 1)];
        }
    }
}

void
Rotate::Execute()
{
    Image *output = GetOutput();

    output -> ResetSize(input1->GetHeight(), input1->GetWidth());
    for (int i = 0; i < output->GetWidth(); i++){
        for (int j = 0; j < output->GetHeight(); j++){
            image -> GetPixel()[j * output->GetWidth() + i] = input1->GetPixel()[(input1->GetHeight() - i - 1) * input1->GetWidth() + j];
        }
    }
}

void
Subtract::Execute()
{
    Image *output = GetOutput();

    if (input1->GetWidth() != input2->GetWidth() || input1->GetHeight() != input2->GetHeight())
    {
        char msg[1024];
        sprintf(msg, "%s: width and height must match: %d, %d\n", SinkName(), input1->GetWidth(),
                input2->GetWidth());
        DataFlowException e(SinkName(), msg);
        throw e;
    }
    else
    {
        output -> ResetSize(input1->GetWidth(), input1->GetHeight());
        for (int i = 0; i < input1->GetWidth(); i++){
            for (int j = 0; j < input1->GetHeight(); j++){
                int in1_index = j * input1->GetWidth() + i;
                int in2_index = j * input2->GetWidth() + i;
                int out_index = j * input1->GetWidth() + i;
                if(input1->GetPixel()[in1_index].r > input2 -> GetPixel()[in2_index].r){
                    image->GetPixel()[out_index].r = input1->GetPixel()[in1_index].r - input2 -> GetPixel()[in2_index].r;
                }
                else
                {
                    image->GetPixel()[out_index].r = 0;
                }
                if(input1->GetPixel()[in1_index].g > input2 -> GetPixel()[in2_index].g){
                    image->GetPixel()[out_index].g = input1->GetPixel()[in1_index].g - input2 -> GetPixel()[in2_index].g;
                }
                else
                {
                    image->GetPixel()[out_index].g = 0;
                }
                if(input1->GetPixel()[in1_index].b > input2 -> GetPixel()[in2_index].b){
                    image->GetPixel()[out_index].b = input1->GetPixel()[in1_index].b - input2 -> GetPixel()[in2_index].b;
                }
                else
                {
                    image->GetPixel()[out_index].b = 0;
                }
            }
        }
    
    }
}

void
Grayscale::Execute()
{
    Image *output = GetOutput();

    output -> ResetSize(input1->GetWidth(), input1->GetHeight());
    for (int i = 0; i < input1->GetWidth(); i++){
        for (int j = 0; j < input1->GetHeight(); j++){
            int in1_index = j * input1->GetWidth() + i;
            int out_index = j * input1->GetWidth() + i;
            image->GetPixel()[out_index].r = input1->GetPixel()[in1_index].r / 5 + 
                                             input1->GetPixel()[in1_index].g / 2 + 
                                             input1->GetPixel()[in1_index].b / 4;
            image->GetPixel()[out_index].g = image->GetPixel()[out_index].r;
            image->GetPixel()[out_index].b = image->GetPixel()[out_index].r;
        }
    }
}

void
Blur::Execute()
{
    Image *output = GetOutput();

    output -> ResetSize(input1->GetWidth(), input1->GetHeight());
    int w = input1->GetWidth();
    int h = input1->GetHeight();
    for (int i = 0; i < input1->GetWidth(); i++){
        for (int j = 0; j < input1->GetHeight(); j++){
            if (i == 0 || j == 0 || i == w - 1 || j == h - 1){
                image->GetPixel()[j * input1->GetWidth() + i] = input1->GetPixel()[j * input1->GetWidth() + i];
            }
            else{
                image->GetPixel()[j * input1->GetWidth() + i].r = input1->GetPixel()[(j-1) * input1->GetWidth() + (i-1)].r / 8 +
                                                              input1->GetPixel()[(j-1) * input1->GetWidth() + (i)  ].r / 8 +
                                                              input1->GetPixel()[(j-1) * input1->GetWidth() + (i+1)].r / 8 +
                                                              input1->GetPixel()[(j)   * input1->GetWidth() + (i-1)].r / 8 +
                                                              input1->GetPixel()[(j)   * input1->GetWidth() + (i+1)].r / 8 +
                                                              input1->GetPixel()[(j+1) * input1->GetWidth() + (i-1)].r / 8 +
                                                              input1->GetPixel()[(j+1) * input1->GetWidth() + (i)  ].r / 8 +
                                                              input1->GetPixel()[(j+1) * input1->GetWidth() + (i+1)].r / 8;
                image->GetPixel()[j * input1->GetWidth() + i].g = input1->GetPixel()[(j-1) * input1->GetWidth() + (i-1)].g / 8 +
                                                              input1->GetPixel()[(j-1) * input1->GetWidth() + (i)  ].g / 8 +
                                                              input1->GetPixel()[(j-1) * input1->GetWidth() + (i+1)].g / 8 +
                                                              input1->GetPixel()[(j)   * input1->GetWidth() + (i-1)].g / 8 +
                                                              input1->GetPixel()[(j)   * input1->GetWidth() + (i+1)].g / 8 +
                                                              input1->GetPixel()[(j+1) * input1->GetWidth() + (i-1)].g / 8 +
                                                              input1->GetPixel()[(j+1) * input1->GetWidth() + (i)  ].g / 8 +
                                                              input1->GetPixel()[(j+1) * input1->GetWidth() + (i+1)].g / 8;
                image->GetPixel()[j * input1->GetWidth() + i].b = input1->GetPixel()[(j-1) * input1->GetWidth() + (i-1)].b / 8 +
                                                              input1->GetPixel()[(j-1) * input1->GetWidth() + (i)  ].b / 8 +
                                                              input1->GetPixel()[(j-1) * input1->GetWidth() + (i+1)].b / 8 +
                                                              input1->GetPixel()[(j)   * input1->GetWidth() + (i-1)].b / 8 +
                                                              input1->GetPixel()[(j)   * input1->GetWidth() + (i+1)].b / 8 +
                                                              input1->GetPixel()[(j+1) * input1->GetWidth() + (i-1)].b / 8 +
                                                              input1->GetPixel()[(j+1) * input1->GetWidth() + (i)  ].b / 8 +
                                                              input1->GetPixel()[(j+1) * input1->GetWidth() + (i+1)].b/ 8;                                
            } 
        }
    }
}
