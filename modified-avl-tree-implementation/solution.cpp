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
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <random>
#include <chrono>


#endif

namespace AVL {

template <class T>
class Node {
public :
    const T data;

    Node<T> * left, * right;

    int height;
    size_t onLeft;
    size_t onRight;

    size_t sumLeft;
    size_t sumRight;

    void compute_height () {
        height = 0;
        if (left != nullptr && left -> height > height) {
            height = left -> height;
        }
        if (right != nullptr && right -> height > height) {
            height = right -> height;
        }
        height += 1;
    }
    
    Node (const T & inData)
        : data (inData), left (nullptr), right (nullptr), height (1), onLeft(0), onRight(0), sumLeft(0), sumRight(0) { }

    ~Node () {
        delete left;
        delete right;
    }

    Node<T> * balance () {
        
        int d = difference_in_height ();

        // only rotate if out of balance
        if (d < -1 || d > 1) {
            // too heavy on the right
            if (d < 0) 
            {
                // if right child is too heavy on the left,
                // rotate right child to the right
                if (right -> difference_in_height () > 0) {
                    right = right -> rotate_right ();
                }

                // rotate current node to the left
                return rotate_left ();
            }
            // too heavy on the left
            else
            {
                // if left child is too heavy on the right,
                // rotate left child to the left
                if (left -> difference_in_height () < 0) {
                    left = left -> rotate_left (); 
                }
    
                // rotate current node to the right
                return rotate_right ();
            }
        }
    
        // recompute the height of each node on the way up
        compute_height ();

        // otherwise, the node is balanced and we simply return it
        return this;
    }

private:

    Node<T> * exchange_left (Node<T> * & r, Node<T> * node) {
        r = left; // pointing/exchanging x's right to y's left
        if (left != nullptr) {
            node->onRight = left->onRight + left->onLeft + 1;
            node->sumRight = left->sumRight + left->sumLeft + left->data->sold; 
        } else {
            node->onRight = 0;
            node->sumRight = 0;
        }
        onLeft = node->onRight + node->onLeft + 1;
        sumLeft = node->sumRight + node->sumLeft + node->data->sold;
        
        left = node -> balance (); // y left is now pointing to x (or whatver x is balanced to)
        return balance ();
    }

    Node<T> * exchange_right (Node<T> * & l, Node<T> * node) {
        l = right;
        if (right != nullptr) {
            node->onLeft = right->onRight + right->onLeft + 1;
            node->sumLeft = right->sumRight + right->sumLeft + right->data->sold;
        } else {
            node->onLeft = 0;
            node->sumLeft = 0;
        }
        onRight = node->onRight + node->onLeft + 1;
        sumRight = node->sumRight + node->sumLeft + node->data->sold;
        right = node -> balance ();
        return balance ();
    }

    int difference_in_height () {
        int left_height = (left != nullptr) ? left -> height : 0;
        int right_height = (right != nullptr) ? right -> height : 0;
        return left_height - right_height;
    }

    Node<T> * rotate_left () {
        return right -> exchange_left (right, this);
    }

    Node<T> * rotate_right () {
        return left -> exchange_right (left, this);
    }

};

template <class T>
class Tree {
private :
    Node<T> * root;
    size_t size;

public :
    Tree () {
        root = nullptr;
        size = 0;
    }

    ~Tree () {
        delete root;
    }

    void insert (const T & inData) {
        root = insert_node (root, inData);
        ++size;

    }
    size_t getSize () const {
        return size;
    }

    void remove (const T & inData) {
        root = remove_node (root, inData);
        --size;
    }

    // void sort (std::vector<T>& inorder) const {
    //     inorder.clear();
    //     size_t i = 0;
    //     inorder_traversal(root, inorder, i);
    //     // std:: cout << "-----------" << std:: endl;
    // }

    T find_rank (size_t rank) const {
        return (find(root, rank))->data;
    }

    size_t getRank (const T& value) const {
        return find_rank_by_value(root, value, 0);
    }
    
    size_t firstSame (const size_t value) const {
        return find_first_same(root, value, 0);
    }

    size_t lastSame (const size_t value) const {
        return find_last_same(root, value, 0);
    }
    
    size_t sumRange (size_t start, size_t end) const {
        return (root->sumLeft + root->data->sold + root->sumRight) - (remove_before(root, start)) - (remove_after(root, end));
        return 0;
    }




private:

    // size_t calc_sumRange(Node<T>* node, size_t start, size_t end, size_t rank) {
    //     return sumTree - before(start) - after(end);
    // }

    // size_t calc_idx(Node<T>* node, size_t idx) {
    //     return node->onLeft + node->onRight + 1;
    // }

    size_t remove_before (Node<T>* node, size_t idx)  const {
        size_t sum = 0;
        if (idx == node->onLeft + 1)  return sum += node->sumLeft;
        
        if ( idx < node->onLeft+1) {
            return sum += remove_before(node->left, idx );
        } else {
            sum += node->sumLeft + node->data->sold;
            return sum += remove_before(node->right, (idx - (node->onLeft+1)) );
        }

        // if ( idx == 1) return 
        // if (idx == node->onLeft + 1) {
        //     return remove_before(node, idx - node->onLeft);
        // }
        
        // if ( rank < node->onLeft+1) {
        //     return find(node->left, rank );
        // } else {
        //     return find(node->right, (rank - (node->onLeft+1)) );
        // }
    }



    size_t remove_after (Node<T>* node, size_t idx)  const {
        size_t sum = 0;
        if (idx == node->onLeft + 1)  return sum += node->sumRight;
        
        if ( idx > node->onLeft+1) {
            return sum += remove_after(node->right, (idx - (node->onLeft+1)));
        } else {
            sum += node->sumRight + node->data->sold;
            return sum += remove_after(node->left, idx );
        }
    }


    size_t find_first_same (Node<T>* node, const size_t value, size_t rank)  const {
        if (node == nullptr) return 0;
        if (value == node->data->sold) {
            size_t tmp = find_first_same(node->left, value , rank);
            tmp = (tmp == 0 ) ? (rank + (node->onLeft + 1)) : tmp;
            return tmp;
        }
        
        if (value > node->data->sold) {
            return find_first_same(node->left, value, rank );
        } else { 
            return find_first_same(node->right, value, rank + (node->onLeft + 1) );
        }
    }


    size_t find_last_same (Node<T>* node, const size_t value, size_t rank)  const {
        if (node == nullptr) return 0;
         if (value == node->data->sold) {
            size_t tmp = find_last_same(node->right, value, rank + (node->onLeft + 1) );
            tmp = (tmp == 0 ) ? (rank + (node->onLeft + 1)) : tmp;
            return tmp;
        }
        
        if (value > node->data->sold) {
            return find_last_same(node->left, value, rank );
        } else { 
            return find_last_same(node->right, value, rank + (node->onLeft + 1) );
        }
    }
    
    size_t find_rank_by_value (Node<T>* node, const T& value, size_t rank)  const {
        if (node == nullptr) return 0;
        if (*value == *(node->data)) return rank + (node->onLeft + 1);
        
        if ( *value < *(node->data) ) {
            return find_rank_by_value(node->left, value, rank );
        } else if (*value > *(node->data) ){
            return find_rank_by_value(node->right, value, rank + (node->onLeft + 1) );
        } else {
            auto tmp = find_rank_by_value(node->left, value, rank );
            if (tmp == 0)  tmp = find_rank_by_value(node->right, value, rank + (node->onLeft + 1) );
            return tmp;
        }
    }

    Node<T> * find (Node<T>* node, size_t rank)  const {
        if (rank == node->onLeft + 1) return node;
        
        if ( rank < node->onLeft+1) {
            return find(node->left, rank );
        } else {
            return find(node->right, (rank - (node->onLeft+1)) );
        }
    }

    Node<T> * insert_node (Node<T>* node, const T & inData) {
        if (node == nullptr) return new Node<T> (inData);
        if (*inData < *(node->data)) {
            node->left = insert_node (node->left, inData);
            ++(node->onLeft);
            node->sumLeft += inData->sold;
        } else {
            node->right = insert_node (node->right, inData);
            ++(node->onRight);
            node->sumRight += inData->sold;
        }
        return node->balance();
    }

    // Recursively traverse to extract sorted array).
    // void inorder_traversal (Node<T>* node, std::vector<T> & inorder, size_t& i) const {
    //     if (node == nullptr) return;

    //     std::cout << node->data->sold << " - " << "L" << node->onLeft << " | " <<"R" << node->onRight << std::endl;

    //     inorder_traversal (node->left, inorder, i);
    //     // std::cout << node->onLeft << " L - R "<<node->onRight << std::endl;
    //     node->data->rank = ++i;
    //     inorder.push_back(node->data);
    //     // std:: cout << "up <<  ";
    //     inorder_traversal (node->right, inorder, i);
    // }

    Node<T> * remove_node (Node<T>* node, const T & inData) {
        if (node == nullptr) return nullptr;


        // we found the data we were looking for

        if (*inData == *(node->data))
        {
            // save the children

            Node<T> * tmp = move_down_righthand_side (node->left, node->right);

            // by setting the children to nullptr, we delete exactly one node.

            node->left = nullptr;
            node->right = nullptr;
            delete node;

            // return the reorganized children

            return tmp;
        }

        if (*inData < *(node->data)) {
            node->left = remove_node (node->left, inData);
        } else if (*inData > *(node->data)) {
            node->right = remove_node (node->right, inData);
        } else {
            node->left = remove_node (node->left, inData);
            node->right = remove_node (node->right, inData);
        }

        // assuming we know the node we want to delete exist
        if (node->left != nullptr) {
            node->onLeft = node->left->onRight + node->left->onLeft + 1;
            node->sumLeft = node->left->sumRight + node->left->sumLeft + node->left->data->sold;
        } else {
            node->onLeft = 0;
            node->sumLeft = 0;
        }
        if (node->right != nullptr) {
            node->onRight = node->right->onRight + node->right->onLeft + 1;
            node->sumRight = node->right->sumRight + node->right->sumLeft + node->right->data->sold;
        } else {
            node->onRight = 0;
            node->sumRight = 0;
        }

        return node->balance();
    }

    Node<T> * move_down_righthand_side (Node<T> * node, Node<T> * rhs) {
        if (node == nullptr) return rhs;

        node->right = move_down_righthand_side (node->right, rhs);
        if (node->right != nullptr) {
            node->onRight = node->right->onRight + node->right->onLeft + 1;
            node->sumRight = node->right->sumRight + node->right->sumLeft + node->right->data->sold;
        } else {
            node->onRight = 0;
            node->sumRight = 0;
        }
        return node->balance ();
    }
};

}




// TODO implement
template < typename Product >
struct Bestsellers {

  struct Node {
    Product product;
    size_t sold;
    size_t rank;
    size_t totalSold;
    Node (const Product& n, size_t s) : product(n), sold(s), rank(0), totalSold(0) {}

    bool operator == ( const Node& rhs) const {
        return (product == rhs.product && sold == rhs.sold );
    }

    bool operator < (const Node& rhs) const {
        return sold > rhs.sold;
    }

    bool operator > (const Node& rhs) const {
        return sold < rhs.sold;
    }
  };

  std::unordered_map<Product, std::shared_ptr<Node> > hashMap;
  AVL::Tree<std::shared_ptr<Node> > tree;
  
  // The total number of tracked products  
  size_t products() const { return tree.getSize();  }

  void sell (const Product& p, size_t amount) {
    std::shared_ptr<Node> tmp = std::make_shared<Node>(p, amount);
        
     if ((hashMap.try_emplace(p, tmp)).second) { 
        // add new product
        tree.insert(tmp);
      } else {
        // update existing product
        tree.remove(hashMap[p]);
        hashMap[p]->sold += amount;
        tree.insert(hashMap[p]);
      }
    return;
  }


  // The most sold product has rank 1
  size_t rank(const Product& p) const {
    auto it = hashMap.find(p);
    if (it == hashMap.end()) throw std::out_of_range("no such product found");
    return tree.getRank(it->second);
    }

  const Product& product(size_t rank) const {
    if (rank < 1 || rank > tree.getSize()) { throw std::out_of_range("rank out of range");}
    return (tree.find_rank(rank))->product; 
  }

  // How many copies of product with given rank were sold
  size_t sold(size_t rank) const {
    if (rank < 1 || rank > tree.getSize() ) throw std::out_of_range("Given rank is out of range");
    return (tree.find_rank(rank))->sold; 
  }

  // The same but sum over interval of products (including from and to)
  // It must hold: sold(x) == sold(x, x)
  size_t sold(size_t from, size_t to) const {
    if (from < 1 || to < 1 || to < from || from > tree.getSize() || to > tree.getSize() ) {
      throw std::out_of_range("rank range is out of range");
    }
    
    if (from == to) return (tree.find_rank(from))->sold;
    return tree.sumRange(from , to);

  }



  // Bonus only, ignore if you are not interested in bonus
  // The smallest (resp. largest) rank with sold(rank) == sold(r)
  // basically for example for the first_same - it will find the product sold for item on the position r
  // and then find the smallest rank of the product who has the same amount sold.

  size_t first_same(size_t r) const {
    if (r < 1 || r > tree.getSize() ) throw std::out_of_range("Given rank is out of range");
    return tree.firstSame((tree.find_rank(r))->sold);
    }
  
  size_t last_same(size_t r) const {
    if (r < 1 || r > tree.getSize() ) throw std::out_of_range("Given rank is out of range");
    return tree.lastSame((tree.find_rank(r))->sold);
    }

// size_t first_same(size_t r) const { return 0;}
//   size_t last_same(size_t r) const { return 0;}

};


#ifndef __PROGTEST__

void test1() {
  Bestsellers<std::string> T;
  T.sell("coke", 32);
  T.sell("bread", 1);
  assert(T.products() == 2);
  T.sell("ham", 2);
  T.sell("mushrooms", 12);

  assert(T.products() == 4);
  assert(T.rank("ham") == 3);
  assert(T.rank("coke") == 1);
  assert(T.sold(1, 3) == 46);
  assert(T.product(2) == "mushrooms");

  T.sell("ham", 11);
  assert(T.products() == 4);
  assert(T.product(2) == "ham");
  assert(T.sold(2) == 13);
  assert(T.sold(2, 2) == 13);
  assert(T.sold(1, 2) == 45);
}

void test2() {
# define CATCH(expr) \
  try { expr; assert(0); } catch (const std::out_of_range&) { assert(1); };

  Bestsellers<std::string> T;
  T.sell("coke", 32);
  T.sell("bread", 1);
  CATCH(T.rank("ham"));
  CATCH(T.product(3));
  CATCH(T.sold(0));
  CATCH(T.sold(9));
  CATCH(T.sold(0, 1));
  CATCH(T.sold(3, 2));
  CATCH(T.sold(1, 9));
  

#undef CATCH
}

void test3 () {
  Bestsellers<std::string> T;
  T.sell("coke", 32);
  T.sell("bread", 2);
  assert(T.products() == 2);
  T.sell("ham", 2);
  T.sell("mushrooms", 12);
  T.sell("bread", 30);

  assert(T.products() == 4);
  assert(T.sold(1, 4) == 78);
  assert(T.product(2) == "coke" || T.product(2) == "bread");

  T.sell("Jam", 32);
  assert(T.products() == 5);
  assert(T.sold(1, 5) == 110);
  T.sell("x", 32);

  assert(T.products() == 6);
  assert(T.sold(1, 6) == 142);
  assert(T.sold(1, 4) == 128);
  assert(T.sold(1) == 32);
  assert(T.sold(4) == 32);

  T.sell("yleiz", 84667);
  T.sell("ykojz", 84667);
  T.sell("yleiz", 2000);
  T.sell("ykojz", 845);

  assert(T.products() == 8);
  assert(T.product(1) == "yleiz");
  assert(T.product(2) == "ykojz");


  
}
void test4 () {
  Bestsellers<std::string> T;
  T.sell("coke", 62);
  T.sell("x", 42);
  T.sell("c", 5);
  T.sell("e", 32);
  T.sell("ex", 2);
  T.sell("top", 222);
  T.sell("exercc", 32);
  T.sell("exxxx", 55);
  T.sell("exercc", 88);
  T.sell("top", 58);
  T.sell("coke", 10);
  T.sell("exxxx", 17);

  assert(T.rank("coke") == 3);
  assert(T.rank("top") == 1);

  assert(T.rank("ex") == 8);
  T.sell("Test", 72);

  for (size_t i = 1; i <= T.products(); i++) {
    assert(T.sold(T.rank(T.product(i))) == T.sold(i) );
    // std::cout <<T.rank(T.product(i)) <<" : " << T.product(i)  <<" (" << T.sold(i) <<")"<< std::endl;
  }

  assert(T.first_same(5) == 3 );
  assert(T.first_same(1) == 1 );
  assert(T.first_same(9) == 9 );
  
  assert(T.last_same(3) == 5 );
  assert(T.last_same(1) == 1 );
  assert(T.last_same(5) == 5 );
  assert(T.last_same(4) == 5 );
  assert(T.last_same(9) == 9 );
  assert(T.sold(3, 5) == 216 );
  assert(T.sold(1, 2) == 280+120 );
  assert(T.sold(1, 3) == 280+120+72 );
  
}



void test5 () {
  Bestsellers<std::string> T;
  T.sell("a", 33);
  T.sell("s", 33);
  T.sell("d", 33);
  T.sell("f", 33);
  T.sell("g", 33);
  T.sell("h", 33);
  T.sell("j", 33);
  T.sell("x", 33);
  T.sell("c", 33);
  T.sell("v", 33);
  T.sell("b", 33);
  T.sell("top", 203);


  assert(T.first_same(5) == 2 );
  assert(T.first_same(1) == 1 );
  assert(T.first_same(9) == 2 );
  
  assert(T.last_same(3) == T.products() );
  assert(T.last_same(2) == T.products() );
  assert(T.last_same(5) == T.products() );
  assert(T.last_same(4) == T.products() );
  assert(T.last_same(9) == T.products() );

  assert(T.last_same(1) == 1 );
  
}


void test6 () {
  Bestsellers<std::string> T;
  T.sell("a", 280);
  T.sell("s", 120);
  T.sell("d", 72);
  T.sell("f", 75);
  T.sell("g", 73);
  T.sell("h", 72);
  T.sell("j", 72);
  T.sell("x", 42);
  T.sell("c", 72);
  T.sell("v", 72);
  T.sell("b", 5);
  T.sell("top", 32);
  T.sell("tp", 2);


  assert(T.first_same(1) == 1 );
  assert(T.first_same(2) == 2 );
  assert(T.first_same(13) == 13 );
  assert(T.first_same(12) == 12 );
  assert(T.first_same(3) == 3 );
  assert(T.first_same(4) == 4 );

  assert(T.first_same(5) == 5 );
  assert(T.first_same(6) == 5 );
  assert(T.first_same(7) == 5 );
  assert(T.first_same(8) == 5 );
  assert(T.first_same(9) == 5 );

  
  
  assert(T.last_same(1) == 1 );
  assert(T.last_same(2) == 2 );
  assert(T.last_same(13) == 13 );
  assert(T.last_same(12) == 12 );
  assert(T.last_same(3) == 3 );
  assert(T.last_same(4) == 4 );
  
  assert(T.last_same(5) == 9 );
  assert(T.last_same(6) == 9 );
  assert(T.last_same(7) == 9 );
  assert(T.last_same(8) == 9 );
  assert(T.last_same(9) == 9 );

  assert(T.last_same(1) == 1 );
  
}

int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6 ();
  return 0;
}

#endif


