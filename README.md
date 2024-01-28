# TCPServer
This is a TCP/IP server, designed to be flexible in usage for end edge programmer. It includes simple server base, on the basis of which you can develop you own server, or use already developed interface.


# How to

## develop your own server
- If you want to develop your own type of server on scratch, you`ll need just to use ITCPServer abstact class. It gives you almost nothing, so why are you going to do this???
- If you want to develop your own TCP server with additional functions, all you need is to use TCPServerBase as parent class. It implements basics function of creating and connecting sockets, so you need only create you own specific methods.

## use TCPServerBase
TCPServerBase offers simple interface to interract with incoming connections. It create, bind socket and accept connections by itself, and handle them by your own developed handler. All you need to do is :
- develop Handler class, which is a child class of IConnectionhandler, in which you write server logic.
- Create an object of server by one of the instance methods (in which you set up your address or server will try to find it by itself, but it dont recommended).
- Set your handler in server.
- Use start method.

That is all. After what you will get working server with simplified console. It`s commands is:
- stop - it speaks for itself.
- log - show all accepted connections.
- state - show the state of the server.

Also in src folder you can find simple HTTP handler. It get, parse and send via simple interface - all you need to do is get HTTPreq struct and give HTTPans struct.

You can see examples on this [page](https://github.com/MeDGranD/TCPServer/tree/main/example).

# Requirements
TCPServerBase was develop on and for *nix system, so it is the main requirement. But it`s interfaces still can be used for another system, but implementaion will laid on the programmer.
