#include <assert.h>
#include <ap_axi_sdata.h>

#define MCR_SIZE 1024 // 32*32
typedef ap_axiu<32,4,5,5> AXI_VAL;

// 処理の実態
template <typename T, int DIM>
void axpy(T a[DIM][DIM], T b[DIM][DIM], T out[DIM][DIM]) {
	L1:for (int i = 0; i < DIM; ++i) {
		L2:for (int j = 0; j < DIM; ++j) {
			out[i][j] = a[i][j] + b[i][j];
		}
	}
}

template <typename T, int U, int TI, int TD>
T pop_stream(ap_axiu <sizeof(T)*8,U,TI,TD> const &e) {
	#pragma HLS INLINE

	// sizeof(T) == sizeof(int): 後述のunionによるcastのため(float等をビットレベルでキャストしたい)
	assert(sizeof(T) == sizeof(int));
	union {
		int ival;
		T oval;
	} converter;
	converter.ival = e.data;
	T ret = converter.oval;

	volatile ap_uint<sizeof(T)> strb = e.strb;
	volatile ap_uint<sizeof(T)> keep = e.keep;
	volatile ap_uint<U> user = e.user;
	volatile ap_uint<1> last = e.last;
	volatile ap_uint<TI> id = e.id;
	volatile ap_uint<TD> dest = e.dest;

	return ret;
}

template <typename T, int U, int TI, int TD> ap_axiu <sizeof(T)*8,U,TI,TD>
push_stream(T const &v, bool last = false) {
	#pragma HLS INLINE
	ap_axiu<sizeof(T)*8,U,TI,TD> e;

	assert(sizeof(T) == sizeof(int));
	union {
		int oval;
		T ival;
	} converter;
	converter.ival = v;
	e.data = converter.oval;

	// set it to sizeof(T) ones
	e.strb = -1;
	e.keep = 15; //e.strb;
	e.user = 0;
	e.last = last ? 1 : 0;
	e.id = 0;
	e.dest = 0;
	return e;
}

// 入出力と関数呼び出し
template <typename T, int DIM, int U, int TI, int TD>
void wrapped_axpy_hw (AXI_VAL in_stream[2*DIM*DIM], AXI_VAL out_stream[DIM*DIM]){
	#pragma HLS INLINE

	T a[DIM][DIM];
	T b[DIM][DIM];
	T out[DIM][DIM];

	assert(sizeof(T) == 4);

	for(int i=0; i<DIM; i++) {
		for(int j=0; j<DIM; j++) {
			#pragma HLS PIPELINE II=1
			int k = i*DIM+j;
			a[i][j] = pop_stream<T,U,TI,TD>(in_stream[k]);
		}
	}

	for(int i=0; i<DIM; i++) {
		for(int j=0; j<DIM; j++) {
			#pragma HLS PIPELINE II=1
			int k = i*DIM+j + DIM*DIM;
			b[i][j] = pop_stream<T,U,TI,TD>(in_stream[k]);
		}
	}

	axpy<T, DIM>(a,b,out);

	for(int i=0; i<DIM; i++) {
		for(int j=0; j<DIM; j++) {
			#pragma HLS PIPELINE II=1
			int k = i*DIM+j;
			bool last = k == DIM*DIM - 1;
			out_stream[k] = push_stream<T,U,TI,TD>(out[i][j], last);
		}
	}
}
