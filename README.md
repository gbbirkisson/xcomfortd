This code implements communication with the Eaton CKOZ-00/14
Communication stick (CI stick).  The CI stick needs to be added into
the network with CKOZ-00/13.

The code has been written without any kind of documentation from
Eaton, and may not follow their specifications.

The CKOZ-00/14 doesn't expose or know which devices hide behind the
datapoints.  It's the user's responsibility to send correct messages
to the datapoints; this code does no validation of messages sent.
However, the devices appear to ignore messages they don't understand.
MRF can export a datapoint to device map, but I have not added support
for that, as the benefits were limited.

A simple daemon for forwarding events to and from an MQTT server is
provided.

_WARNING: When I upgraded the CI stick to the "RF V2.08 - USB V2.05"
firmware, status reports from dimmers stopped working.  This was not
only an issue with this code, but with other applications as well.
The solution was to downgrade to "RF V1.08 - USB V1.04"._

This code was reverse engineered from a variety of sources, plus some
initial inspiration from <https://github.com/mifi/libxcomfort>.
