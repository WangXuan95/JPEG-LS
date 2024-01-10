#ifndef   __JLS_H__
#define   __JLS_H__

extern int JLSencodeImageGray(int bpp, int near, int ysz, int xsz, int *img, unsigned char *pbuf);                            // import from JLS.c

extern int JLSencodeImageRGB (int bpp, int near, int ysz, int xsz, int *img_r, int *img_g, int *img_b, unsigned char *pbuf);  // import from JLS.c

#endif // __JLS_H__
