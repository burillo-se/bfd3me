# bfd3me
Kit, kitpiece and preset metadata and database editor for BFD3

This is a cross-platform (in theory) app to edit BFD3 metadata for kits, kitpieces and presets, both in form of individual files (i.e. a single preset) and directly editing BFD3 database files.

For kitpieces, the following pieces of metadata are supported:
- Name (i.e. "Sabian Uber Hat 100500 extra VIP series")
- Library long name ("BFD Heavy")
- Library short name ("HVY")
- Class ("Hihat")
- Subclass (usually empty for hats, but can be "Floor" for toms, for example)
- Manufacturer ("Sabian")
- Model ("Uber Hat 100500")
- Dimensions ("25'")
- Date ("Late 1800's")
- Beater ("Stick")

For Kits and Presets, only Name and Library names are supported.

When in database mode, considerable amounts of memory may be consumed (my kitpiece database usually eats up to 100MB of RAM). This is because when in database mode, the complete XML file is held in memory. If i had implemented *complete* support for *all* BFD3 metadata then it wouldn't be necessary as i could simply generate XML files on the fly, but i don't have neither time nor desire to fix this.

The individual files mode doesn't suffer from this (well, it did, initially, but not any more) because, unlike database mode, we can *know* where each XML node came from (we store paths to each XML file), and we know that there is only one XML node available in each file, so when saving changes, instead of simply saving XML like the database mode does, we can load the XML, edit necessary XML attributes and save it back. Neat, huh?

I tried to keep the source code as tidy as possible, but obviously my dreams of perfect architecture crashed and burned when met with reality. I still try to compartmentalize the most horrible things, and i think i've done a pretty good job at it.
