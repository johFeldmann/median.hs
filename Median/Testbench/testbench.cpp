#include <stdio.h>
#include <stdlib.h>

#include "../EasyBMP/EasyBMP.h"

int main()
{
	// Load input image
	BMP image;
	if(!image.ReadFromFile("input_image.bmp"))
	{
		printf("Error reading input image.\n");
		return 1;
	}


	// Compare image with reference
	BMP image_ref;
	if(!image_ref.ReadFromFile("output_image_reference.bmp"))
	{
		printf("Error reading reference picture.\n");
		return 1;
	}

	// Everthing is correct
	return 0;
}
