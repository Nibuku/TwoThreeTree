#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

template <typename T>
class Two3Tree {
private:
    struct TwoThreeNode {
        std::vector<T> keys;  
        TwoThreeNode* left = nullptr;
        TwoThreeNode* middle = nullptr;  
        TwoThreeNode* right = nullptr;

        TwoThreeNode() = default;
        TwoThreeNode(const T& key) {
            keys.push_back(key);
        }
    };

    TwoThreeNode* _root;
   void clear(TwoThreeNode*& node) {
        if (node) {
            if (node->left)
                clear(node->left);
            if (node->middle)
                clear(node->middle);
            if (node->right)
                clear(node->right);
            delete node;
            node = nullptr;
        }
    }

   bool insert(TwoThreeNode*& node, const T& key, T& new_key, TwoThreeNode*& new_node) {
       if (!node) {
           node = new TwoThreeNode(key);
           return true;
       }

       if (!node->left) {
           if (node->keys.size() < 2) {
               node->keys.push_back(key);
               std::sort(node->keys.begin(), node->keys.end());
               return false;
           }
           else {
               std::vector<T> tmp = { node->keys[0], node->keys[1], key };
               std::sort(tmp.begin(), tmp.end());

               node->keys = { tmp[0] };
               new_key = tmp[1];
               new_node = new TwoThreeNode(tmp[2]);
               return true;
           }
       }
       else {
           TwoThreeNode* child;
           if (key < node->keys[0]) {
               child = node->left;
           }
           else if (node->keys.size() == 1 || key < node->keys[1]) {
               child = node->middle;
           }
           else {
               child = node->right;
           }

           T child_new_key;
           TwoThreeNode* child_new_node = nullptr;
           bool result = insert(child, key, child_new_key, child_new_node);

           if (result) {
               if (node->keys.size() == 1) {
                   if (key < node->keys[0]) {
                       node->keys.push_back(node->keys[0]);
                       node->keys[0] = child_new_key;
                       node->right = node->middle;
                       node->middle = child_new_node;
                   }
                   else {
                       node->keys.push_back(child_new_key);
                       node->right = child_new_node;
                   }
                   return false;
               }
               else {
                   std::vector<T> tmp = { node->keys[0], node->keys[1], child_new_key };
                   std::sort(tmp.begin(), tmp.end());

                   node->keys = { tmp[0] };
                   new_key = tmp[1];
                   new_node = new TwoThreeNode(tmp[2]);
                   return true;
               }
           }
           return false;
       }
   }

   TwoThreeNode* findMin(TwoThreeNode* node) const {
       while (node->left) {
           node = node->left;
       }
       return node;
   }

   bool remove(TwoThreeNode*& node, const T& key) {
       if (!node) return false;

       auto it = std::find(node->keys.begin(), node->keys.end(), key);

       if (it != node->keys.end()) {
           if (!node->left) {
               node->keys.erase(it);
               if (node->keys.empty()) {
                   delete node;
                   node = nullptr;
                   return true;
               }
               return false;
           }
           else {
               if (node->keys.size() == 1) {
                   TwoThreeNode* min = findMin(node->middle);
                   *it = min->keys[0];
                   return remove(node->middle, min->keys[0]);
               }
               else {
                   if (key == node->keys[0]) {
                       TwoThreeNode* min = findMin(node->middle);
                       node->keys[0] = min->keys[0];
                       return remove(node->middle, min->keys[0]);
                   }
                   else {
                       TwoThreeNode* min = findMin(node->right);
                       node->keys[1] = min->keys[0];
                       return remove(node->right, min->keys[0]);
                   }
               }
           }
       }
       else {
           bool result = false;
           if (key < node->keys[0]) {
               result = remove(node->left, key);
           }
           else if (node->keys.size() == 1 || key < node->keys[1]) {
               result = remove(node->middle, key);
           }
           else {
               result = remove(node->right, key);
           }

           if (result) {
               if (node->keys.size() == 1) {
                   if (node->left && node->left->keys.size() == 1) {
                       node->keys.push_back(node->left->keys[0]);
                       node->keys.push_back(node->middle->keys[0]);
                       std::sort(node->keys.begin(), node->keys.end());
                       delete node->middle;
                       node->middle = nullptr;
                   }
                   else if (node->middle && node->middle->keys.size() == 1) {
                       node->keys.push_back(node->middle->keys[0]);
                       node->keys.push_back(node->right->keys[0]);
                       std::sort(node->keys.begin(), node->keys.end());
                       delete node->right;
                       node->right = nullptr;
                   }
               }
               else {
                   if (key < node->keys[0]) {
                       if (node->left && node->left->keys.size() == 1) {
                           node->keys.erase(node->keys.begin());
                           node->keys.insert(node->keys.begin(), node->left->keys[0]);
                           delete node->left;
                           node->left = nullptr;
                       }
                   }
                   else {
                       if (node->middle && node->middle->keys.size() == 1) {
                           node->keys.erase(node->keys.begin() + 1);
                           node->keys.insert(node->keys.begin() + 1, node->middle->keys[0]);
                           delete node->middle;
                           node->middle = nullptr;
                       }
                   }
               }
           }
           return result;
       }
   }


    bool search(TwoThreeNode* node, const T& key) const {
        if (!node) 
            return false;
        if (std::find(node->keys.begin(), node->keys.end(), key) != node->keys.end()) 
            return true;
        if (key < node->keys[0]) 
            return search(node->left, key);
        else if (node->keys.size() == 1 || key < node->keys[1])
            return search(node->middle, key);
        else 
            return search(node->right, key);
    }

    void print_tree(TwoThreeNode* root) {
        if (!root)
            return;

        print_tree(root->left);
        for (size_t i = 0; i < root->keys.size();
            ++i) {
            std::cout << root->keys[i] << " ";
        }
        print_tree(root->middle);
        print_tree(root->right);
    }

public:
    Two3Tree() : _root(nullptr) {}
    ~Two3Tree() {
       		clear(_root);
            _root = nullptr;
    }

    void insert(const T& key) {
        if (!_root) {
            _root = new TwoThreeNode(key);
        }
        else {
            TwoThreeNode* newChild = nullptr;
            T newKey;
            if (insert(_root, key, newKey, newChild)) {
                TwoThreeNode* newRoot = new TwoThreeNode(newKey);
                newRoot->left = _root;
                newRoot->middle = newChild;
                _root = newRoot;
            }
        }
    }

    bool search(const T & key) const {
        return search(_root,key);
    }

    void print() {
        if (!_root)
            return;
        print_tree(_root);
    }

    void remove(const T& key) {
        remove(_root, key);
    }

};
