#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

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

    void print_tree(TwoThreeNode* root) const {
        if (!root)
            return;

        print_tree(root->left);
        for (const auto& key : root->keys) {
            std::cout << key << " ";
        }
        print_tree(root->middle);
        print_tree(root->right);
    }


    bool insert(TwoThreeNode*& node, const T& key, T& new_key, TwoThreeNode*& new_node) {
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
                    std::sort(node->keys.begin(), node->keys.end());
                    return false;
                }
                else {
                    std::vector<T> tmp = { node->keys[0], node->keys[1], child_new_key };
                    std::sort(tmp.begin(), tmp.end());

                    TwoThreeNode* new_left = new TwoThreeNode(tmp[0]);
                    TwoThreeNode* new_right = new TwoThreeNode(tmp[2]);

                    if (child == node->left) {
                        new_left->left = child;
                        new_left->middle = child_new_node;
                        new_right->left = node->middle;
                        new_right->middle = node->right;
                    }
                    else if (child == node->middle) {
                        new_left->left = node->left;
                        new_left->middle = child;
                        new_right->left = child_new_node;
                        new_right->middle = node->right;
                    }
                    else {
                        new_left->left = node->left;
                        new_left->middle = node->middle;
                        new_right->left = child;
                        new_right->middle = child_new_node;
                    }

                    node->keys = { tmp[1] };
                    node->left = new_left;
                    node->middle = new_right;
                    node->right = nullptr;

                    new_key = tmp[1];
                    new_node = new_right;
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

    public:
        Two3Tree() : _root(nullptr) {}

       ~Two3Tree() {
            clear(_root);
            _root = nullptr;
        }

        void insert(const T& key) {
            T new_key;
            TwoThreeNode* new_node = nullptr;
            if (_root == nullptr) {
                _root = new TwoThreeNode(key);
            }
            else {
                if (insert(_root, key, new_key, new_node)) {
                    TwoThreeNode* old_root = _root;
                    _root = new TwoThreeNode(new_key);
                    _root->left = old_root;
                    _root->middle = new_node;
                }
            }
        }

        bool search(const T& key) const {
            return search(_root, key);
        }

        void print() const {
            if (!_root)
                return;
            print_tree(_root);
        }

        void remove(const T& key) {
            remove(_root, key);
        }

        void fill_tree_with_random_values(int count, int min_value, int max_value) {
            std::srand(std::time(0));
            for (int i = 0; i < count; ++i) {
                int value = min_value + std::rand() % (max_value - min_value + 1);
                insert(value);
            }
        }

        void search_random_values(const std::vector<int>& values) {
            for (const auto& value : values) {
                search(value);
            }
        }

        void remove_random_values(const std::vector<int>& values) {
            for (const auto& value : values) {
                remove(value);
            }
        }

};
