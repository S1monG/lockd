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

# Initial example usage
0. Find the IP address of your server: `hostname -I`
1. Start the server: `make run`
2. Send a request from a device on the same network to the server: 
`curl -v -H "Authorization: securly-secured-secret-token" http://<SERVER_IP>:8888/` or 
`curl -v -H "Authorization: securly-secured-secret-token" -X POST http://<SERVER_IP>:8888/lock` or
`curl -v -H "Authorization: securly-secured-secret-token" -X POST http://<SERVER_IP>:8888/unlock`
