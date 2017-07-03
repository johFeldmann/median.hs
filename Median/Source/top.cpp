#include "../Header/top.h"
#include "../Header/config.h"

#include <systemc.h>

void FillBuffer(hls::stream<pixel_depth> &stream_in, sc_uint<8>* buffer, sc_uint<8> (&pixel)[3][3]);

void median(sc_uint<8>* pixel, sc_uint<8>& val);

void compare_exchange(sc_uint<8> ia, sc_uint<8> ib, sc_uint<8>& oa, sc_uint<8>& ob);

void top(hls::stream<pixel_depth> &stream_in, hls::stream<pixel_depth> &stream_out)
{
	#pragma HLS DEPENDENCE variable=stream_in inter true
	#pragma HLS DEPENDENCE variable=stream_out inter true
	#pragma HLS INTERFACE axis register port=stream_in
	#pragma HLS INTERFACE axis register port=stream_out
	//#pragma HLS DEPENDENCE variable=stream_in intra true
	//#pragma HLS DEPENDENCE variable=stream_out intra true

	sc_uint<8> pixel[3][3];
	sc_uint<8> buffer[2*IMAGE_WIDTH+3];

	// First Row - buffer and write through
	for(sc_uint<7> i = 0; i < IMAGE_WIDTH; i++)
	{
#pragma HLS DEPENDENCE variable=stream_in inter true
#pragma HLS DEPENDENCE variable=stream_out inter true
#pragma HLS DEPENDENCE variable=buffer inter true
		FillBuffer(stream_in, buffer, pixel);
		stream_out.write(pixel[0][0]);
	}

	// Second row - buffer
	for(sc_uint<7> i = 0; i < IMAGE_WIDTH; i++)
	{
#pragma HLS DEPENDENCE variable=stream_in inter true
#pragma HLS DEPENDENCE variable=stream_out inter true
#pragma HLS DEPENDENCE variable=buffer inter true
		FillBuffer(stream_in, buffer, pixel);
	}

	// Row 3 until IMAGE_HEIGHT-1: Calculate median
	for(sc_uint<7> i = 2; i < IMAGE_HEIGHT; i++)
	{

		FillBuffer(stream_in, buffer, pixel);
		stream_out.write(pixel[1][0]);

		FillBuffer(stream_in, buffer, pixel);

		for(sc_uint<7> j = 2; j < IMAGE_WIDTH; j++)
		{
			sc_uint<8> data;
			FillBuffer(stream_in, buffer, pixel);
			median(&pixel[0][0], data);
			stream_out.write(data);
		}

		stream_out.write(pixel[1][0]);
	}

	for(sc_uint<7> i = 0; i < IMAGE_WIDTH; i++)
	{
		#pragma HLS PIPELINE
		stream_out.write(buffer[IMAGE_WIDTH-i-1]);
	}
}

void FillBuffer(hls::stream<pixel_depth> &stream_in, sc_uint<8>* buffer, sc_uint<8> (&pixel)[3][3])
{
	FIFO:for(int i = 2*IMAGE_WIDTH+2; i > 0; i--)
	{
		buffer[i] = buffer[i-1];
	}

	stream_in.read(buffer[0]);

	// Pixel matrix are well defined places inside the buffer
	pixel[0][0] = buffer[0];
	pixel[0][1] = buffer[1];
	pixel[0][2] = buffer[2];
	pixel[1][0] = buffer[0+IMAGE_WIDTH];
	pixel[1][1] = buffer[1+IMAGE_WIDTH];
	pixel[1][2] = buffer[2+IMAGE_WIDTH];
	pixel[2][0] = buffer[0+2*IMAGE_WIDTH];
	pixel[2][1] = buffer[1+2*IMAGE_WIDTH];
	pixel[2][2] = buffer[2+2*IMAGE_WIDTH];
}

void median(sc_uint<8>* pixel, sc_uint<8>& val)
{
	sc_uint<8> buffer[12][9];

	compare_exchange(pixel[0], pixel[1], buffer[0][0], buffer[0][1]);
	compare_exchange(pixel[2], pixel[3], buffer[0][2], buffer[0][3]);
	compare_exchange(pixel[4], pixel[5], buffer[0][4], buffer[0][5]);
	compare_exchange(pixel[6], pixel[7], buffer[0][6], buffer[0][7]);
	buffer[0][8] = pixel[8];

	buffer[1][0] = buffer[0][0];
	compare_exchange(buffer[0][1], buffer[0][2], buffer[1][1], buffer[1][2]);
	compare_exchange(buffer[0][3], buffer[0][4], buffer[1][3], buffer[1][4]);
	compare_exchange(buffer[0][5], buffer[0][6], buffer[1][5], buffer[1][6]);
	compare_exchange(buffer[0][7], buffer[0][8], buffer[1][7], buffer[1][8]);

	median_label0:for(int j = 1; j < 10; j+=2)
	{
		compare_exchange(buffer[j][0], buffer[j][1], buffer[j+1][0], buffer[j+1][1]);
		compare_exchange(buffer[j][2], buffer[j][3], buffer[j+1][2], buffer[j+1][3]);
		compare_exchange(buffer[j][4], buffer[j][5], buffer[j+1][4], buffer[j+1][5]);
		compare_exchange(buffer[j][6], buffer[j][7], buffer[j+1][6], buffer[j+1][7]);
		buffer[j+1][8] = buffer[j][8];

		buffer[j+2][0] = buffer[j+1][0];
		compare_exchange(buffer[j+1][1], buffer[j+1][2], buffer[j+2][1], buffer[j+2][2]);
		compare_exchange(buffer[j+1][3], buffer[j+1][4], buffer[j+2][3], buffer[j+2][4]);
		compare_exchange(buffer[j+1][5], buffer[j+1][6], buffer[j+2][5], buffer[j+2][6]);
		compare_exchange(buffer[j+1][7], buffer[j+1][8], buffer[j+2][7], buffer[j+2][8]);
	}

	val = buffer[11][4];
}

void compare_exchange(sc_uint<8> ia, sc_uint<8> ib, sc_uint<8>& oa, sc_uint<8>& ob)
{
	if(ia > ib)
	{
		oa = ib;
		ob = ia;
	}
	else
	{
		oa = ia;
		ob = ib;
	}
}
