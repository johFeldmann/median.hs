#ifndef TOP_H
#define TOP_H

#include "../Header/config.h"
#include <hls_stream.h>

void top(hls::stream<pixel_depth> &stream_in, hls::stream<pixel_depth> &stream_out);

#endif // TOP_H
