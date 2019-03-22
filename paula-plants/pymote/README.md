# CoAPthon

[CoAPthon](https://github.com/Tanganelli/CoAPthon) is a python 2.7 library for the CoAP protocol.

## Installation
Create an virtual environment for python2.7 with:
```
$ virtualenv -p python2 venv2
```
Install CoAPthon library:
```
$ pip install CoAPthon
```

## CoAP server
The CoAP server exposes "/temp" resource in localhost, to run it:
```
$ python server.py
```

## CoAP client
The CoAP client send a GET request, then a POST for "/temp" resource:
```
$ python client.py
```

## CoAPthon

This work was done for:<br>
G.Tanganelli, C. Vallati, E.Mingozzi, "CoAPthon: Easy Development of CoAP-based IoT Applications with Python", IEEE World Forum on Internet of Things (WF-IoT 2015)

Software available at https://github.com/Tanganelli/CoAPthon

## Copper for Chrome
You can use [Copper](https://github.com/mkovatsc/Copper4Cr) to visually discover and interact with your CoAP device. You can discover the available resources on localhost:5683
