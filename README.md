# Webserver


# Configuration file
The configuration file consists of a 4 different classes:
* application
* server
* host
* location

The structure is simply: application stores server objects, the server stores host objects, the host stores location objects.

The root of the file writes into the Application object while the others are initiated by writing their name (in lowercase)
and then opening / closing brackets to define their region.

server
{
	host
	{
		location ...
		{
		}
		...
	}
	...
}

---
permalink: /making-readmes-readable/
title: Making READMEs readable
---

Available settings:
**Application:**
* listen_backlog		-->		backlog amount for the listen socket
* buffer_size			-->		the size in bytes for I/O buffering (recommended to be at least 4k bytes)
* mime_config			-->		path to the mime config file
* max_requestbody_size	-->		maximum of bytes clients can send within their body

**Server:**
* host					-->		define a host child for this server (at least one should be defined)
* port					-->		the port on which this server will be running

**Host/Location:**
* autoindex
* root
* default_index
* default_error
* accepted_methods
* allow_upload
* error_page
location
name
