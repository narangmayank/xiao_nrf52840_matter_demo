## Setup OTBR (Open Thread Bourder Router)

Let's Create a Thread network using OTBR host on Linux (Ubuntu recommended) with nRF52840 DK as the Thread radio co-processor

### Prepare the hardware

We just need to connect nRF52840 DK hardware to the linux PC. Use Segger J-Link USB port and you're done.

### Flash the firmware

**Using pre-compiled binary (Recommended for easy start)**

```bash
# Flash hex file using nrfjprog
$ nrfjprog -f nrf52 --program hex\coprocessor_merged.hex --sectorerase

# Reset to apply the new FW
$ nrfjprog --reset
```

OR

**Generating binary (Using West)**

```bash
# Step into coprocessor project
$ cd coprocessor

#  Build the coprocessor sample
$ west build -p -b  nrf52840dk/nrf52840

# Flash the binary
$ west flash --erase
```

### Disable J-LINK MSD Operations

Open J-Link Commander and disable the mass storage device feature, You can enable it later anytime

```bash
# Disable JLINK mass storage
$ MSDDisable

# Force Hardware Flow Control to avoid potential race conditions
$ SetHWFC Force
```

Now thread radio hardware is ready, you can follow below instructions to setup the OTBR host.

### Configure the Border Router

``` bash
# Install pre-requisites docker
sudo apt update && sudo apt install docker.io

# Start docker
sudo systemctl start docker

# Create a ipv6 network for OTBR contianer
sudo docker network create --ipv6 --subnet fd11:db8:1::/64 -o com.docker.network.bridge.name=otbr0 otbr

# Get the otbr container 
docker pull nrfconnect/otbr:fbde28a

# filter out ipv6 table
sudo modprobe ip6table_filter

# Run the docker container 
sudo docker run -it --rm --privileged --name otbr --network otbr -p 8080:80 \
--sysctl "net.ipv6.conf.all.disable_ipv6=0 net.ipv4.conf.all.forwarding=1 net.ipv6.conf.all.forwarding=1" \
--volume /dev/ttyACM0:/dev/radio nrfconnect/otbr:fbde28a --radio-url spinel+hdlc+uart:///dev/radio?uart-baudrate=1000000

# Route the messages over Ipv6
sudo ip -6 route add fd11:22::/64 dev otbr0 via fd11:db8:1::2
```

### Forming Thread Network

Now the OTBR container is running, you should be able to access to its user interface by searching on a web browser `localhost:8080`. To form the thread network go to the Forms menu and click the Form button

You should see Form operation is successfull.


### Check OTBR Running Status

Finally hread network is created you can check the running status anytime using below commands,

```bash
# Check the OTBR running status
sudo docker exec -it otbr sh -c "sudo service otbr-agent status"

# Check the thread node status
sudo docker exec -it otbr sh -c "sudo ot-ctl state"
```

### Collect Network Operational Dataset

Now that your thread network is up and running and you need collect the network operational dataset hex value (i.e We will be using this during device commisioning)

```bash
# Get the thread operational dataset hex
sudo docker exec -it otbr sh -c "sudo ot-ctl dataset active -x"
```