# Decentralized architectures & sensor networks in IoT: 6LoWPAN, border routers and gateways

## Introduction

Below you can find general info about setting, flashing, using and accessing the CoAP server and client which are implemented for use in this workshop

## Requirements

Repositories and libraries must have been set and installed as described [here](https://github.com/relayr/workshop-6lowpan/blob/workshop/README.md).

## Er-server.c

This CoAP server allows you to remotely access its resources using REST requests. By default the server has four main paths:
    
 1. **.well-known/core** Access with a GET request. Here all of the mote's resources are advertized
    
 2. **actuators/** Usually accessed with POST or PUT requests. Under this path you can find resourses which use the GPIO pins or the LEDs of the device to implement any kind of actuation.

 3. **sensors/** Use GET requests to retrieve sensor readings from resources under the sensor path. 

 4. **observables/** Use CoAP OBSERVE requests to subscribe to observable resources and get a notification if a condition is satisfied.

### Activating resources
 
 You can activate or deactivate resources simply by including a function, which binds them with a unique path. Do not forget to also activate any hardware a resource might need. The activation is done just after the REST engine is initialized in the PROCESS: 
 
 ```c
   /* Initialize the REST engine. */
  rest_init_engine();

  /* Set our resources paths and also activate the sensor. */
  
  rest_activate_resource(&res_hih6130_temp, "sensors/hih6130/temperature");
  rest_activate_resource(&res_hih6130_hum, "sensors/hih6130/humidity");
  
  /* Activating the HIH6130 sensor. */
  SENSORS_ACTIVATE(hih6130);
 
 ```
### Flashing the server on a Firefly mote

In the *workshop/* directory execute:
   
```shell
make TARGET=zoul BOARD=firefly er-server.upload
```
Now the mote is accessible from its hard-coded IPv6 address.
 
 
 
##Er-client.c
 
 The CoAP client is used for sending data to the [relayr.](https://developer.relayr.io) cloud. A loop is triggered periodically and a CoAP message is sent to a predefined server address. In the given example, the client probes the [HIH6130](https://www.sparkfun.com/products/11295) sensor and sends two messages (temperature and humidity) to a remote CoAP server, which is responsible for uploading the data to the cloud.
 
### Setting up the client
 Inside the code many variables must be set before flashing the code:

 1. Begin with setting the border router address (default [fd00::1]), the CoAP port the server is listening to (default 5683) as well as the path, where the request should be done:

 ```c
/* Define the border router settings. */
#define SERVER_NODE(ipaddr)   uip_ip6addr(ipaddr, 0xfd00, 0, 0, 0, 0, 0, 0, 0x1)
#define REMOTE_PORT     UIP_HTONS(8181)
#define URL_PATH "/target"
 ```

 2. Next, specify the credentials in order to successfully connect to the cloud, and choose the time interval in seconds, for probing the data: 

 ```c
	/* Interval for setting data to the server. */
	#define TOGGLE_INTERVAL 5

	/* Path info and cloud credentials. */
	#define DEVICE_ID "42beac85-ec65-4e3ee-9b7a-df4ffa85b17d"
	#define USER_TOKEN "Bearer nlm5mMJNTu6N7mm6Q42lKnS5QNf11WrrNhgRku" 
 ```

###Flashing the client on a Firefly mote

Again, in the *workshop/* directory, execute:

```shell
make TARGET=zoul BOARD=firefly er-client.upload
```
   
##References

More info on [CoAP](http://coap.technology), [zolertia firefly](https://github.com/Zolertia/Resources/wiki/Firefly) and [relayr. cloud](http://docs.relayr.io).

The examples make use of the [Erbium REST engine](http://people.inf.ethz.ch/mkovatsc/erbium.php).

 