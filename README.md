 ![language](https://img.shields.io/badge/language-C-green.svg) ![build](https://img.shields.io/badge/build-Windows-blue.svg) ![build](https://img.shields.io/badge/build-linux-FF1010.svg)

# JPEG-LS encoder

A light-weight JPEG-LS baseline (ITU-T T.87) grayscale image encoder.

C 语言实现的 JPEG-LS 灰度图像编码器。

　

# 开发进度

编码器：

- [x] 灰度 8 bit 图像无损编码 (near=0)
- [x] 灰度 8 bit 图像有损编码 (near≥1)
- [x] 灰度 9-16 bit 图像无损编码 (near=0)
- [ ] 灰度 9-16 bit 图像有损编码 (near≥1)

解码器：

- [ ] 灰度 8 bit 图像无损解码 (near=0)
- [ ] 灰度 8 bit 图像有损解码 (near≥1)
- [ ] 灰度 9-16 bit 图像无损解码 (near=0)
- [ ] 灰度 9-16 bit 图像有损解码 (near≥1)

　

# 代码说明

代码文件在目录 src 中。包括 2 个文件：

- `JLS.c` : 实现了 JPEG-LS encoder
- `JLSMain.c` : 包含 `main` 函数的文件，是调用 `JLS.c` 的一个示例。

　

# 编译

### Windows (命令行)

如果你把 Visual Studio 里的 C 编译器 (`cl.exe`) 加入了环境变量，也可以用命令行 (CMD) 进行编译。在本目录里运行命令：

```bash
cl src\*.c /FeJLSencode.exe /Ox
```

产生可执行文件 JLSencode.exe 。这里我已经编译好。

### Linux (命令行)

在本目录里运行命令：

```bash
gcc src/*.c -o JLSencode -O3 -Wall
```

产生可执行文件 JLSencode 。这里我已编译好。

　

# 运行

### Windows (命令行)

用以下命令压缩图像。其中 `<near>` 值可以取 0-9 的整数。0 代表无损，≥1 代表有损，越大则压缩率越高，图像质量越差

```bash
JLSencode.exe  <input-image-file(.pgm)>  <output-file(.jls)>  [near]
```

例如，以下命令可以把 `img_kodak` 文件夹里的 `01.pgm` 压缩为 `01.jls`  ，near=1 ：

```bash
JLSencode.exe  img_kodak\01.pgm  01.jls  1
```

> :warning: `.pgm` 是一种非压缩的灰度图像文件格式，详见 [2]

### Linux (命令行)

命令格式与 Windows 类似，把可执行文件换成 `./JLSencode` 即可。

　

# 参考资料

[1] JPEG-LS baseline ITU-T T.87 : https://www.itu.int/rec/T-REC-T.87/en 

[2] PGM Image File Specification : https://netpbm.sourceforge.net/doc/pgm.html#index
