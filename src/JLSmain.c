#include <stdio.h>


extern int JLSencode (int bpp, int near, int ysz, int xsz, int *img, int *imgrcon, unsigned char *pbuf);               // import from JLS.c




// return:   -1:failed   0:success
int loadPGMfile (const char *filename, int *img_buffer, int *ysz, int *xsz, int *pix_max_val) {
    int i;
    FILE *fp;

    *ysz = *xsz = *pix_max_val = -1;
    
    if ( (fp = fopen(filename, "rb")) == NULL )
        return -1;

    if ( fgetc(fp) != 'P' ) {
        fclose(fp);
        return -1;
    }
    
    if ( fgetc(fp) != '5' ) {
        fclose(fp);
        return -1;
    }

    if ( fscanf(fp, "%d", xsz) < 1 ) {
        fclose(fp);
        return -1;
    }
    
    if ( fscanf(fp, "%d", ysz) < 1 ) {
        fclose(fp);
        return -1;
    }

    if ( fscanf(fp, "%d", pix_max_val) < 1 ) {
        fclose(fp);
        return -1;
    }
    
    i = fgetc(fp);
    if ( i != ' ' && i != '\n' && i != '\r' && i != '\t') {
        fclose(fp);
        return -1;
    }
    
    for (i=(*xsz)*(*ysz); i>0; i--) {
        if (feof(fp)) {                                        // pixels not enough
            fclose(fp);
            return -1;
        }
        *img_buffer = fgetc(fp) & 0xFF;
        if (*pix_max_val >= 256) {                             // two bytes per pixel
            *img_buffer <<= 8;
            *img_buffer += fgetc(fp) & 0xFF;
        }
        if (*img_buffer > *pix_max_val)
            *img_buffer = *pix_max_val;
        img_buffer ++;
    }

    fclose(fp);
    return 0;
}



// return:   -1:failed   0:success
int writeBytesToFile (const char *filename, const unsigned char *buffer, const int len) {
    const unsigned char *buffer_end_ptr = buffer + len;
    
    FILE *fp;
    
    if ( (fp = fopen(filename, "wb")) == NULL )
        return -1;

    for (; buffer<buffer_end_ptr; buffer++) {
        if ( fputc( *buffer , fp) == EOF ) {
            fclose(fp);
            return -1;
        }
    }

    fclose(fp);
    return 0;
}




#define    MAX_YSZ              8192
#define    MAX_XSZ              8192


int main (int argc, char **argv) {

    static int           img        [MAX_YSZ*MAX_XSZ];
    static int           imgrcon    [MAX_YSZ*MAX_XSZ];
    static unsigned char jls_buffer [MAX_YSZ*MAX_XSZ*2] = {0};

    const char *src_fname=NULL, *dst_fname=NULL;

    int ysz=-1 , xsz=-1 , pix_max_val=-1, bpp=8 , near=0 , jls_length;


    printf("JPEG-LS baseline ITU-T T.87 encoder\n");
    printf("see github.com/WangXuan95/JPEG-LS\n");
    printf("\n");
    
    
    if (argc < 3) {                           // illegal arguments: print USAGE and exit
        printf("Usage:\n");
        printf("    Encode:\n");
        printf("        %s  <input-image-file(.pgm)>  <output-file(.jls)>  [near]\n" , argv[0] );
        printf("\n");
        printf("Note: the file suffix \".jls\" means JPEG-LS image file\n");
        printf("\n");
        return -1;
    }
    
    
    src_fname = argv[1];
    dst_fname = argv[2];
    
    
    if (argc >= 4) {
        near = argv[3][0] - '0';
        if (near < 0)
            near = 0;
        if (near > 9)
            near = 9;
    }


    printf("  near               = %d\n" , near);
    printf("  input  file        = %s\n" , src_fname);
    printf("  output file        = %s\n" , dst_fname);


    if ( loadPGMfile(src_fname, img, &ysz, &xsz, &pix_max_val) ) {
        printf("open %s failed\n", src_fname);
        return -1;
    }
    
    
    for (bpp=8; bpp<=16; bpp++)
        if ( pix_max_val < (1<<bpp) )
            break;
    
    
    printf("  image size         = %d x %d\n"  , xsz , ysz );
    printf("  original bpp       = %d\n"       , bpp );
    
    
    if (ysz > MAX_YSZ) {
        printf("  *** image height (%d) must not be larger than %d", ysz, MAX_YSZ);
        return -1;
    }
    
    if (xsz > MAX_XSZ) {
        printf("  *** image width  (%d) must not be larger than %d", xsz, MAX_XSZ);
        return -1;
    }
    
    if (bpp > 8 && near) {
        printf("  *** Error: current version do not support lossy in bpp>8 images");
        return -1;
    }

    jls_length = JLSencode(bpp, near, ysz, xsz, img, imgrcon, jls_buffer);
    
    if (jls_length <= 0) {
        printf("compress failed\n");
        return -1;
    }
    

    printf("  compressed length  = %d Bytes\n" , jls_length );
    printf("  compressed bpp     = %.5f\n"     , 8.0*jls_length/(xsz*ysz) );
    
    
    if ( writeBytesToFile(dst_fname, jls_buffer, jls_length) ) {
        printf("write %s failed\n", dst_fname);
        return -1;
    }
    
    
    return 0;
}

