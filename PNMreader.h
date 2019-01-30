#include <image.h>
#include <source.h>

#ifndef PNM_H
#define PNM_H
class PNMreader: public Source
{
public:
    PNMreader(char *f);
    ~PNMreader();
    void Execute();
    void Update();
    const char *SourceName() { return "PNMreader"; };
private:
    char *filename;
};
#endif