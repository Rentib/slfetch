slfetch - simple linux (or suckless) fetch
==========================================
slfetch is extremly fast, small, and neofetch-like looking command-line system 
information tool written in <code>C</code>

Requirements
------------
In order to build slfetch you need <code>C99</code>.

Installation
-----------
Edit config.mk to match your local setup
(by default slfetch is installed into the /usr/local namespace)

Afterwards enter the following command to build and install slfetch
(probably it will require root permisions):
  
    make clean install

Running slfetch
---------------
Just type:

    slfetch

and it will give you your info.

Customization
-------------
In order to customize and configure slfetch just change *config.h*,
or other files.

* packages - change *packages* function in *slfetch.c*, maybe I will add it to *config.h*
* colors - colors in slfetch are done using [printf](http://andrewnoske.com/wiki/Bash_-_adding_color),
  you have them listed in *colors.h*

Download
--------
You can [browse](https://github.com/Rentib/slfetch) the source code repository on GitHub or get a copy using git with the following command:

    $ git clone https://github.com/Rentib/slfetch

