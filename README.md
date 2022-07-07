# Webserver

#### Capable of serving a fully static website, implementing HTTP 1.1 and CGI according to the RFC's related to the individual components of a webserver.

The project is written in C++98, according to the rules and restrictions as described by the subject-sheet provided by our school, Codam, part of the 42 Network. For anyone interested in the rules / restrictions, we've included the file named subject.pdf.

## The ✨ most interesting ✨ topics
#### Observer pattern
In the design of the program, we've implemented a couple useful and interesting design patterns. Because we are not allowed to multithread the program, but it should still always be non-blocking, we've had to poll for incoming / outgoing operations to reduce the amount of cycles and system resources used by the webserver. To solve this in a clean way, we've implemented observer pattern, where ```IPollable``` can subscribe to / unsubscribe from ```PollHandler``` which is the only object concerned with poll. ```PollHandler``` will in order fire functions on ```IPollable```'s implementor whenever neccesary. This approach encapsulates poll very well. Timeout-handling for example for CGI and Clients, is implemented using the same strategy.

#### Composite pattern (see [default.config](https://github.com/Ryno95/webserv/blob/main/server/config/default.config) for clarification about the structure)
The configuration file parser has become a top-down parser, where basicly every layer in the configuration file is represented by an instance of a class. Every 'branch' class can have children and a 'leaf' class can obviously not (more on this in the section [Configuration File](#configuration-file)). Every class in the hierarchy has a set of keywords and commands prepared in a map, which will fire whenever the keyword is encountered in the configuration file. Because we used an interface for commands, we can run callback functions in a generic way. Variables can be parsed to their expected type, for example, or child nodes can be instantiated. See [this example](https://github.com/Ryno95/webserv/blob/main/src/srcs/config/HostConfigParser.cpp).

#### [HTTP Pipelining](https://en.wikipedia.org/wiki/HTTP_pipelining)
Is implemented for fun, because it seemed like a cool and fun challenge to implement. It's the reason why we created small state-machines for receiving / sending operations and we've had to create queue's for requests / responses so it remains FIFO. And pipelining is very efficient. Who doens't like efficiency?

## Testing strategy
The very first thing we did when starting the project, was setting up the testing environment. For this we attached continuous-integration by CircleCI to Github, set up criterion as our unit-testing framework and (progressively) created Python scripts for runtime tests with the server. (Doing requests, comparing results)

## Compiling and running the program
Short answer: ```./run.sh```.

As described in ```run.sh```:
1. ```make -C src/ -j5``` builds a library and an executable. The library is also used by the ```tests/``` directory, and therefore contains all object files, except ```main.o```.
2. ```cp src/webserv server/``` move the executable to a directory containing your configuration and server files.
3. ```cd server/``` change directory to the root of ```webserv``` executable, so the program can find the config files it uses by default.
4. ```./webserv [optional path to configuration file]```

## Configuration file
The configuration file consists of a 4 different classes:
* application
* server
* host
* location

The structure is as follows: application stores server objects, server stores host objects, host stores location objects.

The root of the file writes into the Application object while the other classes are instantiated by writing their name (in lowercase)
and then opening / closing brackets to define their region.
**Host and Location share most keywords, as the host sets them as default values and the location can override the host's values for it's specific route.**

**Application:**
| Keyword | Values | Default | Description |
|---------|--------|---------|-------------|
| server		| none | none | Define the beginning of a server object. |
| listen_backlog	| uint | 42 | Backlog amount for the listen sockets. |
| buffer_size		| uint | 4096 | Size in bytes for I/O buffering. (higher = less cycles per I/O operation) |
| mime_config		| string | mime.config | Path to the mime config file. |
| max_requestbody_size	| uint | 40960 | Maximum of bytes clients can send within their body. |

**Server:**
| Keyword | Values | Default | Description |
|---------|--------|---------|-------------|
| host			| none | none | Define the beginning of a host object. |
| port			| uint | 80   | Port on which this server will be running. |

**Host:**
| Keyword | Values | Default | Description |
|---------|--------|---------|-------------|
| location		| string | null   | Defines a specific location object with a pattern to match with routing and custom settings. Wildcard's can be used in the pattern. Example: ```location /images/*.jpg``` |
| name			| strings | default | Hostname of this host. Used in routing by matching 'Host' headerfield with this value. |

**Location:**
| Keyword | Values | Default | Description |
|---------|--------|---------|-------------|
| route_type | "fileserver" / "cgi" / "redirect" | fileserver | Specify how to process uri matches (specify one). |

**Host/Location:**
| Keyword | Values | Default | Description |
|---------|--------|---------|-------------|
| root			| string | root/   | Directory from where files will be searched for this location. **Note:** *if pattern routing is applied, the pattern will be suffixed to this value. That means if pattern '/images/\*' is matched, then root should be 'root/' if the intended directory equals 'root/images/*'.*  |
| autoindex		| bool | false | Enable/disable listing of files for when the uri targets a directory. |
| default_index		| string | index.html   | Name of file to return when the uri targets a directory. |
| default_error		| string | error.html   | Error page to return when no specific error page is defined. |
| accepted_methods	| "GET" "POST" "DELETE" | null   | Method(s) allowed on this location (specify any). |
| allow_upload		| boolean | false   | Allow/disallow uploading files in this location. |
| error_page		| [n * uint] string | null | Define specific error pages to be shown on specific error codes. Example: ```error_page 405 500 myerrorpage.html``` |

