# What is this?

A IPC test program between C and Python 

# idea

```sequence
client -> client: allocate image in share memory
client -> client: grab images to share memory directly
client -> client: fill rpc input, including image, \nimage is represented by a string, share memory name
client -> server: rpc
server -> server: get rpc input, where image buffer \nshould be get by calling share mem function 
server -> server: algorithm
server --> client: reply
```

# How to test

build client program in `??` by VS2010

start server by

`python server.py`

start client by run

`TimXmlRpc.exe`

the client program will print the test performance

```
is_even timePercallInMs 0.764000
create
BasicPythonShareMemTest timePercallInMs 0.796000
```

# Existing issues

Sometimes there is following errors

```
HttpSendRequest : Failed to connect
```



# Appendix

the HTTP message is as follows

```
<?xml version="1.0"?>
<methodCall><methodName>is_even</methodName>
<params><param><value><i4>3</i4></value></param>
</params></methodCall>


<?xml version='1.0'?>
<methodResponse>
<params>
<param>
<value><boolean>0</boolean></value>
</param>
</params>
</methodResponse>

```

