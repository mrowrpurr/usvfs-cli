# `$ usvfs.exe`

> Command Line Interface for [usvfs](https://github.com/ModOrganizer2/usvfs)

- [`$ usvfs.exe`](#-usvfsexe)
- [What?](#what)
- [Why?](#why)
- [Usage](#usage)
- [Installation](#installation)
  - [⬇️ Download](#️-download)
  - [🛠️ Build](#️-build)
  - [License](#license)
  - [Licensing notes from usvfs](#licensing-notes-from-usvfs)


# What?

Command-line interface for [USVFS](https://github.com/ModOrganizer2/usvfs) (short for User Space Virtual File System).

> `usvfs`
>
> USVFS (short for User Space Virtual File System) aims to allow windows applications to create file or directory links that are visible to only a select set of processes. It does so by using api hooking to fool file access functions into discovering/opening files that are in fact somewhere else.

> `USVFS` is released under the GNU General Public License v3.0 (_with additional permissions granted for dynamic linking and distribution_)
> https://github.com/ModOrganizer2/usvfs/blob/master/LICENSE


```
usvfs - User-Space Virtual File System, Copyright (C) Sebastian Herbord
```

# Why?

So that I can virtualize files/folders and inject them into processes from the command line.

# Usage

# Installation

## ⬇️ Download

## 🛠️ Build

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

`usvfs.exe` is licensed under the [BSD Zero Clause License](https://opensource.org/licenses/0BSD).

Use however, no attribution required.

_However,_ `usvfs` itself is licensed under the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.html) (_with additional permissions granted for_)

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
...

Additional permissions are granted under GNU GPL version 3 section 7 to Free and Open Source Software ("FOSS") without requiring that such software is covered by the GPLv3.

  1. Permission to link with the usvfs_x64.dll and/or usvfs_x86.dll

  2. Permission to distribute unmodified binary copies of the usvfs library

Theses permissions (and no other) are granted provided that the software:

  1. Is distributed under a license that satisfies the Free Software Definition (https://www.gnu.org/philosophy/free-sw.en.html) or the Open Source Definition Version (https://opensource.org/osd)

  2. Includes the copyright notice "usvfs - User-Space Virtual File System, Copyright (C) Sebastian Herbord", a copy of this license and a link to the usvfs repository in its user-interface and any user-facing documentation.

  3. Is not linked or distributed with proprietary (non-FOSS) software.

...
```
