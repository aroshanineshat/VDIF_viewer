# VDIF viewer

A tool to inspect VDIF files with standard 32 bytes headers.

# Todo list

TODO: implement libzmq and parse streams rather than files

TODO: proess payload

TODO: simple GUI

TODO: load large VDIF files

# Compile

simply run 

`cmake . && make` 

and then provide the vdif file through command line:

`./vdif_viewer ~/cool_things.vdif`

# Sample output (for now)

The first draft of the software loads the whole vdif file into memory 
and then explores the vdif frames. 

It prints each frame and the header words individually:

```
Frame X ====================================
Word 0:
Word 1:
Word 2:
Word 3:
Word 4:
Word 5:
Word 6:
Word 7:
```
