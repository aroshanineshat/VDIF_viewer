# VDIF_viewer
A tool to inspect VDIF files

# Todo list
TODO: implement libzmq
TODO: parse data if required
TODO: simple GUI
TODO: load large VDIF files

# Compile

simply run 
`g++ src/vdif_viewer.cpp src/vdif_cli.cpp -o vdif_viewer`

and then provide the vdif file through command line:
`./vdif_viewer ~/cool_things.vdif`