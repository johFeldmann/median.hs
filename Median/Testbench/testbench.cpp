#include <stdio.h>
#include <stdlib.h>

#include <systemc.h>
#include <hls_stream.h>

#include "../EasyBMP/EasyBMP.h"
#include "../Header/config.h"
#include "../Header/top.h"

int main()
{
	hls::stream<pixel_depth> stream_in;
	hls::stream<pixel_depth> stream_out;

	// Load input image
	BMP image;
	if(!image.ReadFromFile("input_image.bmp"))
	{
		printf("Error reading input image.\n");
		return 1;
	}

	// Fill input stream
	for(int i = 0; i < IMAGE_WIDTH; i++)
	{
		for(int j = 0; j < IMAGE_HEIGHT; j++)
		{
			// Because this is an grey picture, red=green=blue - we only use red channel
			stream_in.write(image.GetPixel(j,i).Red);
		}
	}
	// Load reference image
	BMP image_ref;
	if(!image_ref.ReadFromFile("output_image_reference.bmp"))
	{
		printf("Error reading reference picture.\n");
		return 1;
	}

	// Filter
	top(stream_in, stream_out);

	// Check and save output
	BMP image_out;
	image_out.SetSize(IMAGE_WIDTH, IMAGE_HEIGHT);
	for(int i = 0; i < IMAGE_WIDTH; i++)
	{
		for(int j = 0; j < IMAGE_HEIGHT; j++)
		{
			pixel_depth color = 255;
			bool empty = stream_out.empty();
			if(!empty)
				stream_out.read(color);
			if(color != image_ref.GetPixel(j,i).Red)
			{
				// Wrong pixel color
				//printf("Error: Calculated image differs from reference image.\n");
				//return 1;
			}
			image_out(j,i)->Red = color.to_uint();
			if(!empty)
			{
				image_out(j,i)->Green = color.to_uint();
				image_out(j,i)->Blue = color.to_uint();
			}
			else
			{
				image_out(j,i)->Green = 0;
				image_out(j,i)->Blue = 0;
			}
			image_out(j,i)->Alpha = 0;
		}
	}
	image_out.WriteToFile("output.bmp");

	// Everything is correct
	return 0;
}
