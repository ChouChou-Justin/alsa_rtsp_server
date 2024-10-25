# ALSA RTSP Server

This project implements an RTSP server that captures video from a ALSA (Advanced Linux Sound Architecture) device and streams it using the RTSP protocol. It utilizes the Live555 library for RTSP streaming capabilities.

## Features

- Captures audio from ALSA devices
- Streams audio over RTSP 
- Configurable audio parameters
- Based on Live555 for robust RTSP implementation

## Dependencies

- CMake (version 3.10 or higher)
- Live555 library
- ALSA development libraries (libasound2-dev)
- C++ compiler with C++11 support

## Building the Project

1. Clone the repository:  
   ```
   git clone https://github.com/ChouChou-Justin/alsa_rtsp_server.git
   cd alsa_rtsp_server
   ```

2. Create a build directory and navigate to it:  
   ```
   mkdir build
   cd build   
   ```

3. Run CMake and build the project:   
   ```
   cmake ..
   make   
   ```

## Usage

After building the project, you can run the server with:
    ```
    ./alsa_rtsp_server
    ```

