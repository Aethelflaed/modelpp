# model++

`model++` is a C++20 library containing various modules used to easily represent data models.

The main goal is to provide an easy way to load and extract the field values
from the models, so that the data can be transmitted via a web-service or
stored in a database.

This library provides absolutely no feature related to data storage, but could
easily be used in such a project.

## Design goals

- Simple interface
- Modern C++
- No external compiler to detect fields or whatever
- No MACRO needed, although some may be provided later but just to provide a
  short-hand syntax.

## Features

- model export (`data()`) and load (`load()`) for registered fields
- inherited model support
- tracking changes (`changed()`, `changed(field)`, `changes()`)

## Bugs

- At the moment, `std::variant` has troubles handling `bool` and `std:optional`,
  as identified in [P0608R3](www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0608r3.html)

## TODO

- provides support for nullable data with `std::optional`
- comment / doc
- validation module?
