#include <gtest/gtest.h>
#include <twothreetree/twothreetree.h>

using namespace std;

TEST(Two3Tree, insert_test) {
    Two3Tree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(5);
    tree.insert(6);
    tree.insert(12);
    tree.insert(30);
    tree.insert(17);
       
}

TEST(Two3Tree, print_test) {
    Two3Tree<int> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(18);
    tree.insert(6);
    tree.insert(12); 
    tree.insert(-1);
    tree.insert(-7);
    tree.print();
}

TEST(Two3Tree, search_test) {
    Two3Tree<int> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(18);
    tree.insert(12);
    tree.insert(-1);
    tree.insert(-7);
    EXPECT_TRUE(tree.search(10) == true);
    EXPECT_TRUE(tree.search(12) == true);
    EXPECT_TRUE(tree.search(0) == false);
}
TEST(Two3Tree, remove_test) {
    Two3Tree<int> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(18);
    tree.insert(12);
    tree.insert(-1);
    tree.insert(-7);
    tree.remove(18);
    tree.remove(1);
    tree.remove(-7);
    tree.print();
}
