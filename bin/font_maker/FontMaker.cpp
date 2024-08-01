#include "lodepng-master/lodepng.h"
#include "lodepng-master/lodepng.cpp"
#include <vector>
#include <iostream>
#include <fstream> 
#include <ft2build.h>


#include FT_FREETYPE_H      // part of library FreeType
#include FT_OUTLINE_H       // part of library FreeType

#define width_image  2048   // horizontal size 
#define height_image  2048  // vertical size
#define scale_image 48      // Scale of letter

#define color_r  255        //Inside color of letter 
#define color_g  255
#define color_b  255

#define border_r 0          // Letter border color 
#define border_g 0
#define border_b 0
#define borderWidth 2       // size of border

#define fontpath "font/ARIALN.TTF"  //path to the font used 
#define name "alphabet.png"         //Png name at output 


// Function for defining a pixel in the image
void SetPixel(std::vector<unsigned char>& image, unsigned width,unsigned height, unsigned x, unsigned y, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    if (x < width && y < height) {
        image[4 * width * y + 4 * x + 0] = r; //Red
        image[4 * width * y + 4 * x + 1] = g; //Green
        image[4 * width * y + 4 * x + 2] = b; //Blue
        image[4 * width * y + 4 * x + 3] = a; //Alpha
    }
}


// Draw the edge of the letters
void DrawBorder(std::vector<unsigned char>& image, unsigned width,unsigned height, unsigned startX, unsigned startY, FT_Bitmap* bitmap, int BorderWidth) {
    for (unsigned y = 0; y < bitmap->rows; ++y) {
        for (unsigned x = 0; x < bitmap->width; ++x) {
            if (bitmap->buffer[y * bitmap->pitch + x]) {
                for (int i = -BorderWidth; i <= BorderWidth; ++i) {
                    for (int j = -BorderWidth; j <= BorderWidth; ++j) {
                        int nx = x + i + startX;
                        int ny = y + j + startY;
                        if (i * i + j * j <= BorderWidth * BorderWidth) {
                            unsigned char value = bitmap->buffer[y * bitmap->pitch + x];
                            SetPixel(image, width,height, nx, ny, border_r, border_g, border_b, (value<150)?150: value);
                        }
                    }
                }
            }
        }
    }
}

// Function to draw a letter using FreeType
void DrawLetter(std::vector<unsigned char>& image, unsigned width,unsigned height, unsigned startX, unsigned startY, FT_Bitmap* pbitmap) {
    DrawBorder(image, width,height, startX, startY, pbitmap, borderWidth);
    for (unsigned y = 0; y < pbitmap->rows; ++y) {
        for (unsigned x = 0; x < pbitmap->width; ++x) {
            unsigned char value = pbitmap->buffer[y * pbitmap->pitch + x];
            if(value>= 150)
                SetPixel(image, width,height, startX + x, startY + y, color_r, color_g, color_b, value); 
        }
    }
}

// Function to create and save a PNG image
void CreatePNG(const char* filename, unsigned width, unsigned height, FT_Face face) {
    // Create a buffer to hold the image data
    std::vector<unsigned char> image(width * height * 4, 0); // RGBA format
    
    unsigned startX = 10;           //Start Point X
    unsigned startY = 10;           //StartPoint Y
    unsigned letterSpacing = 40;    //Space enter letters
    std::ofstream UVs ("LetterUV.txt", std::ofstream::out);
    // Draw each letter of the alphabet
    for (int letter = 32; letter< 383; ++letter) {
        if (FT_Load_Char(face, letter, FT_LOAD_RENDER)) {
            std::cerr << "Error loading character " << letter << std::endl;
            continue;
        }
        if (face->glyph->bitmap.buffer == nullptr) {
            std::cerr << "Character " << letter << " has no bitmap, skipping." << std::endl;
            continue;
        }
        if( letter == 127){ //Skip letter 
            letter = 160;
            continue;
        }

        UVs<< letter ;
        UVs<< " "<<(float)startX/(float)width_image << " " <<(((float)startY+face->glyph->bitmap.rows)/(float)height_image) ;//botomleft 
        UVs<< " "<<(float)startX/(float)width_image << " " <<((float)startY/(float)height_image);//topleft
        UVs<< " "<<((float)startX+face->glyph->bitmap.width)/(float)width_image << " " <<((float)startY/(float)height_image);//topright
        UVs<< " "<<((float)startX+face->glyph->bitmap.width)/(float)width_image << " " <<(((float)startY+face->glyph->bitmap.rows)/(float)height_image) << std::endl ;//bottomright
        DrawLetter(image, width,height, startX, startY, &face->glyph->bitmap);
        startX += face->glyph->bitmap.width + letterSpacing;
        if (startX + letterSpacing > width ) {
            startX = 10;
            startY += face->glyph->bitmap.rows + letterSpacing;
        }

    }
     UVs.close();

    // Encode the image
    unsigned error = lodepng::encode(filename, image, width, height);

    // Check for errors
    if (error) {
        std::cerr << "Encoding error " << error << ": " << lodepng_error_text(error) << std::endl;
    } else {
        std::cout << "PNG file created: " << filename << std::endl;
    }
}
int main() {

    //Initialize  FreeType
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Error initializing FreeType" << std::endl;
        return -1;
    }

    //Initialize Font
    FT_Face face;
    if (FT_New_Face(ft, fontpath, 0, &face)) {
        std::cerr << "Error loading font" << std::endl;
        return -1;
    }

    FT_Set_Pixel_Sizes(face, 0, scale_image); // Font size

    CreatePNG("alphabet.png", width_image, height_image, face);

    // freeing the memory 
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return 0;
}
