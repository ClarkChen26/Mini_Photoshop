#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <image.h>
#include <PNMreader.h>
#include <logging.h>

PNMreader::PNMreader(char *f)
{
    filename = new char[strlen(f) + 1];
    int len = strlen(f);
    for (int i = 0; i<len; i++)
    {
        filename[i] = f[i];
    }
    filename[len] = '\0';

}
PNMreader::~PNMreader(void)
{
    delete [] filename;
}

void
PNMreader::Execute()
{

    FILE *f_in;
    f_in = fopen(filename, "r");
    char magicNum[128];
    int width, height, maxval;
    if (f_in == NULL)
    {
        char msg[1024];
        fprintf(stderr, "Unable to open file %s\n", filename);
        DataFlowException e(filename, msg);
        throw e;
    }
    fscanf(f_in, "%s\n%d %d\n%d\n", magicNum, &width, &height, &maxval);
    if (strcmp(magicNum, "P6") != 0)
    {
        fprintf(stderr, "Unable to read from file %s, because it is not a PNM file of type P6\n", filename);
    }
    image->ResetSize(width, height);
    fread(image->GetPixel(), sizeof(Pixel), height*width, f_in);
    fclose(f_in);

}

void PNMreader::Update()
{
    char msg[128];
    sprintf(msg, "%s: about to execute\n", SourceName());
    Logger::LogEvent(msg);
    Execute();
    sprintf(msg, "%s: done executing\n", SourceName());
    Logger::LogEvent(msg);
}