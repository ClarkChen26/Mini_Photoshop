#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <source.h>
#include <logging.h>

Source::Source(void)
{
    image = new Image();
    image -> SetSource(this);
}

Source::~Source(void)
{
    delete image;
}

Image*
Source::GetOutput()
{
    return image;
}