#include <stdio.h>

#include "JLS.h"


// return:   -1:failed   0:success
// load PNM image file (including PGM and PPM)
//   PNM is a simple uncompressed image format, including two types: PPM (RGB) and PGM (grayscale)
//   For PPM specification, see https://netpbm.sourceforge.net/doc/ppm.html
//   For PGM specification, see https://netpbm.sourceforge.net/doc/pgm.html
int loadPNMfile (const char *p_filename, int *img_r, int *img_g, int *img_b, int *ysz, int *xsz, int *channel, int *bpp) {
    FILE *fp;
    int i, maxval;

    *ysz = *xsz = maxval = -1;
    
    if ( (fp = fopen(p_filename, "rb")) == NULL )
        return -1;

    if ( fgetc(fp) != 'P' ) {
        fclose(fp);
        return -1;
    }
    
    i = fgetc(fp);
    *channel = (i == '6') ? 3 : 1;
    if (i != '5' && i != '6') {
        fclose(fp);
        return -1;
    }

    if ( fscanf(fp, "%d%d%d", xsz, ysz, &maxval) < 3 ) {
        fclose(fp);
        return -1;
    }
    
    i = fgetc(fp);
    if (i != ' ' && i != '\n' && i != '\r' && i != '\t') {
        fclose(fp);
        return -1;
    }
    
    if ((*xsz) < 1 || (*ysz) < 1 || maxval < 2 || maxval > 65535) {
        fclose(fp);
        return -1;
    }
    
    for ((*bpp)=8; (*bpp)<=16; (*bpp)++)
        if (maxval < (1<<(*bpp)))
            break;
    
    if (*channel == 3) {
        for (i=(*xsz)*(*ysz); i>0; i--) {
            *img_r = fgetc(fp) & 0xFF;
            if (maxval >= 256) {
                *img_r <<= 8;
                *img_r += fgetc(fp) & 0xFF;
            }
            img_r ++;
            *img_g = fgetc(fp) & 0xFF;
            if (maxval >= 256) {
                *img_g <<= 8;
                *img_g += fgetc(fp) & 0xFF;
            }
            img_g ++;
            *img_b = fgetc(fp) & 0xFF;
            if (maxval >= 256) {
                *img_b <<= 8;
                *img_b += fgetc(fp) & 0xFF;
            }
            img_b ++;
        }
    } else {
        for (i=(*xsz)*(*ysz); i>0; i--) {
            *img_r = fgetc(fp) & 0xFF;
            if (maxval >= 256) {
                *img_r <<= 8;
                *img_r += fgetc(fp) & 0xFF;
            }
            *img_g = *img_b = *img_r;
            img_r ++;
            img_g ++;
            img_b ++;
        }
    }

    fclose(fp);
    return 0;
}


// return:
//     -1 : failed
//      0 : success
int writeBytesToFile (const char *p_filename, const unsigned char *p_buf, int len) {
    FILE *fp;
    int   len_actual;
    
    if ( (fp = fopen(p_filename, "wb")) == NULL )
        return -1;
    
    len_actual = fwrite(p_buf, sizeof(unsigned char), len, fp);
    
    fclose(fp);
    
    return (len != len_actual) ? -1 : 0;
}



#define    MAX_YSZ              8192
#define    MAX_XSZ              8192


int main (int argc, char **argv) {
    static int           img_r      [MAX_YSZ*MAX_XSZ];
    static int           img_g      [MAX_YSZ*MAX_XSZ];
    static int           img_b      [MAX_YSZ*MAX_XSZ];
    static unsigned char jls_buffer [MAX_YSZ*MAX_XSZ*6] = {0};
    
    const char *src_fname=NULL, *dst_fname=NULL;
    
    int ysz=-1 , xsz=-1, channel=-1 , bpp=-1 , near=0 , jls_length;
    
    printf("JPEG-LS baseline ITU-T T.87 encoder\n");
    printf("see github.com/WangXuan95/JPEG-LS\n");
    printf("\n");
    
    if (argc < 3) {                           // illegal arguments: print USAGE and exit
        printf("Usage:\n");
        printf("  To encode:\n");
        printf("    %s  <input-image-file>  <output-file>  [near]\n" , argv[0] );
        printf("    where <input-image-file> can be:\n");
        printf("      - .pgm (grayscale image)\n");
        printf("      - .ppm (RGB image)\n");
        printf("      - .pnm (grayscale or RGB image)\n");
        printf("    usually <output-file> ends with .jls , which means JPEG-LS file.\n");
        printf("\n");
        return -1;
    }
    
    src_fname = argv[1];
    dst_fname = argv[2];
    
    if (argc >= 4) {
        near = argv[3][0] - '0';
        if (near < 0) near = 0;
        if (near > 9) near = 9;
    }
    
    printf("  input  file        = %s\n" , src_fname);
    printf("  output file        = %s\n" , dst_fname);
    printf("  near               = %d\n" , near);
    
    if ( loadPNMfile(src_fname, img_r, img_g, img_b, &ysz, &xsz, &channel, &bpp) ) {
        printf("  **Error: open %s failed\n", src_fname);
        return -1;
    }
    
    printf("  image size         = %d x %d\n" , xsz , ysz );
    printf("  image channels     = %d (%s)\n" , channel , ((channel==3)?"RGB":"grayscale") );
    printf("  image bpp          = %d\n"      , bpp );
    
    if (ysz > MAX_YSZ) {
        printf("  **Error: image height (%d) must not be larger than %d", ysz, MAX_YSZ);
        return -1;
    }
    
    if (xsz > MAX_XSZ) {
        printf("  **Error: image width  (%d) must not be larger than %d", xsz, MAX_XSZ);
        return -1;
    }
    
    if (bpp > 8 && near) {
        printf("  **Error: current version do not support lossy in bpp>8 images");
        return -1;
    }
    
    if (channel == 3)
        jls_length = JLSencodeImageRGB (bpp, near, ysz, xsz, img_r, img_g, img_b, jls_buffer);
    else
        jls_length = JLSencodeImageGray(bpp, near, ysz, xsz, img_r, jls_buffer);
    
    if (jls_length <= 0) {
        printf("  **Error: compress failed\n");
        return -1;
    }
    
    printf("  compressed length  = %d Bytes\n" , jls_length );
    printf("  compressed bpp     = %.5f\n"     , 8.0*jls_length/(xsz*ysz) );
    
    if ( writeBytesToFile(dst_fname, jls_buffer, jls_length) ) {
        printf("  **Error: write %s failed\n", dst_fname);
        return -1;
    }
    
    printf("\n");
    
    return 0;
}

