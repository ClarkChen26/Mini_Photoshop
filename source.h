#include <image.h>

#ifndef SOURCE_H
#define SOURCE_H
class Source
{
protected:
    Image* image;
    virtual void Execute(void) = 0;
public:
    Source(void);
    ~Source(void);
    virtual void Update(void) = 0;
    Image* GetOutput();
};
#endif