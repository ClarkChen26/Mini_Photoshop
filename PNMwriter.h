#include <image.h>
#include <sink.h>

#ifndef PNMW_H
#define PNMW_H
class PNMwriter: public Sink
{
public:
    void Write(char *filename);
};
#endif