# 42_KFS

## Features
- **Modular Architecture:** The project is organized into multiple kernel versions, each implementing different functionalities or optimizations.
- **x86 Architecture:** The kernel is designed to run on the x86 architecture, targeting the i386 instruction set.
- **GRUB Bootloader Integration:** The kernels are bootable using the GRUB bootloader, making them suitable for testing on real hardware or virtual machines.
- **Assembly and C Programming:** KFS utilizes both assembly language and C programming for implementing low-level functionalities and higher-level features.

## Getting Started
Follow these instructions to get a copy of the project up and running on your local machine.

### Prerequisites
- QEMU emulator
- NASM assembler
- GNU Compiler Collection (GCC)
- GRUB bootloader utilities (grub-mkrescue, grub-file)
- Xorriso

Installing Prerequisites on Debian-Based systems
```bash
sudo apt-get install qemu nasm gcc grub2-common xorriso
```

Installing Prerequisites on Arch-Based systems
```bash
sudo pacman -S qemu nasm gcc grub xorriso
```

With these tools installed, you can simply run make to build the project, and everything should work seamlessly.

```bash
make # make all the KFSs

make KFS-1 # only make KFS1

make iso # only make the iso

make run # execute qemu booting all KFSs

make re # fclean and compile

make clean # clean only objs

make fclean # clean + clean iso and bin
```