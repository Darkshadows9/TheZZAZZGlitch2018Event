// decodecc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define FALSE 0
#define TRUE !FALSE

const unsigned char table[256] = { //256 byte table used during key manipulation.
    0x6D, 0xE5, 0x9A, 0x4C, 0xC7, 0x35, 0x1A, 0x3B, 0x78, 0xFB, 0x02, 0x84, 0x7B, 0x4B, 0x4A, 0xC0,
    0x6C, 0x9B, 0x36, 0x1F, 0x34, 0x4D, 0xCE, 0x24, 0xB9, 0xE0, 0x29, 0x54, 0x99, 0x67, 0x19, 0x21,
    0x73, 0xCB, 0x57, 0x46, 0x2F, 0xDF, 0x5E, 0x43, 0x72, 0x7A, 0x28, 0xB0, 0x0F, 0xF6, 0x49, 0xE2,
    0x12, 0xF0, 0x09, 0x44, 0xCD, 0x69, 0x95, 0x6B, 0xD3, 0xD1, 0xE6, 0x87, 0x92, 0xF7, 0xDD, 0x89,
    0xC2, 0x63, 0xEA, 0x1D, 0xBB, 0xA7, 0x0A, 0x48, 0x93, 0x90, 0xC1, 0x08, 0x14, 0x1B, 0x79, 0x91,
    0x65, 0xF8, 0x0D, 0xD8, 0xD0, 0x47, 0xE1, 0xF9, 0x15, 0x9E, 0x05, 0x41, 0xC8, 0xB7, 0x0E, 0x7E,
    0x22, 0xE9, 0xDA, 0xB1, 0x62, 0x13, 0x26, 0x42, 0xAB, 0xD4, 0x5C, 0x4F, 0x74, 0xC4, 0x04, 0x66,
    0xD6, 0x5F, 0x38, 0x4E, 0x10, 0xA5, 0x75, 0x52, 0xAA, 0xFE, 0xF2, 0xA3, 0x70, 0x25, 0x82, 0x3A,
    0x0C, 0x9D, 0x97, 0x56, 0x7D, 0xD5, 0xA4, 0xE8, 0xAF, 0x11, 0xB8, 0x33, 0xE4, 0xF4, 0x3E, 0x60,
    0xCC, 0x5A, 0xFD, 0x71, 0xDE, 0x94, 0x7F, 0x40, 0x53, 0xD7, 0xF3, 0x03, 0x96, 0xBF, 0x17, 0x2C,
    0x98, 0xF5, 0x50, 0x8A, 0x88, 0x59, 0xAC, 0x6E, 0x8E, 0x77, 0xC5, 0x58, 0x8D, 0xC9, 0xB5, 0xBE,
    0x3F, 0xEC, 0xA2, 0xBC, 0xA0, 0x23, 0x0B, 0x85, 0xB2, 0x86, 0x07, 0x61, 0xD9, 0xA1, 0x8F, 0x7C,
    0x01, 0x64, 0xAD, 0x3C, 0xFF, 0x06, 0x8B, 0xA8, 0xE3, 0x76, 0x31, 0x80, 0xEF, 0x81, 0x51, 0x32,
    0x45, 0xDB, 0x3D, 0x1E, 0x20, 0xBA, 0x8C, 0x27, 0x30, 0x6A, 0xD2, 0xB3, 0x18, 0xB4, 0xC6, 0xFC,
    0x55, 0x1C, 0xDC, 0xEB, 0xAE, 0xF1, 0xA6, 0xCA, 0x6F, 0x5B, 0x9F, 0x16, 0x9C, 0xCF, 0xB6, 0xEE,
    0x39, 0xA9, 0x2A, 0x68, 0x37, 0xFA, 0x5D, 0x83, 0x00, 0x2D, 0xED, 0x2E, 0x2B, 0xE7, 0xBD, 0xC3};

const unsigned char original_image[128] = { //Bytecode of original image.
    0x7D, 0xBC, 0x5D, 0x92, 0xC7, 0x66, 0xFB, 0x16, 0xB3, 0xC7, 0x25, 0x39, 0x07, 0x65, 0xE4, 0x00,
    0x2E, 0xCE, 0xBC, 0xA8, 0xDF, 0x6A, 0x85, 0xE7, 0x8E, 0x5E, 0x49, 0x51, 0xD9, 0x8E, 0xFC, 0x5C,
    0xD5, 0xCA, 0x3E, 0x0E, 0x90, 0xF8, 0x6A, 0x3E, 0x38, 0xC1, 0xBE, 0x33, 0xDC, 0x4B, 0xCE, 0x04,
    0x50, 0x4D, 0x50, 0xA3, 0xE7, 0x80, 0x88, 0xDB, 0xEF, 0x6D, 0xC8, 0xB4, 0x3F, 0xEB, 0x3B, 0x39,
    0xA0, 0x64, 0xEE, 0xC4, 0xE3, 0xA0, 0x89, 0x2F, 0x83, 0xD1, 0x98, 0x62, 0xB6, 0xFF, 0xEB, 0x07,
    0x0F, 0xC2, 0xE8, 0xDB, 0x53, 0xDA, 0x0B, 0xFA, 0x16, 0x53, 0x9B, 0x51, 0x6F, 0xB3, 0xD8, 0x5E,
    0xBD, 0x05, 0xD1, 0xD6, 0x0D, 0xF6, 0x66, 0xBC, 0x89, 0x84, 0x44, 0xAD, 0x68, 0x09, 0x4C, 0x00,
    0x9F, 0x60, 0x8D, 0x79, 0xAB, 0x73, 0xF8, 0x12, 0x98, 0xE1, 0x56, 0x62, 0x36, 0x9B, 0xDF, 0xE7};

//const unsigned char key_default_seed[10] = {
//	0xF6, 0xF6, 0xF6, 0xF6, 0xF6, 0xF6, 0xF6, 0xF6, 0xF6, 0xF6 };

/*
void convertImageToRGB(unsigned char *image, unsigned char *rgb_buffer) //Convert the image from the decoded format to an RGB string.
{
	for (unsigned char image_index = 0; image_index < 127; image_index++) //128 bytes in the original image, which is a monochrome bitfield.
	{
		for (unsigned char byte_offset = 0; byte_offset < 7; byte_offset++) //Every bit becomes a byte, so we write 8 bytes for every byte we read.
		{
			unsigned char bit_to_copy = 0x80;  //Initialize this value.
			bit_to_copy >>= (7 - byte_offset); //Shift to whatever bit we're actually checking this time.
			bit_to_copy &= image[image_index]; //Check the value by ANDing.
			bit_to_copy <<= byte_offset;	   //We bitshift so that if we got a match, the value is now 1. Otherwise, it's 0.
			bit_to_copy *= 255;				   //We multiply the value by to make it a maxed greyscale value instead of a black/white bit. By the power of math, this should only give 0 or 1 and not overflow.
			for (unsigned char rgb_offset = 0; rgb_offset < 2; rgb_offset++)
			{
				rgb_buffer[(image_index * 8 * 3) + ((byte_offset * 3) + rgb_offset)] = bit_to_copy; //We write three bytes per monochrome byte, since it's RGB. That's why the index is image_index * 8 * 3, plus the byte offset we need.
			}
		}
	}
	return;
}

void saveFinishedImage(unsigned char *rgb_buffer, unsigned char *key_current_seed) //Write image to PPM file.
{
	unsigned char width = 32; //Width and height of all decrypted images are 32.
	unsigned char height = 32;
	printf("Saving result for key: ");
	for (unsigned char i = 0; i == 9; i++)
	{
		printf("0x%X ", key_current_seed[i]);
	}
	printf("\n");
	char seed_string[11]; //We need a terminated string to properly write to other strings.
	for (unsigned char i = 0; i < 9; i++)
	{
		seed_string[i] = (unsigned char)(key_current_seed[i] - 0xC6); //Subtract a certain amount from the value, so it becomes the correct number, when used in a print statement.
	}
	seed_string[10] = '\0';										  //Terminate the new string.
	const char *file_name = strcat(seed_string, ".ppm");		  //Convert it into a filename.
	FILE *file_pointer = fopen(file_name, "wb");				  //Open file to write in binary mode.
	fprintf(file_pointer, "P6\n %d\n %d\n 255\n", width, height); //Magic number, comment, width, height, max intensity.
	fwrite(rgb_buffer, 3072, 1, file_pointer);
	fclose(file_pointer);
	printf("Finished saving %s.\n", file_name);
	return;
}*/

unsigned char rrc(unsigned char to_shift) //Rotate right with carry.
{
    unsigned char shifted_right = to_shift >> 1;  //Shift right one.
    unsigned char carry = to_shift << 7;          //This is the carry from the shift, since it's a rotate. Shift left seven.
    carry &= 0x80;                                //Eliminate all bits except the highest one by ANDing with 10000000. Just to make doubly sure and all.
    unsigned char result = shifted_right | carry; //OR them together to combine them.
    return result;
}

void shuffleKey(unsigned char *key_buffer)
{
    for (unsigned char i = 0; i < 25; i++) //Iterate 25 times.
    {
        unsigned char temp;
        unsigned char previous = key_buffer[9];
        for (unsigned char j = 0; j < 10; j++) //Iterate over every value.
        {
            temp = key_buffer[j]; //Temporary value is equal to the current value being manipulated.
            temp = rrc(temp);     //Rotate right with carry.
            temp ^= 0x5C;         //Bitwise OR.
            temp += 0x1E;         //Add 30.
            temp = table[temp];   //Get value from table.
            temp ^= previous;     //OR the value from the table with the secret value.
            previous = temp;      //Make the secret value the temp value.
            key_buffer[j] = temp; //Put it back into the buffer.
        }
    }
    return;
}

void decodeImage(unsigned char *image, unsigned char *key_buffer)
{
    for (unsigned char index = 0; index < 128; index++)
    {
        unsigned char key_index = index % 10;                 //Key index is the remainder after division by 10.
        image[index] ^= key_buffer[key_index];                // Bitwise OR.
        key_buffer[key_index] = table[key_buffer[key_index]]; //Take value from key_buffer to manipulate
    }
    return;
}

int imageRandomness(unsigned char *image)
{ // we test for how many times the image is changing between 1 and 0, guessing the image has any recurring bits.
    int count = 0;
    unsigned char previous = 0;
    unsigned char current;
    for (int i = 0; i < 128; i++) // through all bytes of image
    {
        for (int j = 7; j >= 0; j--) // all bits of byte
        {
            current = (image[i] >> j) & 1; // get the current bit
            if (previous != current)
            {
                previous = current;
                count++;
            }
        }
    }
    return count;
}

int attemptDecrypt(unsigned char *key_current_seed)
{
    unsigned char image[128];     //Create a buffer for the new image.
    unsigned char key_buffer[10]; //Create buffer to hold a copy of the seed for in-place mutation.
    for (unsigned char i = 0; i < 128; i++)
    {
        image[i] = original_image[i];
    }
    for (unsigned char i = 0; i < 10; i++)
    {
        key_buffer[i] = key_current_seed[i];
    }
    shuffleKey(key_buffer);
    decodeImage(image, key_buffer);

    //unsigned char rgb_buffer[3072];					 //Create a buffer we'll use to save the image.
    //convertImageToRGB(image, rgb_buffer);			 //Convert the binary black/white string to an RGB string.
    //saveFinishedImage(rgb_buffer, key_current_seed); //Save the image to disk with the name of the seed used.
    return imageRandomness(image);
}

bool incrementKey(unsigned char *key_current_seed)
{
    for (int i = 9; i >= 0; i--) // go through all digits
    {
        if (++key_current_seed[i]) // if addition doesnt overflow to 0, return successfully false
        {
            return FALSE;
        }
        key_current_seed[i] = 0xF6; // else revert to 0 char and continue to next digit.
    }
    return TRUE; // at the end if it still hasnt returned, return the failing true.
}

int main()
{
    time_t start = time(NULL);          //Set the start time.
    unsigned char key_current_seed[10]; //Seed we'll use every loop.
    int minRandomness = 1024;           // for filtering for what seems like an image.
    for (unsigned char i = 0; i < 10; i++)
    {
        key_current_seed[i] = 0xF6; //Initialize the seed.
    }
    printf("Starting...\n");
    do
    {
        int currentRandomness = attemptDecrypt(key_current_seed); //Attempt to decrypt the image with our seed.
        if (currentRandomness <= minRandomness)                   // ERR randomness is static? how so. is the algorim fixed in that variant?
        {
            minRandomness = currentRandomness;
            printf("New minimum: %d at ", minRandomness);
            for (int i = 0; i < 10; i++)
            {
                printf("%d", key_current_seed[i] - 0xf6);
            }
            printf("\n");
        }
    } while (!incrementKey(key_current_seed));
    printf("Execution took %f seconds.\n", difftime(time(NULL), start)); //Calculate the difference between the values and print it.
    return EXIT_SUCCESS;                                                 //Return successfully.
}