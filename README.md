[![Build Status](https://travis-ci.org/dcarp/ProcessingFlow.png?branch=master)](https://travis-ci.org/dcarp/ProcessingFlow)

# ProcessingFlow

**ProcessingFlow** is a library for defining data processing flows usable
from multiple programming languages.

**ProcessingFlow**  is build on top of Intel's [TBB](https://www.threadingbuildingblocks.org)
library and provides small building blocks (`TwoTrackNodes`) that defines a
usualy liniar data flow with an additional track for collecting the errors.

[FlatBuffers](https://google.github.io/flatbuffers) serialization is used on
the border of the `TwoTrackNode`s with the goal to make the nodes accessible
from different programming language.

Currently just C++ support is implemented and tested.
