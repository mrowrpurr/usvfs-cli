# `$ usvfs.exe`

> Command Line Interface for [usvfs](https://github.com/ModOrganizer2/usvfs)

- [`$ usvfs.exe`](#-usvfsexe)
- [What?](#what)
- [Why?](#why)
- [Installation](#installation)
- [Usage](#usage)
  - [Command Line Interface](#command-line-interface)
    - [Usage examples](#usage-examples)
    - [Running processes via the CLI](#running-processes-via-the-cli)
    - [Starting a websocket server via the CLI](#starting-a-websocket-server-via-the-cli)
  - [WebSocket Interface](#websocket-interface)
- [Building](#building)
  - [License](#license)
  - [Licensing notes from usvfs](#licensing-notes-from-usvfs)


# What?

...

# Why?

...

# Installation

...

# Usage

...

## Command Line Interface

The command-line interface can be used to either:
- (a) run one or more processes with `usvfs` injected for virtual file system access
- (b) start a websocket server

### Usage examples

```sh
# Run a single process with usvfs injected
usvfs.exe \
   --process '{"path":"C:/path/to/program.exe"}' \
   --link-on-create '{"source":"C:/actual/path","overwrite":"C:/overwrite-folder"}' \
   --link-dir '{"source":"C:/actual/path","target":"C:/virtual/path"}' \
   --link-file '{"source":"C:/actual/file.txt","target":"C:/virtual/file.txt"}'
```

### Running processes via the CLI

You can run one or more processes with `usvfs` injected for virtual file system access.

For each process you want to run:

```sh
usvfs.exe --process '{"path":"C:/path/to/program.exe"}'
```

### Starting a websocket server via the CLI


## WebSocket Interface

...

# Building

These instructions are for building `usvfs.exe` on Windows.

> _If you have instructions for building on another platform, let me know and we can share_
> _the instructions here in the README._

1. Download and install the precompiled Boost 1.79.0 binaries for Windows:
   > https://sourceforge.net/projects/boost/files/boost-binaries/1.79.0/boost_1_79_0-msvc-14.3-64.exe/download
   >
   > _Note the directory that you install to!_

2. Download `usvfs` source code from https://github.com/ModOrganizer2/usvfs.git
   > e.g. `git clone --recurse-submodules https://github.com/ModOrganizer2/usvfs.git`

3. Build `usvfs` using Visual Studio (_open the `usvfs\vsbuild\usvfs.sln` solution_)
   > In Visual Studio, select `Release` and `x64` and build the solution.
   >
   > It will fail due to missing `boost` headers and libraries, but that's okay.
   >
   > Go to the "Property Manager" and, for each project which failed to build, right-click and select "Properties".
   >
   > In the "VC++ Directories" section, add the path to the `boost` headers and libraries.

4. Set the `USVFS_ROOT` environment variable to the directory where you cloned `usvfs`
   > e.g. `C:\path\to\usvfs`

5. Build using `xmake`

## License

Use however, no attribution required.

```
BSD Zero Clause License (SPDX: 0BSD)

Permission to use, copy, modify, and/or distribute this software for any purpose
with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
THIS SOFTWARE.

```

## Licensing notes from usvfs

https://github.com/ModOrganizer2/usvfs/blob/master/LICENSE

```
Additional permissions are granted under GNU GPL version 3 section 7 to Free and Open Source Software ("FOSS") without requiring that such software is covered by the GPLv3.

  1. Permission to link with the usvfs_x64.dll and/or usvfs_x86.dll

  2. Permission to distribute unmodified binary copies of the usvfs library
```