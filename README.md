 ![language](https://img.shields.io/badge/language-C-green.svg) ![build](https://img.shields.io/badge/build-Windows-blue.svg) ![build](https://img.shields.io/badge/build-linux-FF1010.svg)

# JPEG-LS encoder

A simple JPEG-LS baseline (ITU-T T.87) image compressor (encoder) in C language.

　

# Background of JPEG-LS

**JPEG-LS** (**JLS**) is a lossless/lossy image compression standard which has the best lossless compression ratio compared to PNG, Lossless-JPEG2000, Lossless-WEBP, Lossless-HEIF, etc.

The standard specification of JPEG-LS is ITU-T T.87 [1], see https://www.itu.int/rec/T-REC-T.87/en 

**JPEG-LS** uses the maximum difference between the pixels before and after compression (**NEAR** value) to control distortion, **NEAR=0** is the lossless mode; **NEAR>0** is the lossy mode, the larger the **NEAR**, the greater the distortion and the greater the compression ratio.

The file suffix name for **JPEG-LS** compressed image is .**jls** .

　

# Development progress

- [x] gray 8 bit image lossless encode (near=0)
- [x] gray 8 bit image lossy encode (near-lossless, near≥1)
- [x] gray 9-16 bit image lossless encode (near=0)
- [ ] gray 9-16 bit image lossy encode (near-lossless, near≥1)
- [x] RGB 24 bit image lossless encode (near=0)
- [x] RGB 24 bit image lossy encode (near-lossless, near≥1)
- [x] RGB 48 bit image lossless encode (near=0)
- [ ] RGB 24 bit image lossy encode (near-lossless, near≥1)

　

# Code list

The code is in pure-C. See [src](./src) folder:

- `JLS.c` : Implement a JLS encoder
- `JLS.h` : Expose the JLS encoder function to users
- `JLSMain.c` : A main program with `main()` function, which call the JLS encoder to achieve image file compression.

　

# Compile

### Compile in Windows (CMD)

If you installed MinGW Compiler for Windows, you can compile using the command line (CMD).

```powershell
gcc src/*.c -o JLSencode.exe -O3 -Wall
```

We'll get the executable file `JLSencoder.exe` . Here I've compiled it for you, see `JLSencoder.exe` .

### Compile in Linux

Run the command in the current directory:

```bash
gcc src/*.c -o JLSencode -O3 -Wall
```

We'll get the binary file `JLSencoder` . 

　

# Run image compression

This program can compress the following three types of image file to `.jls` compressed file.

- PGM grayscale image file, whose specification is in https://netpbm.sourceforge.net/doc/pgm.html
- PPM RGB image file, whose specification is in https://netpbm.sourceforge.net/doc/ppm.html
- PNM grayscale or RGB image file, whose specification is in https://netpbm.sourceforge.net/doc/pnm.html

There are some PGM and PPM files in [testimage ](./testimage)directory.

### Run image compress in Windows (CMD)

Use the following command to compress a image file to a `.jls` file.

```bash
.\JLSencode.exe  <input-image-file>  <output-file(.jls)>  [near]
```

Where `[near]` is a optional parameter of range 0\~9 :

- 0 : lossless (default)
- 1\~9 : lossy. The larger the near value, the higher distortion and the lower compressed size.

For example, the following command to compress `rgb1.ppm` in `testimage` folder to `rgb1.jls` with near=1

```bash
.\JLSencode.exe  testimage\rgb1.ppm  rgb1.jls  1
```

### Run image compress in Linux

Use the following command to compress a image file to a `.jls` file.

```
./JLSencode  <input-image-file>  <output-file(.jls)>  [near]
```

　

# Appendix: How to decompress .jls file?

This repo do not have a JPEG-LS decompressor. There are several ways to decompress a .jls file:

- The most convenient method: Try [this website](https://products.groupdocs.app/viewer/JLS) to view .jls image online (it may not work).
- Use other JPEG-LS code or libraries such as UBC's JPEG-LS code [3] or CharLS [4].
- Use python `pillow` library and `pillow_jpls` library, see [5].

　

# Reference

[1] ITU-T T.87 : JPEG-LS baseline specification : https://www.itu.int/rec/T-REC-T.87/en 

[2] PNM Image File Specification : https://netpbm.sourceforge.net/doc/pnm.html

[3] UBC's JPEG-LS baseline Public Domain Code : http://www.stat.columbia.edu/~jakulin/jpeg-ls/mirror.htm

[4] CharLS, a C++ JPEG-LS library implementation : https://github.com/team-charls/charls

[5] pillow-jpls library for Python - PyPI :  https://pypi.org/project/pillow-jpls

[6] FPGA-based Verilog JPEG-LS encoder (basic version which support 8-bit gray lossless and lossy) : https://github.com/WangXuan95/FPGA-JPEG-LS-encoder

[7] FPGA-based Verilog JPEG-LS encoder (ultra high performance version which support 8-bit gray lossless) : https://github.com/WangXuan95/UH-JLS
