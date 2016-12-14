[![Build Status](https://travis-ci.org/dcarp/ProcessingFlow.png?branch=master)](https://travis-ci.org/dcarp/ProcessingFlow)

# ProcessingFlow

**ProcessingFlow** is a library for defining data processing flows usable
from multiple programming languages.

**ProcessingFlow** is built on top of Intel's [TBB](https://www.threadingbuildingblocks.org)
library and provides small building blocks (`TwoTrackNodes`) that connected,
define a usually linear data flow with an additional track for collecting the
errors.

Although currently only C++ is supported and tested, the `TwoTrackNode`s
interfaces provide [FlatBuffers](https://google.github.io/flatbuffers)
serialization, with the goal of making the nodes accessible from different
programming languages.
