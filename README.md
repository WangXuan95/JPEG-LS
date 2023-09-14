 ![language](https://img.shields.io/badge/language-C-green.svg) ![build](https://img.shields.io/badge/build-Windows-blue.svg) ![build](https://img.shields.io/badge/build-linux-FF1010.svg)

# JPEG-LS encoder

A light-weight JPEG-LS baseline (ITU-T T.87) grayscale image encoder in C language.

　

# Background of JPEG-LS

**JPEG-LS** (**JLS**) is a lossless/lossy image compression standard which has the best lossless compression ratio compared to PNG, Lossless-JPEG2000, Lossless-WEBP, Lossless-HEIF, etc.

The standard specification of JPEG-LS is ITU-T T.87 [1], see https://www.itu.int/rec/T-REC-T.87/en 

**JPEG-LS** uses the maximum difference between the pixels before and after compression (**NEAR** value) to control distortion, **NEAR=0** is the lossless mode; **NEAR>0** is the lossy mode, the larger the **NEAR**, the greater the distortion and the greater the compression ratio.

The file suffix name for **JPEG-LS** compressed image is .**jls** .

　

# Development progress

JPEG-LS encoder:

- [x] gray 8 bit image lossless encode (near=0)
- [x] gray 8 bit image lossy encode (near-lossless, near≥1)
- [x] gray 9-16 bit image lossless encode (near=0)
- [ ] gray 9-16 bit image lossy encode (near-lossless, near≥1)

JPEG-LS decoder:

- [ ] gray 8 bit image lossless decode (near=0)
- [ ] gray 8 bit image lossy decode (near-lossless, near≥1)
- [ ] gray 9-16 bit image lossless decode (near=0)
- [ ] gray 9-16 bit image lossy decode (near-lossless, near≥1)

　

# Code list

The code is in pure-C. See [src](./src) folder:

- `JLS.c` : Implement a JLS encoder
- `JLS.h` : Expose the JLS encoder function to users
- `JLSMain.c` : A main program with `main()` function, which call the JLS encoder to achieve image file compression.

　

# Compile

### Windows (CMD)

If you add the Microsoft C compiler (`cl. exe`) of Visual Studio to environment variables, you can compile using the command line (CMD).

Run the command in the current directory:

```bash
cl src\*.c /FeJLSencode.exe /Ox
```

We'll get the executable file `JLSencoder.exe` . Here I've compiled it for you, see `JLSencoder.exe` .

### Compile in Linux

Run the command in the current directory:

```bash
gcc src/*.c -o JLSencode -O3 -Wall
```

We'll get the binary file `JLSencoder` . Here I've compiled it for you, see `JLSencoder` .

　

# Run image compression

This program can compress `.pgm` image file to `.jls` image file.

Note that `.pgm` is a simple uncompressed image format (see PGM Image File Specification [2]). PGM file format contains:

- A simple header that contains the width, height, and depth of this image.
- The raw pixel values of this image.

### Run image compress in Windows (CMD)

Use the following command to compress a `.pgm` image to a `.jls` image.

```bash
JLSencode.exe  <input-image-file(.pgm)>  <output-file(.jls)>  [near]
```

Where `[near]` is a optional parameter of range 0\~9 :

- 0 : lossless (default)
- 1\~9 : lossy. The larger the near value, the higher distortion and the lower compressed size.

For example, the following command compress `01.pgm` in `img_kodak` folder to `01.jls` with near=1

```bash
JLSencode.exe  img_kodak\01.pgm  01.jls  1
```

### Run image compress in Linux

Use the following command to compress a `.pgm` image to a `.jls` image.

```
./JLSencode  <input-image-file(.pgm)>  <output-file(.jls)>  [near]
```

　

# Reference

[1] JPEG-LS baseline ITU-T T.87 : https://www.itu.int/rec/T-REC-T.87/en 

[2] PGM Image File Specification : https://netpbm.sourceforge.net/doc/pgm.html#index
