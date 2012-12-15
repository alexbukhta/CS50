/****************************************************************************
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 * Alexander Bukhta
 *
 * Copies a BMP piece by piece, then resizes it
 ***************************************************************************/
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: scale infile to outfile, please enter 4 arguments\n");
        return 1;
    }

    // Store the scaling value as n
    int scaling_value = atoi(argv[1]);
    if(scaling_value < 1 || scaling_value > 100)
        {
        printf("Please enter an integer between 1 and 100 for the scaling factor: \n");
        return 1;
        }
    
    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    
    

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    //Scale the width and preserve the old biWidth value
    int old_biWidth = bi.biWidth;
    bi.biWidth = bi.biWidth * scaling_value;
    
    //Scale the height and preserve the old biHeight value
    int old_biHeight = bi.biHeight;
    bi.biHeight = bi.biHeight * scaling_value;

    // determine padding for scanlines
    int padding =  (4 - (old_biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // determine padding for resized image
    int resize_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //scale the size of the image according to padding in the INFO header
    bi.biSizeImage = ((bi.biWidth * 3) + resize_padding) * abs(bi.biHeight);
    
    //scale the size of the image according to padding in the FILE header
    bf.bfSize = bi.biSizeImage + 54;
        
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(old_biHeight); i < biHeight; i++)
    {
        //Loops the amount of times the rows are repeated by the scaling value
        for(int l = scaling_value; l>0; l--)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < old_biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                //Loops the amount of times the columns are repeated by the scaling value
                for(int k = scaling_value; k>0; k--)
                    {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    }
            }
            
            // add padding
            for(int z = 0; z <resize_padding; z++)
                fputc(0x00, outptr);
            
            //Skips the padding
            fseek(inptr, padding, SEEK_CUR);
                
            // Points back to beginning
            if(l>1)
                fseek(inptr, -((sizeof(RGBTRIPLE) * old_biWidth) + padding), SEEK_CUR);
        }
    }

    // close infile
    fclose(inptr);
    
    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
