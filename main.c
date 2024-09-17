#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "schrift.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#define MAX_INPUT_SIZE 100

    
int main()
{

    char word[MAX_INPUT_SIZE];

    printf("Please enter some text (max %d characters): ", MAX_INPUT_SIZE - 1);

    if (fgets(word, MAX_INPUT_SIZE, stdin) == NULL) {
        printf("Error: Invalid Word");
        return 1;
    }


    int xScale = 140;
    int yScale = 140;
    int xPadding = 10;
    int yPadding = 100;
    int channels = 3;
    int wordLength = strlen(word);

    int maxLetterHeight = 0;
    SFT sft = {NULL, xScale, yScale, 0, 0, 0};

    sft.font = sft_loadfile("Open_Sans/OpenSans-VariableFont_wdth,wght.ttf");

    if (sft.font == NULL)
    {
        printf("Error: Font was not loaded");
        return 1;
    }

    // First calculate total image dimensions
    int totalWidth = xPadding;
    int totalHeight = yPadding + 10;

    for (size_t i = 0; i < wordLength; i++)
    {
        SFT_Glyph gid;
        if (sft_lookup(&sft, word[i], &gid) < 0)
        {
            printf("SFT LOOKUP FAILED\n");
            sft_freefont(sft.font);
            return 1;
        }

        SFT_GMetrics mtx;
        if (sft_gmetrics(&sft, gid, &mtx) < 0)
        {
            printf("SFT GMETRICS FAILED\n");
            sft_freefont(sft.font);
            return 1;
        }

        totalWidth += mtx.advanceWidth;
        maxLetterHeight = mtx.minHeight > maxLetterHeight ? mtx.minHeight : maxLetterHeight;
        if (totalHeight < yPadding + mtx.minHeight)
        {
            totalHeight = yPadding + mtx.minHeight;
        }
    }
    totalWidth += xPadding;
    yPadding = round(((totalHeight - maxLetterHeight) / 3) + maxLetterHeight);

    // Allocate memory for the image
    unsigned char* image = calloc(totalWidth * totalHeight, sizeof(unsigned char));
    if (image == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        sft_freefont(sft.font);
        return 1;
    }

    // Render each glyph and composite into the image
    for (size_t i = 0; i < wordLength; i++)
    {
        SFT_Glyph gid;
        if (sft_lookup(&sft, word[i], &gid) < 0)
        {
            printf("SFT LOOKUP FAILED\n");
            free(image);
            sft_freefont(sft.font);
            return 1;
        }

        SFT_GMetrics mtx;
        if (sft_gmetrics(&sft, gid, &mtx) < 0)
        {
            printf("SFT GMETRICS FAILED\n");
            free(image);
            sft_freefont(sft.font);
            return 1;
        }

        SFT_Image img = {
            .width = (mtx.minWidth + 3) & ~3,
            .height = mtx.minHeight,
        };

        char *pixels = (char *)calloc(img.width * img.height, sizeof(char));
        if (!pixels)
        {
            perror("Memory allocation failed\n");
            free(image);
            sft_freefont(sft.font);
            return 1;
        }
        img.pixels = pixels;

        if (sft_render(&sft, gid, img) < 0)
        {
            printf("SFT RENDER FAILED\n");
            free(pixels);
            free(image);
            sft_freefont(sft.font);
            return 1;
        }

        // Composite glyph into main image
        for (int iy = 0; iy < img.height; iy++)
        {
            for (int ix = 0; ix < img.width; ix++)
            {
                int img_x = xPadding + ix + mtx.leftSideBearing;
                int img_y = yPadding - iy - mtx.yOffset;

                if (img_x >= 0 && img_x < totalWidth && img_y >= 0 && img_y < totalHeight)
                {
                    image[img_y * totalWidth + img_x] = ((unsigned char *)img.pixels)[iy * img.width + ix];
                }
            }
        }

        free(pixels); 
        xPadding += mtx.advanceWidth;
    }

    // Write the grayscale image
    //stbi_write_png("output_gray.png", totalWidth, totalHeight, 1, image, totalWidth);

    // Convert to RGB
    unsigned char* newImage = calloc(totalWidth * totalHeight * channels, sizeof(unsigned char));
    if (newImage == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        free(image);
        sft_freefont(sft.font);
        return 1;
    }
    int br, bg, bb;
    int fr, fg, fb;

    printf("Enter your background color in this format: R-G-B \n");
    
    if (scanf("%d-%d-%d", &br, &bg, &bb) != 3) {  
        fprintf(stderr, "Error: Invalid input for background color. Expected format: R-G-B\n");
        return 1;
    }

    printf("Enter your foreground color in this format: R-G-B \n");
    
    if (scanf("%d-%d-%d", &fr, &fg, &fb) != 3) {  
        fprintf(stderr, "Error: Invalid input for foreground color. Expected format: R-G-B\n");
        return 1;
    }

    printf("Background color: R=%d, G=%d, B=%d\n", br, bg, bb);
    printf("Foreground color: R=%d, G=%d, B=%d\n", fr, fg, fb);

    for (size_t y = 0; y < totalHeight; y++)
    {
        for (size_t x = 0; x < totalWidth; x++)
        {
            if (image[y * totalWidth + x] != 0)
            {
                newImage[((y * totalWidth + x) * channels)] = fr;
                newImage[((y * totalWidth + x) * channels) + 1] = fg;
                newImage[((y * totalWidth + x) * channels) + 2] = fb;
            } else {
              newImage[((y * totalWidth + x) * channels)] = br;
                newImage[((y * totalWidth + x) * channels) + 1] = bg;
                newImage[((y * totalWidth + x) * channels) + 2] = bb;
            }
        }
    }

    free(image);
    if (stbi_write_png("images/text.png", totalWidth, totalHeight, 3, newImage, totalWidth * 3))
    {
        fprintf(stdout,"Image was written successfully");
    }
    else
    {
        fprintf(stderr,"Error: Image was not written successfully");
    }
     ;
    free(newImage);
    sft_freefont(sft.font);
    return 0;
}
