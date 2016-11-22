# Decentralized architectures & sensor networks in IoT: 6LoWPAN, border routers and gateways

## Introduction

This repository contains the code used for the
[6LoWPAN](https://www.eventbrite.com/e/decentralized-architectures-sensor-networks-in-iot-6lowpan-border-routers-and-gateways-tickets-28534379096)
workshop [relayr](https://).

## Requirements

The requirements willvary depending if you are in Linux, Mac or
Windows.

### Linux setup

For [Debian](http://www.debian.org) based systems (Debian, Ubuntu,
Mint, etc.) these are the steps.

 1. Get the basic tools for C based development:
 ```shell
 apt-get install build-essential automake gettext
 ```
 2. Get the compiler and supporting libraries:
 ```shell
 apt-get install gcc-arm-none-eabi libnewlib-arm-none-eabi
 
 ```
  3. Get git:
 ```shell
 apt-get git
 
 ```
  4. See if have permissions to access serial devices:
 ```shell
 groups | grep dialout
 ``` 
    if the `dialout` groups appears then you are already in the group,
    if nothing is printed, this means that you need to add yourself to
    the group.
 ```shell
 sudo adduser <your username> dialout
 ```
    where `<username>` is your username. You have to **logout** and
    **login** again for the change to take effect.
    
  5. Done. 
  
  
### Mac Os X Setup
  
 1. The first thing you'll need is the `gcc-arm-embedded` compiler.
    There are two possibilities for getting the compiler:
 
    * Using [brew](http://brew.sh/). 
    
     1. Add the brew repository for the `gcc-arm-embedded` compiler. 
    
        ```shell
        brew tap PX4/homebrew-px4
        brew update
        ```
     2. Install the gcc-arm-embedded` compiler.
        ```shell
        brew install gcc-arm-none-eabi
 
        ```
    * Installing the compiler without brew.
    
     1. Go to [https://launchpad.net/gcc-arm-embedded](https://launchpad.net/gcc-arm-embedded)
        and download the **Mac installation tarball**.
    
     2. Make a directory to install it. Is up to you to decide how to 
        organize your filesystem. Our suggestion is to put it under
        `/usr/local`:
        
        ```shell
        mkdir /usr/local/gcc_arm
        ```
     3. Unzip the tarball downloaded in 1. and move it to the above
        created directory.
    
     4. Add the path to the compiler to your `PATH` shell variable so that
        you are able to compile code using the ARM GCC in any directory.
        ```shell
        export PATH="$PATH:/usr/local/gcc_arm/gcc-arm-none-eabi-<version>/bin"
 
        ```
        where `<version>` is the version of ARM GCC you downloaded in 1. 
  
 2. Now you should be able to from **any** directory do:
    ```shell
    arm-none-eabi-gcc --version
 
    ```
    and get the version of the ARM GCC.
   
 3. Note that depending on your particular setup you might have
    already an ARM GCC, it might work with the workshop code or not,
    so we advise to always update ARM GCC to the latest available
    version.
  
 4. Get the
    [driver](http://www.silabs.com/products/mcu/pages/usbtouartbridgevcpdrivers.aspx)
    for the USB to TTL adapter.
 
 5. In Mac you should already have Python available, to make it easier
    to get new packages we'll use `pip. We need the
    [pySerial](https://pyserial.readthedocs.io/en/latest/) module.
    ```shell
    sudo pip install pyserial 
 
    ```
 6. As a final step get [XCode](https://developer.apple.com/xcode/) so
    that we have a properly setup development environment (`git`, `make`,
    other `gcc`, etc).
    
 7. Done. 
  
### Windows setup
 
 To make things work in Windows you'll need to install the **M**inimal
 **G**nu **W**indows environment (MinGW). If you already have it in
 your system then you don't need to set it up.
 
  1. Go to [mingw.org](http://mingw.org/) and download the
     **installer**.
     
  2. Follow the instructions to install MinGW.
  
  3. Get the Windows version of the gcc-arm-embedded` compiler at
     [https://launchpad.net/gcc-arm-embedded](https://launchpad.net/gcc-arm-embedded).
   
  4. If you don't have Python you need to get it from the
     [Python](https://www.python.org/downloads/windows/) site. 
     
  5. We need the
    [pySerial](https://pyserial.readthedocs.io/en/latest/) module.
    ```shell
    pip install pyserial 
    ```
  6. Install the
     [Git for Windows SDK](https://github.com/git-for-windows/build-extra/releases/).
  
  7. Done.
  
  
## Getting the code on to the board

The repository uses
[git submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules)
in order to include the
[Contiki repository](https://github.com/contiki-os/contiki) inside.

 1. Clone the repository:
    ```shell
    git clone --recursive git@github.com:relayr/workshop-6lowpan.git
 
    ```
 
 2. Go to the workshop directory:
    ```shell
    cd workshop
 
    ```
 3. Build the executable:
    ```shell
    make TARGET=zoul BOARD=firefly
 
    ```
 4. Connect the board to the USB of your computer.
 
 5. Upload the code to the board:
    ```shell
    make TARGET=zoul BOARD=firefly er-server.upload
    ```

## Connecting the sensor
    
 1. Connect the [HIH 6130](https://www.sparkfun.com/products/11295)
    sensor breakiout board to the Firefly.
 
## TODO 

 1. Provide the schematic using Fritzing.
 2. Add compiling instructions.
 3. Add networking part to documentation. With and without edege
    router.
 
