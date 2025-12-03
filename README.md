# XIAO nRF52840 Matter Demo

Run a Matter light bulb application on XIAO nRF52840 with Thread connectivity via an OpenThread Border Router (OTBR).

## Prerequisites

* **Hardware**

    * USB Type C Cable
    * Micro USB Cable
    * XIAO nRF52840 Or XIAO nRF52840 Sense board
    * nnRF52840 DK (i.e Thread radio coprocessor)
    * [Seeed Studio Grove Base for XIAO](https://www.seeedstudio.com/Grove-Shield-for-Seeeduino-XIAO-p-4621.html)
    * [Grove - RGB LED Ring (20 - WS2813 Mini)
](https://www.seeedstudio.com/Grove-RGB-LED-Ring-20-WS2813-Mini.html)

* **Hardware**

    * Linux OS (preferably Ubuntu)
    * nRF Connect SDK v2.9.0  (You can download it via VS Code Extension)
    * Segger J-Link v7.94i or higher

You are welcome to follow Nordic's [instructions](https://www.nordicsemi.com/Products/Development-software/nrf-connect-sdk) for setting up the nRF Connect SDK

## Project Setup 

```bash
# Get the xiao_nrf52840 matter_demo
$ git clone https://github.com/narangmayank/xiao_nrf52840_matter_demo

# Step into the project directory
$ cd xiao_nrf52840_matter_demo
```

Lets get started now :)

## 1. Matter Light Node

Here I'm using for XIAO nRF52840 Sense for demo. In case you are using XIAO nRF52840 change the target accordingly. 

### Prepare the hardware

This is pretty simple, attach the XIAO nRF52840 Board to grove base board and connect the grove RGB LED ring to D2 pin. Once done, your hardware setup will look like below,

### Flash the firmware

**Using pre-compiled binary**

```bash
# Flash hex file using nrfjprog (Use hex\xiao_nrf52840_sense_light_bulb_merged.hex if using sense board)
$ nrfjprog -f nrf52 --program hex\xiao_nrf52840_light_bulb_merged.hex --sectorerase

# Reset to apply the new FW
$ nrfjprog --reset
```

OR

**Generating binary (Using West)**

```bash
# Step into matter light node project
$ cd xiao_light_bulb

#  Build the light bulb sample (change target to xiao_ble/nrf52840/sense if using sense board)
$ west build -p -b xiao_ble/nrf52840

# Flash the binary
$ west flash --erase
```

Once done, You should see XIAO inbuilt RGB light blinking (BLUE) which means matter light node firmware is running now, started bluetooth advertising by `Matter_Light` name and waiting for commisioning.

## 2. Setup OTBR (OpenThread Border Router)

Please follow the instructions in [OTBR_SETUP.md](./OTBR_Setup.md) to set up the OpenThread Border Router (OTBR) and create the thread network on your linux machine.

Once OTBR is running and the Thread network is established, proceed with the matter controller node setup below to commission and control the Matter light node.

## 3. Setup Matter Controller Node

We will be using matter chip tool as a matter controller node to run this demo.

### Get the Chip tool Binary

Download the latest chip tool binary for linux from the below github
https://github.com/nrfconnect/sdk-connectedhomeip/releases

Give executbale permision via `chmod +x ./chip-tool_x64`

Usage `./chip-tool_x64`

### Commission the Light Node

Before running any Matter commands, you must **commission the node to the thread network**. Use the following command to commission your node

**Notes on parameters:**

* `node_id` is user-configurable and you can use any unique integer to identify your device on the network (e.g., 1, 2, etc.)
* `operational_dataset_hex` is obtained from the **OTBR Setup**
* `pincode` is  specific to the node firmware. For the Matter Light Bulb node, the default is **20202021**
* `discriminator` is specific to the node firmware. For the Matter Light Bulb node, the default is **3840**

```bash
# Matter thread node commissioning via BLE
./chip-tool_x64 pairing ble-thread <node_id> hex:<operational_dataset_hex> <pincode> <discriminator>
```

### Control the Light Node

Once the node is commissioned, you can control it using standard Matter OnOff cluster commands

**Notes on parameters:**

- `node_id` is assigned during commissioning  
- `endpoint_id` is specific to the node firmware. For the Matter Light Bulb node, the default is **1**.

```bash
# Turn ON LED
./chip-tool_x64 onoff on <node_id> <endpoint_id>

# Turn off LED
./chip-tool_x64 onoff off <node_id> <endpoint_id>

# Toggle the LED state
./chip-tool_x64 onoff toggle <node_id> <endpoint_id>
```