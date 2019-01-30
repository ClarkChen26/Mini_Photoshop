#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <sink.h>
#include <logging.h>


Sink::Sink(void)
{
    input1 = NULL;
    input2 = NULL;
}

void
Sink::SetInput(const Image *input1)
{
    this->input1 = input1;

}

void
Sink::SetInput2(const Image *input2)
{
    this->input2 = input2;
}