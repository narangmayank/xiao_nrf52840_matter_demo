# XIAO nRF52840 Matter Demo

This repo contains everything you need to get started with matter on XIAO nRf52840.
Lets get started quickly,

- **xiao_nrf52840_light_bulb**: Matter Light bulb FW for XIAO nRf52840 board

- **coprocessor**: Thread radio coprocessor FW for nRF52840DK. Follow instrcution below to run the OTBR

After flashing the FWs, please follow the below instructions to run this demo -:

## Open Thread Bourder Router

``` bash
# Install docker
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

## Checking OTBR Status

```bash
# Check the OTBR running status
sudo docker exec -it otbr sh -c "sudo service otbr-agent status"

# Check the thread node status inside docker
sudo docker exec -it otbr sh -c "sudo ot-ctl state"

# Get the thread operational dataset hex code
sudo docker exec -it otbr sh -c "sudo ot-ctl dataset active -x"
```

## Get the Chip tool Binary

Download the latest chip tool binary for linux from the below github
https://github.com/nrfconnect/sdk-connectedhomeip/releases

Give executbale permision via `chmod +x ./chip-tool_x64`

Check usage via `./chip-tool_x64`

## Matter Commands 

```bash
# Matter node commissioning
./chip-tool_x64 pairing ble-thread <node_id> hex:<operational_dataset_hex> <pincode> <discriminator>

# Turn ON LED
./chip-tool_x64 onoff on <node_id> <endpoint_id>

# Turn off LED
./chip-tool_x64 onoff off <node_id> <endpoint_id>

# Toggle the LED state
./chip-tool_x64 onoff toggle <node_id> <endpoint_id>
```