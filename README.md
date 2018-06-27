# LC3 Disassembler

#### Introduction 

This is a basic disassembler for LC3 assembly with support for symbols.
This program takes an assembled LC3 binary file and can print out the instructions in:
* Hexadecimal notation
* Binary notation
* Assembly with symbols OR without symbols
  * Offsets are given as a offset from the current PC value if no symbols are provided.
* This program can optionally print each notation with memory addresses listed before each instruction.

Usage: lc3disas [OPTION] [.obj file path] [.sym file path (optional)]
       -h, --hex		Prints out instructions in hexadecimal format (like a hex dump).
       -b, --bin		Prints out instructions in binary format.
       -a, --asm		Prints out instructions in assembly format.
       -hi, --hex-indexed	Prints out instructions in hexadecimal format with memory indices.
       -bi, --bin-indexed	Prints out instructions in binary format with memory indices.
       -ai, --asm-indexed	Prints out instructions in assembly format with memory indices.

I based this disassembler off of the LC3 compiler given by McGraw Hill. You can get the tools [here.](https://highered.mheducation.com/sites/0072467509/student_view0/lc-3_simulator.html)
#### Installation

This program was built on Ubuntu 18.04 with GCC 7.3.0. It should work on Mac OS, though I have no way to test this.
For Windows users, I strongly recommend you use the Windows Subsystem for Linux and install Ubuntu from the Windows Store.

For dependencies, all you really need is a C99 compliant compiler (GCC or clang will work).
To compile the file, either use Make or compile the source files manually. 
Ubuntu:
```
sudo apt install make gcc git build-essential
```
For Mac OS, install the Xcode command line tools for Make.
Then, compile and install the program:
```
make
sudo make install
```
By default this installs to /usr/local/bin. If you want to remove the program, type
```
sudo make uninstall
```
Instead of installing the program, you can also call this program anywhere by exporting the program directory to your PATH after compiling:
```Bash
cd <lc3disas folder path>
echo "export PATH=$(pwd):\$PATH" > ~/.bashrc  # Bash
echo "export PATH=$(pwd):\$PATH" > ~/.zshrc   # Zsh 
```
If you want this program system-wide, copy the executable to /usr/bin.


Feel free to improve the code I've written here; this was done as a little project on the side for fun.

