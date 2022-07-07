# Webserver

#### Capable of serving a fully static website, implementing HTTP 1.1 and CGI according to the RFC's related to the individual components of a webserver.

The project is written in C++98, according to the rules and restrictions as described by the subject-sheet provided by our school, Codam, part of the 42 Network. For anyone interested in the rules / restrictions, we've included the file named subject.pdf.

## The ✨ most interesting ✨ topics
#### Design patterns
In the design of the program, we've implemented a couple useful and interesting design patterns. Because we are not allowed to multithread the program, we've had to poll for incoming/outgoing operations to reduce the amount of cycles and system resources used by the webserver, while multiplexing and keeping the program non-blocking at all times. That includes socket, CGI and even file communication.

**Todo: Talk about the next topics:**
* Pipelining
* Cgi restrictions/design
* Design patterns

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

The structure is simply: application stores server objects, the server stores host objects, the host stores location objects.

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
| route_type | fileserver cgi redirect | fileserver | Specify how to process uri matches. |

**Host/Location:**
| Keyword | Values | Default | Description |
|---------|--------|---------|-------------|
| root			| string | root/   | Directory from where files will be searched for this location. **Note:** *if pattern routing is applied, the pattern will be suffixed to this value. That means if pattern '/images/\*' is matched, then root should be 'root/' if the intended directory equals 'root/images/*'.*  |
| autoindex		| bool | false | Enable/disable listing of files for when the uri targets a directory. |
| default_index		| string | index.html   | Name of file to return when the uri targets a directory. |
| default_error		| string | error.html   | Error page to return when no specific error page is defined. |
| accepted_methods	| GET POST DELETE | null   | Method(s) allowed on this location. |
| allow_upload		| boolean | false   | Allow/disallow uploading files in this location. |
| error_page		| [n * uint] string | null | Define specific error pages to be shown on specific error codes. Example: ```error_page 405 500 myerrorpage.html``` |

