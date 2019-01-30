#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <PNMwriter.h>
#include <logging.h>

void
PNMwriter::Write(char *filename)
{
    FILE *f_out;
    f_out = fopen(filename, "w");
    fprintf(f_out, "P6\n%d %d\n%d\n", input1->GetWidth(), input1->GetHeight(), 255);
    fwrite(input1->GetPixel(), sizeof(Pixel), input1->GetHeight() * input1->GetWidth(), f_out);
    fclose(f_out);
}