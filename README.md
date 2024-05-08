# IoT
This project is presenting communication between embedded / IoT / PC devices in small networks, connection.
The goal of this project is to implement connection library that can be use in popular connection interfaces,
such as wired interfaces: UART, SPI, I2C, CAN, ETH etc. and wireless interfaces: WiFi, BLE etc.
Project includes messaging library, and routing protocol to be able to connect device in serial mode.


For example

```
      ETH           UART          BLE
PC <------> DEVA <------> DEVB <------> DEVC
```


DEVA and DEVB should provide routing mechanism for packets from PC to DEVC, and from DEVC to PC, to able to establish communication between PC and DEVC

## Futures 
- [ ] Connection between devices
- [ ] Routing messages
- [ ] Sharing data
- [ ] Sharing files
- [ ] Sharing resources (distributed comuptation)

## Usage
Currently project support only TCP/IP based packet sharing.

### Build
```
git clone https://github.com/piotrmiszta/IoT
cd IoT
mkdir build && cd build
cmake ../
make
```

### Run
Run server: 
```
./server_out -p <port>
```

Run client
```
./client_out -i <interface> -p <port> -t <ip/id>
```

For more info look into help or documentation

### Commands

For server/client console commands see documentation (TODO)

## Author

Piotr Miszta piotr.miszta.pm@gmail.com

## License

Distributed under the MIT License. See `LICENSE.txt` for more information.
