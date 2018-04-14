#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef unsigned char bool;
#define FALSE 0
#define TRUE !FALSE

#define KEY_LENGTH 10
#define IMAGE_LENGTH 128

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

const unsigned char original_image[IMAGE_LENGTH] = { //Bytecode of original image. 128 bytes.
	0x7D, 0xBC, 0x5D, 0x92, 0xC7, 0x66, 0xFB, 0x16, 0xB3, 0xC7, 0x25, 0x39, 0x07, 0x65, 0xE4, 0x00,
	0x2E, 0xCE, 0xBC, 0xA8, 0xDF, 0x6A, 0x85, 0xE7, 0x8E, 0x5E, 0x49, 0x51, 0xD9, 0x8E, 0xFC, 0x5C,
	0xD5, 0xCA, 0x3E, 0x0E, 0x90, 0xF8, 0x6A, 0x3E, 0x38, 0xC1, 0xBE, 0x33, 0xDC, 0x4B, 0xCE, 0x04,
	0x50, 0x4D, 0x50, 0xA3, 0xE7, 0x80, 0x88, 0xDB, 0xEF, 0x6D, 0xC8, 0xB4, 0x3F, 0xEB, 0x3B, 0x39,
	0xA0, 0x64, 0xEE, 0xC4, 0xE3, 0xA0, 0x89, 0x2F, 0x83, 0xD1, 0x98, 0x62, 0xB6, 0xFF, 0xEB, 0x07,
	0x0F, 0xC2, 0xE8, 0xDB, 0x53, 0xDA, 0x0B, 0xFA, 0x16, 0x53, 0x9B, 0x51, 0x6F, 0xB3, 0xD8, 0x5E,
	0xBD, 0x05, 0xD1, 0xD6, 0x0D, 0xF6, 0x66, 0xBC, 0x89, 0x84, 0x44, 0xAD, 0x68, 0x09, 0x4C, 0x00,
	0x9F, 0x60, 0x8D, 0x79, 0xAB, 0x73, 0xF8, 0x12, 0x98, 0xE1, 0x56, 0x62, 0x36, 0x9B, 0xDF, 0xE7};

bool incrementKey(unsigned char key_current_seed[])	//Increment the least significant value of the key and handle carry.
{
	bool reached_final_key = FALSE;					//Create a value to represent if we've reached the final key or not and set it to the default assumption.
	for(unsigned char i = 0; i < KEY_LENGTH; i++) 	//Iterate over all digits of the key in reverse order, so I can increment from the least significant digit.
	{
		unsigned char index = KEY_LENGTH - (i + 1);	//We use this variable as the index, so we can go over the seed in reverse order and keep our for loop format consistent through the whole program.
		++key_current_seed[index];					//Increment the current seed digit.
		if(key_current_seed[index] != 0x00) 		//If the increment didn't overflow to 0 (C standard says unsigned addition will overflow to 0),
		{
			reached_final_key = FALSE;				//we haven't reached the final value for the seed
			break;									//and we exit the loop.
		}
		else if(index == 0)							//Otherwise, if this is the most significant value in the iteration,
		{
			reached_final_key = TRUE;				//we've reached the final key
			break;									//and we should exit the loop.
		}
		else										//Otherwise,
		{
			key_current_seed[index] = 0xF6; 		//set the value to an offset 0 and continue.
		}
	}
	return reached_final_key;						//Return the result of our increment.
}

void saveFinishedImage(unsigned char rbg_image[], unsigned char key_current_seed[])	//Save an image to disk with the name of the seed used to generate it.
{
	unsigned char width = 32; 														//Width and height of all decrypted images are 32.
	unsigned char height = 32;
	char file_name[KEY_LENGTH + 5];													//Create a string we'll use to store the seed we used for this image. We add five for the file extension and a terminator.
																					//We can't use the old string, because it has a different purpose and we need to terminate it.
	for(unsigned char i = 0; i < KEY_LENGTH; i++)									//Use a lookup table to convert hex values to the correct number.
	{
		unsigned char temp_value;
		temp_value = key_current_seed[i];
		temp_value -= 0xF6;
		file_name[i] = (char)temp_value + '0';										//If we add '0', we offset the character table we're accessing and starting from the index where '0' is.
	}
	file_name[KEY_LENGTH] = '\0';													//Terminate the new string.
	const char *file_extension = ".ppm";
	strcat(file_name, file_extension);		 										//Convert it into a filename.
	FILE *file_pointer = fopen(file_name, "wb");				  					//Open file to write in binary mode.
	fprintf(file_pointer, "P6\n %d\n %d\n 255\n", width, height);					//Magic number, comment, width, height, max intensity.
	fwrite(rbg_image, 3072, 1, file_pointer);										//Write the file.
	fclose(file_pointer);															//Close the file.
	printf("Finished saving %s.\n", file_name);
	return;
}

void convertImageToRGB(unsigned char image[], unsigned char rbg_image[])							//Convert the image from the decoded, monochrome format to an RGB string.
{
	for(unsigned char image_index = 0; image_index < IMAGE_LENGTH; image_index++)					//Iterate over the monochrome bitfield.
	{
		for(unsigned char byte_offset = 0; byte_offset < 8; byte_offset++)							//Every bit becomes a byte, so we write 8 bytes for every byte we read.
		{
			unsigned char bit_to_copy = 0x80;//1000 0000  											//Initialize this value to only have the most significant bit flagged.
			bit_to_copy >>= (byte_offset);															//Use the loop count to shift to whatever bit we're actually checking this time. We move from the most to least significant bit.
			bit_to_copy &= image[image_index];														//Check the value in the image index by ANDing. This should set every bit to zero, except the one we're checking, which copies.
			bit_to_copy >>= (7 - byte_offset);														//Bitshift, so if we got a match, the value is now 1. Otherwise, it's 0.
			bit_to_copy *= 255;																		//We multiply the value by 255 to make it a maxed greyscale value instead of a black/white bit.
																									//By the power of math and computers, this should only give 0 or 1.
			for(unsigned char rgb_offset = 0; rgb_offset < 3; rgb_offset++)							//We write three bytes per monochrome byte, since it's an RGB image.
			{
				rbg_image[(image_index * 8 * 3) + ((byte_offset * 3) + rgb_offset)] = bit_to_copy;	//We write 8 bytes monochrome bytes as RGB bytes, hence 8 * 3, plus the byte and RGB offset we need.
			}
		}
	}
	return;
}

void decodeImage(unsigned char monochrome_image[], unsigned char key_buffer[])	//Decode the image using an already manipulated key.
{
	for(unsigned char index = 0; index < IMAGE_LENGTH; index++)					//Iterate over the image.
	{
		unsigned char key_index = index % KEY_LENGTH;							//Key_index is just the index modulo the key length.
		monochrome_image[index] ^= key_buffer[key_index];						//Bitwise OR.
		key_buffer[key_index] = table[key_buffer[key_index]];					//Take value from key_buffer to use for next time modulo key length equals this key index.
	}
	return;																		//After iterating over the image, we're done.
}

unsigned char rrc(unsigned char to_shift) 			//Rotate right with carry.
{
	unsigned char shifted_right = to_shift >> 1;	//Shift right one.
	unsigned char carry = to_shift << 7;			//This is the carry from the shift, since it's a rotate. Shift left seven.
	carry &= 0x80;									//Eliminate all bits except the highest one by ANDing with 10000000. Just to make doubly sure and all.
	unsigned char result = shifted_right | carry;	//OR them together to combine them.
	return result;									//Return the result of our rotation.
}

void manipulateKey(unsigned char key_buffer[])
{
	unsigned char new_digit;							//Create a temporary value for the new digit well create in the loop.
	unsigned char previous = key_buffer[9];				//The initial value for the previous value is the least significant bit.
	for(unsigned char i = 0; i < 25; i++)				//Iterate over the key buffer 25 times.
	{
		for(unsigned char j = 0; j < KEY_LENGTH; j++)	//Iterate over every value in the buffer.
		{
			new_digit = key_buffer[j];					//Temporary value is equal to the current value being manipulated.
			new_digit = rrc(new_digit);					//Rotate it right with carry.
			new_digit ^= 0x5C;							//Bitwise OR it with 0x5C.
			new_digit += 0x1E;							//Add 30 to the value.
			new_digit = table[new_digit];				//Get value from table using the current value of new_digit as the offset.
			new_digit ^= previous;						//OR the value from the table with the previous loop's value.
			previous = new_digit;						//Make the new previous value the value of the new digit
			key_buffer[j] = new_digit;					//and store the new digit in the key buffer for the next set of loops.
		}
	}
	return;
}

void attemptDecrypt(unsigned char key_current_seed[], unsigned char monochrome_image[])	//Attempt to decrypt the current key and output a monochrome image.
{
	unsigned char key_buffer[KEY_LENGTH];												//Create a temporary buffer to manipulate the key and decode the image with.
	for(unsigned char i = 0; i < KEY_LENGTH; i++) 										//Iterate over all digits of the key,
	{
		key_buffer[i] = key_current_seed[i];											//and assign them to a temporary buffer.
	}
	
	/*																					//I'm unsure if I actually need to reverse the buffer. I need to look at the assembly again.
	unsigned char reverse_buffer[KEY_LENGTH];											//Buffer to hold the key, while we reverse it.
	for(unsigned char i = 0; i < KEY_LENGTH; i++)										//Put the reversed value in the reversed buffer.
	{
		reverse_buffer[KEY_LENGTH-i] = key_buffer[i];
	}
	for(unsigned char i = 0; i < KEY_LENGTH; i++)										//Copy it back to the main key buffer.
	{
		key_buffer[i] = reverse_buffer[i];
	}
	*/
	
	manipulateKey(key_buffer);															//Manipulate the temporary buffer.
	decodeImage(monochrome_image, key_buffer);											//Decode the image.
	return;
}

int main()															//Entry point for the whole program.
{
	time_t start = time(NULL);          							//Set the start time.
	time_t end;														//Create the variable for the end time for a comparison later.
	unsigned char key_current_seed[KEY_LENGTH]; 					//Create the buffer for the seed we'll use every loop.
	for (unsigned char i = 0; i < KEY_LENGTH; i++)					//Iterate over the key buffer
	{
		key_current_seed[i] = 0xF6; 								//and initialize it with the default seed.
	}
	
	/*
	unsigned char key_current_seed[KEY_LENGTH] = {0xFC, 0xFE, 0xF9, 0xF6, 0xFC, 0xFD, 0xF8, 0xFF, 0xFF, 0xFB}; //Replaces previous section. Correct value.
	*/
	
	bool reached_final_key = FALSE;									//Create a flag to check if we've reached the final key and initialize it to the default value.
	unsigned char monochrome_image[IMAGE_LENGTH];					//Create a buffer for a monochrome image we'll use very soon.
	unsigned char rgb_image[3072];									//Create a buffer we'll use to save the image when we convert it from monochrome to RGB.
	printf("Starting the main loop...\n");
	while(TRUE)
	{
		for(unsigned char i = 0; i < IMAGE_LENGTH; i++)				//Populate buffer for monochrome image with the original image.
		{
			monochrome_image[i] = original_image[i];
		}
		attemptDecrypt(key_current_seed, monochrome_image);			//Attempt to decrypt the current key and write it to the image buffer.
		convertImageToRGB(monochrome_image, rgb_image);				//Convert the binary black/white string to an RGB string.
		saveFinishedImage(rgb_image, key_current_seed);				//Save the image to disk with the name of the seed used.
		reached_final_key = incrementKey(key_current_seed);			//Increment the key and check whether or not we've tried to decrypt with the final key.
		if(reached_final_key == TRUE)								//If we've reached the final value,
		{
			break;													//stop the main loop.
		}
	}
	end = time(NULL);
	printf("Execution took %f seconds.\n", difftime(end, start));	//Calculate the difference between the values and print it.
	return EXIT_SUCCESS;											//Return successfully.
}