# lockd
TODO

# Prerequisites
- libgpiod (for GPIO control): 
This project is using libgpiod version 2.x.x. It is not available with `apt` as of now but you can built it from source. 
You can find the source code and more information here: https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/
When it is available through `apt`, you can install it with: `sudo apt install libgpiod-dev`
- libmicrohttpd (for HTTP server): `sudo apt install libmicrohttpd-dev` 

# Build and run
`make`        # build

`make run`    # build then run

`make clean`  # remove build/ and bin/

`make test`   # run tests
