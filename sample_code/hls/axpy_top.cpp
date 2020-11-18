#include <stdio.h>
#include <stdlib.h>

#include "axpy.h"

void mat_sum_top (
		AXI_VAL INPUT_STREAM[2*MCR_SIZE],
		AXI_VAL OUTPUT_STREAM[MCR_SIZE])
{
#pragma HLS INTERFACE s_axilite port=return     bundle=CONTROL_BUS
#pragma HLS INTERFACE axis      port=OUTPUT_STREAM
#pragma HLS INTERFACE axis      port=INPUT_STREAM

	wrapped_axpy_hw <int, 32, 4, 5, 5>(INPUT_STREAM, OUTPUT_STREAM);
}

