gorgon
======

A blender script that exports to a capnproto serialized file.


Exporting a Model from Blender
------------------------------

To export a model from blender, gorgon must first be configured as an
addon. To do this, place the gorgon folder inside your blender addons
folder. This folder can be found by going into the blender python
console (open blender and press <shift> + <f4>):

  `import sys`
  
  `print(sys.path[-1])`

My addon folder is '~/.config/blender/2.70/addons/modules'

Once that is done, open up the Addons user preference: File -> User
Preferences, select the Addons tab. Now select the User category and
you should see 'Import-Export: Gorgon Export'. Check the checkbox.

You can now export a model under the gorgon format by selecting a
single model and choosing File -> Export -> Gorgon (.grg).


Reading a Model in C++11
------------------------

In the main folder, use the Makefile to compile the example
project. This example project loads a model from a file given as the
first argument. The rendering isn't efficient but it should server as
a decent example.


Planned Features
----------------

- Textures

  We will eventually support multiple textures with texture
  coordinates.

- Animations

  We will add support for animations using either the CPU or the
  GPU. We haven't decided yet.


Known Issues
------------

- No Visual Studio support.

  We currently use Cap'n Proto. Cap'n Proto needs a compiler that
  supports C++11. The default visual studio compiler doesn't support
  enough of C++11. This should be fixed as both Cap'n Proto and Visual
  Studio improve.
