#include <image.h>

#ifndef SINK_H
#define SINK_H
class Sink
{   
public:
    Sink(void);
    void SetInput(const Image *input1);
    void SetInput2(const Image *input2);
protected:
    const Image* input1;
    const Image* input2;      
};
#endif