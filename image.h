#ifndef PIXEL_H
#define PIXEL_H
struct Pixel
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};
#endif 

#ifndef IMAGE_H
#define IMAGE_H
class Source;
class Image 
{   
private:    
    int width;
    int height;
    Pixel *pixel;
    Source *source;
public:
    Image(void);            /*default*/
    ~Image(void);
    void ResetSize(int new_width, int new_height);
    void SetWidth(int w);
    void SetHeight(int h);
    void SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    int GetWidth() const;
    int GetHeight() const;
    Pixel* GetPixel() const;
    void Update() const;
    void SetSource(Source *s);
};
#endif
