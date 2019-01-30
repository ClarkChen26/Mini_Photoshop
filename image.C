#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <source.h>
#include <sink.h>

Image::Image(void)
{
    width = 0;
    height = 0;
    pixel = NULL;
}

Image::~Image(void)
{
    if (pixel == NULL)
        return;
    delete [] pixel;
    pixel = NULL;
}

void
Image::ResetSize(int new_width, int new_height)
{
    if (pixel != NULL) {
        delete [] pixel;
        pixel = NULL;
    }
    width = new_width;
    height = new_height;
    pixel = (Pixel*)malloc(sizeof(Pixel)*width*height);
}

void
Image::SetWidth(int w)
{
    width = w;
}

void 
Image::SetHeight(int h)
{
    height = h;
}

void 
Image::SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
    pixel[y * width + x].r = r;
    pixel[y * width + x].g = g;
    pixel[y * width + x].b = b;
}

int
Image::GetWidth() const
{
    return width;
}

int 
Image::GetHeight() const
{
    return height;
}

Pixel*
Image::GetPixel() const
{
    return pixel;
}

void
Image::SetSource(Source *s)
{
    source = s;
}

void
Image::Update() const
{
    if (source != NULL)
    {
        source -> Update();
    }
}