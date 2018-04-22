#include <stdlib.h> //Include the standard library.
#include <stdio.h> //Include standard input and output library.
#include <string.h> //Include string library.
#include <time.h> //Include time library.

typedef unsigned char bool; //Define boolean type.
#define FALSE	0 //In C, 0 is false. Setting this as my definition.
#define TRUE	!FALSE //Anything not false is true.

#define KEY_LENGTH		10 //Key length for decryption is always 10.
#define IMAGE_LENGTH	128 //Image length is always 128 bytes.
#define IMAGE_WIDTH		32 //Image width is always 32 pixels.
#define IMAGE_HEIGHT	32 //Image height is always 32 pixels.

const unsigned char table[256] = { //256 byte table used during key manipulation.
	0x6D, 0xE5, 0x9A, 0x4C, 0xC7, 0x35, 0x1A, 0x3B,
	0x78, 0xFB, 0x02, 0x84, 0x7B, 0x4B, 0x4A, 0xC0,
	0x6C, 0x9B, 0x36, 0x1F, 0x34, 0x4D, 0xCE, 0x24,
	0xB9, 0xE0, 0x29, 0x54, 0x99, 0x67, 0x19, 0x21,
	0x73, 0xCB, 0x57, 0x46, 0x2F, 0xDF, 0x5E, 0x43,
	0x72, 0x7A, 0x28, 0xB0, 0x0F, 0xF6, 0x49, 0xE2,
	0x12, 0xF0, 0x09, 0x44, 0xCD, 0x69, 0x95, 0x6B,
	0xD3, 0xD1, 0xE6, 0x87, 0x92, 0xF7, 0xDD, 0x89,
	0xC2, 0x63, 0xEA, 0x1D, 0xBB, 0xA7, 0x0A, 0x48,
	0x93, 0x90, 0xC1, 0x08, 0x14, 0x1B, 0x79, 0x91,
	0x65, 0xF8, 0x0D, 0xD8, 0xD0, 0x47, 0xE1, 0xF9,
	0x15, 0x9E, 0x05, 0x41, 0xC8, 0xB7, 0x0E, 0x7E,
	0x22, 0xE9, 0xDA, 0xB1, 0x62, 0x13, 0x26, 0x42,
	0xAB, 0xD4, 0x5C, 0x4F, 0x74, 0xC4, 0x04, 0x66,
	0xD6, 0x5F, 0x38, 0x4E, 0x10, 0xA5, 0x75, 0x52,
	0xAA, 0xFE, 0xF2, 0xA3, 0x70, 0x25, 0x82, 0x3A,
	0x0C, 0x9D, 0x97, 0x56, 0x7D, 0xD5, 0xA4, 0xE8,
	0xAF, 0x11, 0xB8, 0x33, 0xE4, 0xF4, 0x3E, 0x60,
	0xCC, 0x5A, 0xFD, 0x71, 0xDE, 0x94, 0x7F, 0x40, 
	0x53, 0xD7, 0xF3, 0x03, 0x96, 0xBF, 0x17, 0x2C,
	0x98, 0xF5, 0x50, 0x8A, 0x88, 0x59, 0xAC, 0x6E,
	0x8E, 0x77, 0xC5, 0x58, 0x8D, 0xC9, 0xB5, 0xBE,
	0x3F, 0xEC, 0xA2, 0xBC, 0xA0, 0x23, 0x0B, 0x85,
	0xB2, 0x86, 0x07, 0x61, 0xD9, 0xA1, 0x8F, 0x7C,
	0x01, 0x64, 0xAD, 0x3C, 0xFF, 0x06, 0x8B, 0xA8,
	0xE3, 0x76, 0x31, 0x80, 0xEF, 0x81, 0x51, 0x32,
	0x45, 0xDB, 0x3D, 0x1E, 0x20, 0xBA, 0x8C, 0x27,
	0x30, 0x6A, 0xD2, 0xB3, 0x18, 0xB4, 0xC6, 0xFC,
	0x55, 0x1C, 0xDC, 0xEB, 0xAE, 0xF1, 0xA6, 0xCA,
	0x6F, 0x5B, 0x9F, 0x16, 0x9C, 0xCF, 0xB6, 0xEE,
	0x39, 0xA9, 0x2A, 0x68, 0x37, 0xFA, 0x5D, 0x83,
	0x00, 0x2D, 0xED, 0x2E, 0x2B, 0xE7, 0xBD, 0xC3};

const unsigned char original_image[IMAGE_LENGTH] = { //Bytecode of original image. 128 bytes.
	0x7D, 0xBC, 0x5D, 0x92, 0xC7, 0x66, 0xFB, 0x16,
	0xB3, 0xC7, 0x25, 0x39, 0x07, 0x65, 0xE4, 0x00,
	0x2E, 0xCE, 0xBC, 0xA8, 0xDF, 0x6A, 0x85, 0xE7,
	0x8E, 0x5E, 0x49, 0x51, 0xD9, 0x8E, 0xFC, 0x5C,
	0xD5, 0xCA, 0x3E, 0x0E, 0x90, 0xF8, 0x6A, 0x3E,
	0x38, 0xC1, 0xBE, 0x33, 0xDC, 0x4B, 0xCE, 0x04,
	0x50, 0x4D, 0x50, 0xA3, 0xE7, 0x80, 0x88, 0xDB,
	0xEF, 0x6D, 0xC8, 0xB4, 0x3F, 0xEB, 0x3B, 0x39,
	0xA0, 0x64, 0xEE, 0xC4, 0xE3, 0xA0, 0x89, 0x2F,
	0x83, 0xD1, 0x98, 0x62, 0xB6, 0xFF, 0xEB, 0x07,
	0x0F, 0xC2, 0xE8, 0xDB, 0x53, 0xDA, 0x0B, 0xFA,
	0x16, 0x53, 0x9B, 0x51, 0x6F, 0xB3, 0xD8, 0x5E,
	0xBD, 0x05, 0xD1, 0xD6, 0x0D, 0xF6, 0x66, 0xBC,
	0x89, 0x84, 0x44, 0xAD, 0x68, 0x09, 0x4C, 0x00,
	0x9F, 0x60, 0x8D, 0x79, 0xAB, 0x73, 0xF8, 0x12,
	0x98, 0xE1, 0x56, 0x62, 0x36, 0x9B, 0xDF, 0xE7};

bool incrementKey(unsigned char key_current_seed[])	//Increment the least significant value of the key and handle carry.
{
	bool reached_final_key = FALSE; //Default assumption.
	for(unsigned char i = 0; i < KEY_LENGTH; i++) //Iterate over the entire key.
	{
		unsigned char index = KEY_LENGTH - (i + 1); //We use this variable as the index for the seed, so we can go over it in reverse order.
		++key_current_seed[index]; //Increment the current seed digit.
		if(key_current_seed[index] != 0x00) //If we didn't wrap around. (C standard says unsigned addition will wraparound to 0.)
		{
			break;
		}
		key_current_seed[index] = 0xF6; //Set the value to an offset 0 and continue.
		else if(index == 0) //If this is the most significant digit in the seed,
		{
			reached_final_key = TRUE;
			break;
		}
	}
	return reached_final_key;
}

void saveFinishedImage(unsigned char rgb_image[], unsigned char key_current_seed[])//Save an image to disk with the name of the seed used to generate it.
{
	char filename[KEY_LENGTH + 5]; //File name is key as name. We add five bytes for the file extension and a terminator.
	//We can't use the old string, because C requires a terminated string to use standard string functions.
	FILE *file_pointer = NULL; //Pointer to image file.
	const char *file_extension = ".ppm"; //Create a string for the file extension we're using. We'll use this to help save the image later.
	for(unsigned char i = 0; i < KEY_LENGTH; i++) //Iterate over the seed.
	{
		unsigned char temp_value;
		temp_value = key_current_seed[i];
		temp_value -= 0xF6;
		filename[i] = (char)temp_value + '0'; //Write the number to the string, offset by the ASCII table instead.
	}
	filename[KEY_LENGTH] = '\0'; //Terminate the string.
	strcat(filename, file_extension); //Append extension to the filename.
	file_pointer = fopen(filename, "wb"); //Open the file "[seed].ppm" to write in binary mode.
	fprintf(file_pointer, "P6\n %d\n %d\n 255\n", IMAGE_WIDTH, IMAGE_HEIGHT); //Write magic number for .ppm file, image width, image height, and max intensity.
	fwrite(rgb_image, 3072, 1, file_pointer); //Write the image data.
	fclose(file_pointer);
	printf("Finished saving %s.\n", filename);
	return;
}

void convertImageToRGB(unsigned char monochrome_image[], unsigned char rbg_image[]) //Convert the image from the decoded, monochrome format to an RGB string.
{	//The original monochrome bitstream image is split into a group of 64 bits to create an 8x8 tile, which all form into a 4x4 tileset.
	for(unsigned char image_index = 0; image_index < IMAGE_LENGTH; image_index++) //Iterate over the monochrome bitfield, one byte at a time.
	unsigned short memory_offset; //How many bytes we'd offset, if we were writing a greyscale image. This helps with getting our RGB offset later.
	{//We can use bit arithmetic on the image_index to determine where exactly we should be writing to every loop.
		unsigned char tile_row = image_index % 8; //The first three least significant bits are the row in the current tile we're reading. Each row of a tile is a single byte.
		unsigned char x_tile = (image_index >> 3) % 4; //The next two bits are the x coordinate of current tile in the tileset we're reading from
		unsigned char y_tile = (image_index >> 5) % 4; //The next two are the same for y.
		//The eighth bit is ignored, since we're normally only iterating to 127.
		memory_offset = y_tile * IMAGE_WIDTH; //Move vertically down the image however many tiles.
		memory_offset += x_tile; //Move horizontally to the right however many tiles.
		memory_offset *= 8; //Offset by 8, since every tile has a width and height of 8 pixels.
		memory_offset += tile_row * IMAGE_WIDTH;//Move down however many rows within the tile.
		
		for(unsigned char bit = 0; bit < 8; bit++) //Iterate over bits in this byte.
		{
			unsigned short offset_to_write = (memory_offset + bit) * 3;//Multiply our greyscale offset by 3 to get our offset for writing an RGB image.
			unsigned char bit_check = 0x80; //Flag only the most significant bit,
			bit_check >>= bit; //then shift to the one we're checking.
			bool is_black = monochrome_image[image_index] & bit_check; //Check if the monochrome value we're checking is black. Black is 1. Yes, it's weird.
			unsigned char value_to_write = (is_black) ? 0 : 255; //If it's black, write 0. Otherwise, 255.
			for(unsigned char color = 0; color < 3; color++) //Write values for red, green, and blue to make either black or white.
			{
				rbg_image[offset_to_write + color] = value_to_write;
			}
		}
	}
	/*
	Runea's solutions:
	r = tile_row
	x = x_tile
	y = y_tile
	n = memory_offset
	t = color

	//in betweeny, madness has its steps
	for (int i = 0; i < 128; i++) //Iterate over the monochrome bitfield.
		for (int t = 0; t < 3; t++) // for every triplet of bytes per pixel
			for (int b = 0; b < 8; b++) // for every bit in the byte
				rbg_image[(8 * ((i >> 3) % 4 + (i >> 5) * 32) + (i % 8) * 32 + 7 - b) * 3 + t] = (image[i] & (0x01 << b)) ? 255 : 0;

	for (int i = 0; i < 3072; i++) // IM A MONSTER, I MUST BE STOPPED // THIS TOTALLY WORKS BTW
		rbg_image[(8 * (((i / 24) >> 3) % 4 + ((i / 24) >> 5) * 32) + ((i / 24) % 8) * 32 + 7 - i % 8) * 3 + (i % 24)/8] = (image[i / 24] & (1 << (i % 8))) ? 255 : 0;
	*/
	return;
}

void decodeImage(unsigned char monochrome_image[], unsigned char key_buffer[]) //Decode the image using an already manipulated key.
{
	for(unsigned char index = 0; index < IMAGE_LENGTH; index++) //Iterate over the image.
	{
		unsigned char key_index = index % KEY_LENGTH; //Key_index is just the index modulo the key length.
		monochrome_image[index] ^= key_buffer[key_index];
		key_buffer[key_index] = table[key_buffer[key_index]];
	}
	return;
}

unsigned char rrc(unsigned char to_shift) //Rotate right with carry.
{
	unsigned char shifted_right = to_shift >> 1; 
	unsigned char carry = to_shift << 7;
	carry &= 0x80; //Eliminate all bits except the highest.
	unsigned char result = shifted_right | carry; //Combine them.
	return result;
}

void manipulateKey(unsigned char key_buffer[]) //Manipulate the key buffer before image decryption.
{
	unsigned char new_digit; //Create a temporary value for the new digit well create in the loop.
	unsigned char previous = key_buffer[9]; //The initial value for the previous value is the least significant bit.
	for(unsigned char i = 0; i < 25; i++) //Iterate over the key buffer 25 times.
	{
		for(unsigned char j = 0; j < KEY_LENGTH; j++) //Iterate over every value in the buffer.
		{
			new_digit = key_buffer[j];
			new_digit = rrc(new_digit);
			new_digit ^= 0x5C;
			new_digit += 0x1E;
			new_digit = table[new_digit];
			new_digit ^= previous;
			previous = new_digit;
			key_buffer[j] = new_digit;
		}
	}
	return;
}

void attemptDecrypt(unsigned char key_current_seed[], unsigned char monochrome_image[]) //Attempt to decrypt the current key and output a monochrome image.
{
	unsigned char key_buffer[KEY_LENGTH]; //Create a temporary buffer to manipulate the key and decode the image with.
	for(unsigned char i = 0; i < KEY_LENGTH; i++) //Iterate over all digits of the key,
	{
		key_buffer[i] = key_current_seed[i]; //and assign them to a temporary buffer.
	}

	manipulateKey(key_buffer); //Manipulate the temporary buffer.
	decodeImage(monochrome_image, key_buffer); //Decode the image.
	return;
}

int main() //Entrypoint.
{
	time_t start = time(NULL); //Set the start time.
	
	unsigned char key_current_seed[KEY_LENGTH]; //Current seed..
	for (unsigned char i = 0; i < KEY_LENGTH; i++) //Iterate over the seed buffer,
	{
		key_current_seed[i] = 0xF6; //and initialize it with the default seed.
	}
	//unsigned char key_current_seed[KEY_LENGTH] = {0xFC, 0xFE, 0xF9, 0xF6, 0xFC, 0xFD, 0xF8, 0xFF, 0xFF, 0xFB}; //Replaces previous section. Correct key.

	bool reached_final_key = FALSE; //Default assumption
	unsigned char monochrome_image[IMAGE_LENGTH]; //Create a buffer for a monochrome image we'll use very soon.
	unsigned char rgb_image[3072]; //Create a buffer we'll use to save the image when we convert it from monochrome to RGB.
	printf("Starting the main loop...\n");
	while(!reached_final_key) //Main loop.
	{
		for(unsigned char i = 0; i < IMAGE_LENGTH; i++) //Populate buffer for monochrome image with the original image.
		{
			monochrome_image[i] = original_image[i];
		}
		attemptDecrypt(key_current_seed, monochrome_image);
		convertImageToRGB(monochrome_image, rgb_image);
		saveFinishedImage(rgb_image, key_current_seed);
		reached_final_key = incrementKey(key_current_seed); //Increment the key and check whether or not we've tried to decrypt with the final key.
	}
	printf("Execution took %.0f seconds.\n", difftime(time(NULL), start)); //Calculate the difference between the time values and print it.
	//Since the times are floats and the time difference isn't counting decimals, we truncate the decimals for display. Hence %.0f instead of %f.
	printf("Press enter to continue...");
	getchar(); //Wait for user input before we finish, so the user can read how long things took and verify the output.
	return EXIT_SUCCESS;
}

/*
int alternate() // Mai kinda way, if you want images, just pipe the output to a file.
{	// idea is you pipe the output to a file, then use regex to look for patterns. UNIX!
	unsigned char key[KEY_LENGTH] = {0xF6, 0xF6, 0xF6, 0xF6, 0xF6, 0xF6, 0xF6, 0xF6, 0xF6, 0xF6};
	unsigned char image[IMAGE_LENGTH];
	do
	{
		unsigned char ckey[KEY_LENGTH];
		for (int i = 0; i < KEY_LENGTH; i++)
		{
			ckey[i] = key[i];
			printf("%d", ckey[i] - 0xf6);
		}
		printf(":\n");
		unsigned char prev = ckey[KEY_LENGTH - 1];
		for (int i = 0; i < 25; i++)
		{
			prev ^= table[(ckey[i] >> 1 | ckey[i] << 7) ^ 0x5c + 0x1e];
			ckey[i] = prev;
		}
		for (int i = 0; i < IMAGE_LENGTH; i++)
		{
			image[i] = ckey[i % KEY_LENGTH] ^ original_image[i];
			ckey[i % KEY_LENGTH] = table[ckey[i % KEY_LENGTH]];
			printf("%02x", image[i]);
		}
		printf("\n");
		for (int i = KEY_LENGTH - 1; i >= 0; i--)
		{
			if (++key[i] || !i)
				break;
			key[i] = 0xF6;
		}
	} while (key[0]);
	return 0;
}
*/
