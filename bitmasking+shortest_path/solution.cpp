#ifndef __PROGTEST__
#include <cassert>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>

using Place = size_t;

struct Map {
  size_t places;
  Place start, end;
  std::vector<std::pair<Place, Place>> connections;
  std::vector<std::vector<Place>> items;
};

template < typename F, typename S >
struct std::hash<std::pair<F, S>> {
  std::size_t operator () (const std::pair<F, S> &p) const noexcept {
    // something like boost::combine would be much better
    return std::hash<F>()(p.first) ^ (std::hash<S>()(p.second) << 1);
  }
};

#endif


// *****MY IMPLEMENTATION

class Box {
public:
  Place prevNode;
  int prevMask;
  size_t dist = 0;
  
  void insert(Place node, int m, size_t d);
  void update (Place node, int m, size_t d);
};

void Box::insert( Place prevRoom, int m, size_t d) {
  prevNode = prevRoom;
  prevMask = m;
  dist = d;
}

void Box::update( Place prevRoom, int m, size_t d) {
  prevNode = prevRoom;
  prevMask = m;
  dist += d;
}


void generatePath(std::vector<std::vector<Box>>& visit, std::list<Place>& shortestPath, Box * magicNode, Place end) {
  shortestPath.push_front(end);
  while (magicNode->dist != 1) {
    shortestPath.push_front(magicNode->prevNode);
    magicNode = &(visit[magicNode->prevNode][magicNode->prevMask]);
  }
}

int setMask(int n, int k) {
	// kth bit of n is being set by this operation
	return ((1 << k) | n);
}


int itemFinder (const std::vector<std::vector<Place>> &items, Place room, int mask) {
  for (size_t i = 0; i < items.size(); ++i) { 
    if ( std::find(items[i].begin(), items[i].end(), room) != items[i].end() ) {
      mask = setMask(mask, i);
    }
  }
  return mask;
}

std::list<Place> find_path(const Map &map) {
  Place rooms = map.places;
  Place start = map.start;
  Place end = map.end;
  std::list<Place> shortestPath;

  std::vector< std::vector<size_t> > adjList (rooms);// to store the map in an adjacency list
  for (auto& i : map.connections) {
    adjList[i.first].push_back(i.second);
    adjList[i.second].push_back(i.first);
  }

  // ****************************************** 

  int itemsNum = map.items.size();

  // if one of them item has no room then we can return 0 as result.
  // sort the items list to make it easier to find
  for (auto it : map.items) {
    if (!it.size()) return shortestPath;
    // std::sort(it.begin(), it.end(), [] (Place a, Place b) {
    //   return a > b;
    // });
  }

  
  int itemPermutation =  1 << itemsNum; // getting the 2^itemsNum using bit shifting because faster
  
  std::vector<std::vector<Box>> visit ( rooms , std::vector<Box> (itemPermutation));

  std::queue<std::pair<size_t, int> > q;

  int mask = 0;
  mask = itemFinder(map.items, start, mask);
  visit[start][mask].insert(start, mask, 1);

  q.push({start, mask});

  int fullMask = (~(-1 << itemsNum) | 0 );

  if ( mask == fullMask && start == end) {
    shortestPath.push_front(start);
    return shortestPath;
  }

  while (!q.empty()) {
    auto node = q.front();
    q.pop();

    for ( auto it: adjList[node.first]) {
      int curMask = itemFinder(map.items, it, node.second);
      if (visit[it][curMask].dist == 0) {
        visit[it][curMask].insert(node.first, node.second, visit[node.first][node.second].dist+1);
        if (curMask == fullMask && it == map.end) {
          Box * magicNode = &(visit[it][curMask]);
          generatePath(visit, shortestPath, magicNode, end);
          return shortestPath;
        }
        q.push({it, curMask});
      }
    }
  }  
  return shortestPath;
}


#ifndef __PROGTEST__

using TestCase = std::pair<size_t, Map>;

// Class template argument deduction exists since C++17 :-)
const std::array examples = {
  TestCase{ 1, Map{ 2, 0, 0,
    { { 0, 1 } },
    { { 0 } }
  }},
  TestCase{ 3, Map{ 2, 0, 0,
    { { 0, 1 } },
    { { 1 } }
  }},
  TestCase{ 3, Map{ 4, 0, 1,
    { { 0, 2 }, { 2, 3 }, { 0, 3 }, { 3, 1 } },
    {}
  }},
  TestCase{ 4, Map{ 4, 0, 1,
    { { 0, 2 }, { 2, 3 }, { 0, 3 }, { 3, 1 } },
    { { 2 } }
  }},
  TestCase{ 0, Map{ 4, 0, 1,
    { { 0, 2 }, { 2, 3 }, { 0, 3 }, { 3, 1 } },
    { { 2 }, {} }
  }},
  TestCase{ 4, Map{ 6, 0, 1,
    { { 0, 2 }, { 0, 3 }, { 2, 3 }, { 3, 1 }, {2, 4}, { 4, 5}, {5, 1} },
    { { 5, 2}, {4, 3} }
  }},
};

int main() {
  int fail = 0;
  for (size_t i = 0; i < examples.size(); i++) {
    auto sol = find_path(examples[i].second);

    if (sol.size() != examples[i].first) {
      std::cout << "Wrong anwer for map " << i << std::endl;
      fail++;
    }
  }

  if (fail) std::cout << "Failed " << fail << " tests" << std::endl;
  else std::cout << "All tests completed" << std::endl;

  return 0;
}

#endif
