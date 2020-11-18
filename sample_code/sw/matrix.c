#include "dma_simple.h"

int main() {
    int uio1_fd = open("/dev/uio5", O_RDWR);
    void *hls_regs = mmap(NULL, 0x1000, PROT_READ|PROT_WRITE, MAP_SHARED, uio1_fd, 0);
    int uio2_fd = open("/dev/uio4", O_RDWR);
    void *dma_regs = mmap(NULL, 0x1000, PROT_READ|PROT_WRITE, MAP_SHARED, uio2_fd, 0);

    struct udmabuf intake_buf;
    struct udmabuf outlet_buf;
    if (udmabuf_open(&intake_buf, "udmabuf0") == -1) exit(1);
    if (udmabuf_open(&outlet_buf, "udmabuf1") == -1) exit(1);

    dma_reset(dma_regs);
    regs_write32(hls_regs, 0x81);

    int a[32*32];
    int b[32*32];
    int c[32*32];
    int cref[32*32];
    int i, j, t;
    int m, n, k;

    m = n = k = 32;

    for (i=0; i<m*m; i++) {
        a[i] = (int) i/2.0;
        b[i] = (int) i/3.0;
        c[i] = (int) 0;
    }

    for (i=0; i<m; i++) {
        for (j=0; j<n; j++) {
            ((int*)(intake_buf.buf))[i*n+j] = a[i*n+j];
        }
    }
    for (i=0; i<m; i++) {
        for (j=0; j<n; j++) {
            ((int *)(intake_buf.buf))[i*n+j+m*m] = b[i*n+j];
        }
    }
    
    dma_setup(dma_regs, intake_buf.phys_addr, outlet_buf.phys_addr);

    dma_outlet_start(dma_regs, m*m*4);
    dma_intake_start(dma_regs, m*m*2*4);

    dma_wait_irq(dma_regs);
    dma_clear_status(dma_regs);

    for (i=0; i<m; i++) {
        for (j=0; j<n; j++) {
            c[i*n+j] = ((int*)(outlet_buf.buf))[i*n+j];
        }
    }
    for (i=0; i<m; i++) {
        for (j=0; j<n; j++) {
            cref[i*n+j] = a[i*n+j] + b[i*n+j];
        }
    }

    int cnt_ok = 0;
    int cnt_ng = 0;
    for (i=0; i<m*m; i++) {
        if (c[i] == cref[i]) {
            cnt_ok++;
        } else {
            printf("NG: hw %d, sw: %d\n", c[i], cref[i]);
            cnt_ng++;
        }
    }
    printf("OK: %d, NG: %d, Total: %d\n", cnt_ok, cnt_ng, m*m);

    udmabuf_close(&outlet_buf);
    udmabuf_close(&intake_buf);
    close(uio2_fd);
    close(uio1_fd);
}
