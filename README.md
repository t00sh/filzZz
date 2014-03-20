elfzz
====
Tool for fuzzing files

### OPTIONS

```
Usage : ./filzZz [OPTIONS] FILE COMMAND
Tool for fuzzing files

  -n, --noerr        Don't display the stderr output
  -b, --bytes        How many bytes to fuzz
  -l, --length       Fuzz length byte of file
  -o, --offset       Fuzz the file starting at offset
  -h, --help         Print help
  -v, --version      Print version
```

### EXAMPLES

Fuzz 3 bytes in range 0-512 in a.out, and execute readelf in this file

* filzZz -b 3 -l 0x200 ./a.out /usr/bin/readelf -a ./a.out


Fuzz 1 byte in range 4096-5096 in a.out, and execute objdump in this file

* filzZz -o 0x1000 -l 1000 ./a.out /usr/bin/objdump -d ./a.out


Fuzz 5 bytes in range 0-100 in a.out, execute objdump and redirect stderr to /dev/null

* filzZz -l 100 ./a.out -n /usr/bin/objdump -D -Mintel ./a.out


### AUTHORS
-TOSH- (duretsimon73 _a-t_ gmail _d-o-t_ com)