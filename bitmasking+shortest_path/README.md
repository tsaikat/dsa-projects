## Problem

The aim is to implement the function `std::list<Place> find_path(const Map &map)` which finds the shortest walk such that you obtain at least one item of each kind. The walk must begin at `map.start`, end at `map.end` and a connection must exist between every two consecutive rooms of the walk. If there is no such a walk, return an empty list.

The structure `Map` contains the following:

- `places`: the total number of rooms,
- `start`, `end`: the first, resp. last, room of the walk,
- `connections`: a list of pairs of rooms describing the connections,
- `items`: a list of lists, `items[i]` is a list of rooms in which the i-th item is placed.

You can assume that the room numbers are always in the range `0` up to `places - 1` (including).