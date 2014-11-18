rtimdb
======

This Real-Time In-Memory Database was designed for integration for embedded systems communicating using one
of the popular SCADA protocols, such as DNP3. As such, the principal use-cases considered in the design 
consist of the device receiving control requests (select, operate, direct-operate, write), freeze requests 
(immediate freeze, freeze-and-clear, freeze-at-time1) and read requests.

A typical device would have a few hundred data points, each of which represented with a type and an index.
Some systems may give unique names to their data points. However, RTIMDB currently only supports accessing 
points through their type and index.
