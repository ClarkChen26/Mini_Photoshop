#include <stdio.h>
#include <source.h>
#include <sink.h>


#ifndef FILTER_H
#define FILTER_H
class Filter: public Source, public Sink {
public:
    void Update();
    void SetInput(const Image *input1);
    void SetInput2(const Image *input2);
    virtual int GetNumberofInputs() {return 0;}
    virtual const char *FilterName() = 0;
    virtual const char *SourceName() { return FilterName(); };
    virtual const char *SinkName() { return FilterName(); };
    Filter():Source(), Sink(){
        updated = false;
    };
private:
    bool updated;
};
#endif

#ifndef SIF_H
#define SIF_H
class SingleInputFiler: public Filter
{
public:
    virtual int GetNumberofInputs() {return 1;}
};
#endif 

#ifndef DIF_H
#define DIF_H
class DoubleInputFiler: public Filter
{
public:
    virtual int GetNumberofInputs() {return 2;}
};
#endif

#ifndef SHRINK_H
#define SHRINK_H
class Shrinker: public SingleInputFiler
{
public:
    void Execute();
    const char *FilterName() { return "Shrinker"; };
};
#endif

#ifndef LRCONCAT_H
#define LRCONCAT_H
class LRCombine: public DoubleInputFiler
{
public:
    void Execute();
    const char *FilterName() { return "LRCombine"; };
};
#endif

#ifndef TBCONCAT_H
#define TBCONCAT_H
class TBCombine: public DoubleInputFiler
{
public:
    void Execute();
    const char *FilterName() { return "TBCombine"; };
};
#endif

#ifndef BLENDER_H
#define BLENDER_H
class Blender: public DoubleInputFiler
{
public:
    void SetFactor(double n) {factor = n;};
    void Execute();
    const char *FilterName() { return "Blender"; };
protected:
    double factor;
};
#endif

#ifndef CC_H
#define CC_H
class Color: public Source
{
public:
    Color(int width, int height, unsigned char r, unsigned char g, unsigned char b);
    void Execute();
    void Update();
    const char *SourceName() { return "Color"; };
};
#endif

#ifndef CS_H
#define CS_H
class CheckSum: public Sink
{
public:
    void OutputCheckSum(const char *filename);
    const char *SinkName() { return "CheckSum"; }
};
#endif

#ifndef MIRROR_H
#define MIRROR_H
class Mirror: public SingleInputFiler
{
public:
    void Execute();
    const char *FilterName() { return "Mirror"; };
};
#endif

#ifndef ROTATE_H
#define ROTATE_H
class Rotate: public SingleInputFiler
{
    void Execute();
    const char *FilterName() { return "Rotate"; };
};
#endif

#ifndef SUB_H
#define SUB_H
class Subtract: public DoubleInputFiler
{
    void Execute();
    const char *FilterName() { return "Subtract"; };
};
#endif

#ifndef GS_H
#define GS_H
class Grayscale: public SingleInputFiler
{
    void Execute();
    const char *FilterName() { return "Grayscale"; };
};
#endif

#ifndef BLUR_H
#define BLUR_H
class Blur: public SingleInputFiler
{
    void Execute();
    const char *FilterName() { return "Blur"; };
};
#endif
