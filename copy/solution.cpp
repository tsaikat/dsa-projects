#ifndef __PROGTEST__
#include <cassert>
#include <cstdint>
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
#include <random>

using ChristmasTree = size_t;

struct TreeProblem {
  int max_group_size;
  std::vector<uint64_t> gifts;
  std::vector<std::pair<ChristmasTree, ChristmasTree>> connections;
};

#endif

struct Node
{   
    size_t id;
    Node* parent;
    // What is the max i gain if i take and if i don't take.
    uint64_t take;
    uint64_t noTake;
    std:: vector <size_t> childs;

    Node() : id(0), parent(nullptr), take(0), noTake(0) {}
    Node(size_t x, Node* p, uint64_t t) :id (x), parent(p), take(t), noTake(0) {}

    bool isChildless() {
        return childs.empty();
    }
    
};


uint64_t solveForOne (std::vector<std::vector<size_t>>& t, 
                  const std::vector<uint64_t>& gifts) {
 
    // Create a stack for DFS
    std::stack<Node*> stack;
    Node* root = new Node();
    

    Node* tmp = new Node(0, root, gifts[0]);
    

    // Push the current source node.
    stack.push(tmp);
 
    while (!stack.empty())
    {
        auto i = stack.top();

        i->childs  = std::move(t[i->id]);


        for ( auto& it : i->childs) {
            if (t[it].empty()) continue;
            stack.push(new Node(it, i , gifts[it]));
        }

        if (i->isChildless()) {
            i->parent->take += i->noTake;
            i->parent->noTake += (i->take > i->noTake ) ? i->take : i->noTake;
            stack.pop();
            delete i;
        }
    }


    auto result = root->noTake;
    delete root;
    return result;   
}


struct NodeTwo
{   
    size_t id;
    NodeTwo* parent;
    size_t size;
    // What is the max i gain if i take and if i don't take.
    uint64_t take;
    uint64_t noTake;
    std::vector <NodeTwo*> chooseChild; 
    std:: vector <size_t> childs;

    // Sum of gain if we don't take any child
    uint64_t noTakesumChilds = 0;

    NodeTwo() : id(0), parent(nullptr), take(0), noTake(0) {}
    NodeTwo(size_t x, NodeTwo* p, uint64_t t) :id (x), parent(p), size(0), take(t), noTake(0) {}

    bool isChildless() {
        return childs.empty();
    }    
};


uint64_t solveForTwo (std::vector<std::vector<size_t>>& t, 
                  const std::vector<uint64_t>& gifts) {
 
    // Create a stack for DFS
    std::stack<NodeTwo*> stack;
    NodeTwo* root = new NodeTwo();
    

    NodeTwo* tmp = new NodeTwo(0, root, gifts[0]);

    // Push the current source node.
    stack.push(tmp);
 
    while (!stack.empty())
    {
        auto i = stack.top();

        i->childs  = std::move(t[i->id]);


        for ( auto& it : i->childs) {
            if (t[it].empty()) continue;
            stack.push(new NodeTwo(it, i , gifts[it]));
        }

        if (i->isChildless()) {
            i->parent->size += i->size +1;
            if ( !(i->chooseChild.empty())) {
          
                for ( auto it : i->chooseChild) {
                  i->noTakesumChilds += it->noTake;
                }
                
                // best choice amonog the child as you can only take one child at max
                uint64_t max = 0;

                for ( auto it : i->chooseChild) {
                  if (it->noTake > it->take) continue;
                  
                  auto tmp = gifts[it->id] + it->noTakesumChilds + (i->noTakesumChilds - it->noTake);
                  max = tmp > max ? tmp : max;
                  
                }

                if (i->size < 2) i->take = max + gifts[i->id];
                else i->take = max + gifts[i->id]  > gifts[i->id] + i->noTakesumChilds ? //not clear right side comparison
                max + gifts[i->id] : gifts[i->id] + i->noTakesumChilds ;

            }

            i->parent->chooseChild.push_back(i);
            i->parent->noTake += i->take > i->noTake ? i->take : i->noTake;

            stack.pop();
            // delete i;
        }
    }

    // auto result = root->take > root->noTake ? root->take : root->noTake;
    // delete root;
    return root->noTake;   
}





uint64_t solve(const TreeProblem& t) {
  std::vector<std::vector<size_t>> adjList (t.gifts.size());
  
  for (auto& i : t.connections) {
    adjList[i.first].push_back(i.second);
    adjList[i.second].push_back(i.first);
  }

  uint64_t result = 0;
  if (t.max_group_size == 1) {
    result = solveForOne ( adjList, t.gifts);
  } else {
    result = solveForTwo (adjList, t.gifts);
  }

  return result;
}


#ifndef __PROGTEST__

using TestCase = std::pair<uint64_t, TreeProblem>;

const std::vector<TestCase> BASIC_TESTS = {
  { 3, { 1, { 1, 1, 1, 2 }, { {0,3}, {1,3}, {2,3} }}},
  { 4, { 1, { 1, 1, 1, 4 }, { {0,3}, {1,3}, {2,3} }}},
  { 57, { 1, {
    17, 11, 5, 13, 8, 12, 7, 4, 2, 8,
  }, {
    {1, 4}, {6, 1}, {2, 1}, {3, 8}, {8, 0}, {6, 0}, {5, 6}, {7, 2}, {0, 9},
  }}},
  { 85, { 1, {
    10, 16, 13, 4, 19, 8, 18, 17, 18, 19, 10,
  }, {
    {9, 7}, {9, 6}, {10, 4}, {4, 9}, {7, 1}, {0, 2}, {9, 2}, {3, 8}, {2, 3}, {5, 4},
  }}},
  { 79, { 1, {
    8, 14, 11, 8, 1, 13, 9, 14, 15, 12, 1, 11,
  }, {
    {9, 1}, {1, 2}, {1, 4}, {5, 10}, {7, 8}, {3, 7}, {11, 3}, {11, 10}, {6, 8}, {0, 1}, {0, 3},
  }}},
  { 102, { 1, {
    15, 10, 18, 18, 3, 4, 18, 12, 6, 19, 9, 19, 10,
  }, {
    {10, 2}, {11, 10}, {6, 3}, {10, 8}, {5, 3}, {11, 1}, {9, 5}, {0, 4}, {12, 3}, {9, 7}, {11, 9}, {4, 12},
  }}},
  { 93, { 1, {
    1, 7, 6, 18, 15, 2, 14, 15, 18, 8, 15, 1, 5, 6,
  }, {
    {0, 13}, {6, 12}, {0, 12}, {7, 8}, {8, 3}, {12, 11}, {12, 1}, {10, 12}, {2, 6}, {6, 9}, {12, 7}, {0, 4}, {0, 5},
  }}},
};

const std::vector<TestCase> BONUS_TESTS = {
  { 3, { 2, { 1, 1, 1, 2 }, { {0,3}, {1,3}, {2,3} }}},
  { 5, { 2, { 1, 1, 1, 4 }, { {0,3}, {1,3}, {2,3} }}},
  { 73, { 2, {
    17, 11, 5, 13, 8, 12, 7, 4, 2, 8,
  }, {
    {1, 4}, {6, 1}, {2, 1}, {3, 8}, {8, 0}, {6, 0}, {5, 6}, {7,\
     2}, {0, 9},
  }}},
  { 33, { 2, { 10, 19, 13, 4 }, { {0,2}, {1,2}, {2,3} }}},
  { 33, { 2, { 1, 20, 13, 10 }, { {0,2}, {1,2}, {2,3} }}},
  { 40, { 2, { 1, 1, 30, 10 }, { {0,2}, {1,2}, {2,3} }}},
  
  { 443, { 2, { 100, 20, 13, 1, 30, 300 }, { {0,2}, {1,2}, {2,3}, {1, 4}, {4, 5} }}},
  { 330, { 2, { 20, 30, 300 }, { {0,1}, {1,2} }}},
};

void test(const std::vector<TestCase>& T) {
  int i = 0;
  for (auto &[s, t] : T) {
    if (s != solve(t))
      std::cout << "Error in " << i << " (returned " << solve(t) << ")"<< std::endl;
    i++;
  }
  std::cout << "Finished" << std::endl;
}

int main() {
  test(BASIC_TESTS);
  test(BONUS_TESTS);
}

#endif


