
#include <iostream>
using namespace std;

class Map {
private:
    const int search(int first) const
    {
        Map* temp = root;

        // Stop only when either the key is found or we
        // have gone further the leaf node

        while (temp != nullptr && temp->first != first) {
            // Go to left if key is less than the key of
            // the traversed node

            if (first < temp->first) {
                temp = temp->left;
            }

            // Go to right if key is greater than the key
            // of the traversed node

            else {
                temp = temp->right;
            }
        }

        // If key is found

        if (temp != nullptr) {
            // If the key is found return the value

            return temp->second;
        }

        // If key is not found

        return 0;
    }

    Map* create(int first)
    {
        Map* newnode = (Map*)malloc(sizeof(Map));
        newnode->first = first;
        newnode->second = 0;
        newnode->left = nullptr;
        newnode->right = nullptr;
        newnode->par = nullptr;

        // Depth of a newnode shall be 1 and not zero to
        // differentiate between no child (which return
        // nullptr) and having child(which return 1)

        newnode->depth = 1;

        return newnode;
    }
    // All the rotation operation are performed about
    // the node itself

    // Performs all the linking done when there is
    // clockwise rotation performed at node "x"

    void right_rotation(Map* x)
    {
        Map* y = x->left;
        x->left = y->right;
        if (y->right != nullptr) {
            y->right->par = x;
        }
        if (x->par != nullptr && x->par->right == x) {
            x->par->right = y;
        }
        else if (x->par != nullptr && x->par->left == x) {
            x->par->left = y;
        }
        y->par = x->par;
        y->right = x;
        x->par = y;
    }

    // Performs all the linking done when there is
    // anti-clockwise rotation performed at node "x"

    void left_rotation(Map* x)
    {
        Map* y = x->right;
        x->right = y->left;
        if (y->left != nullptr) {
            y->left->par = x;
        }
        if (x->par != nullptr && x->par->left == x) {
            x->par->left = y;
        }
        else if (x->par != nullptr && x->par->right == x) {
            x->par->right = y;
        }
        y->par = x->par;
        y->left = x;
        x->par = y;
    }

    // Draw the initial and final graph of each
    // case(take case where every node has two child) and
    // update the nodes depth before any rotation

    void helper(Map* node)
    {

        // 17. if left skewed
        if (depthf(node->left) - depthf(node->right) > 1) {
            // If "depth" of left subtree of left child
            // of "node" is greater than that of right
            // subtree of left child of "node"

            if (depthf(node->left->left)
                > depthf(node->left->right)) {
                node->depth = max(depthf(node->right) + 1,
                          depthf(node->left->right) + 1);
                node->left->depth = max(depthf(node->left->left)
                                        + 1,depthf(node) + 1);
                right_rotation(node);
            }
            // If "depth" of right subtree of left child
            // of "node" is greater than that of left
            // subtree of left child of "node"

            else {
                node->left->depth = max(
                    depthf(node->left->left) + 1,
                    depthf(node->left->right->left) + 1);
                node->depth = max(
                    depthf(node->right) + 1,
                    depthf(node->left->right->right) + 1);
                node->left->right->depth
                    = max(depthf(node) + 1,
                          depthf(node->left) + 1);
                left_rotation(node->left);
                right_rotation(node);
            }
        }

        // If right skewed

        else if (depthf(node->left) - depthf(node->right)
                 < -1) {
            // If "depth" of right subtree of right
            // child of "node" is greater than that of left
            // subtree of right child of "node"

            if (depthf(node->right->right)
                > depthf(node->right->left)) {
                node->depth
                    = max(depthf(node->left) + 1,
                          depthf(node->right->left) + 1);
                node->right->depth
                    = max(depthf(node->right->right) + 1,
                          depthf(node) + 1);
                left_rotation(node);
            }
            // If "depth" of left subtree of right child
            // of "node" is greater than that of right
            // subtree of right child of "node"

            else {
                node->right->depth = max(
                    depthf(node->right->right) + 1,
                    depthf(node->right->left->right) + 1);
                node->depth = max(
                    depthf(node->left) + 1,
                    depthf(node->right->left->left) + 1);
                node->right->left->depth
                    = max(depthf(node) + 1,
                          depthf(node->right) + 1);
                right_rotation(node->right);
                left_rotation(node);
            }
        }
    }

    // Balancing the tree about the "node"

    void balance(Map* node)
    {
        while (node != root) {
            int d = node->depth;
            node = node->par;
            if (node->depth < d + 1) {
                node->depth = d + 1;
            }
            if (node == root
                && depthf(node->left) - depthf(node->right)
                       > 1) {
                if (depthf(node->left->left)
                    > depthf(node->left->right)) {
                    root = node->left;
                }
                else {
                    root = node->left->right;
                }
                helper(node);
                break;
            }
            else if (node == root
                     && depthf(node->left)
                                - depthf(node->right)
                            < -1) {
                if (depthf(node->right->right)
                    > depthf(node->right->left)) {
                    root = node->right;
                }
                else {
                    root = node->right->left;
                }
                helper(node);
                break;
            }
            helper(node);
        }
    }

    // Utility method to return the "depth" of the
    // subtree at the "node"

    int depthf(Map* node)
    {
        if (node == nullptr)

            // 25. if it is null node

            return 0;
        return node->depth;
    }

    Map* insert(int first)
    {
        Map* newnode = create(first);

        // If empty tree simply create the "root"

        if (root == nullptr) {
            root = newnode;
            return root;
        }
        Map *temp = root, *prev = nullptr;
        while (temp != nullptr) {
            prev = temp;
            if (first < temp->first) {
                temp = temp->left;
            }
            else if (first > temp->first) {
                temp = temp->right;
            }
            else {
                free(newnode);

                // If the key is found then it is
                // returned by reference so that it is
                // updatable

                return temp;
            }
        }
        if (first < prev->first) {
            prev->left = newnode;
        }
        else {
            prev->right = newnode;
        }
        newnode->par = prev;

        // Once we have inserted we need to check and
        // balance the tree at every node which is present
        // in the path from "newnode" to "root"

        balance(newnode);

        // New object is inserted and returned by
        // reference to initialize in the main by
        // assignment(hashing)

        return newnode;
    }

public:
    // "root" is kept static because it is a class
    // property and not an instance property

    // If the "root" is not static we will take double
    // memory for the programme than required
    // static "root" is initialized to nullptr outside the
    // class

    static class Map* root;

    // "first" is key and "second" is value

    Map *left, *right, *par;
    int first, second, depth;

    // Overloaded [] operator for assignment(hashing) or
    // inserting a key-value pairs in the map since it might
    // change the members of the class therefore this is
    // invoked when any assignment is done

    int& operator[](int key) { return insert(key)->second; }

    const int operator[](int key) const
    {
        // Search method is also qualified with const

        return search(key);
    }
};

Map* Map::root = nullptr;



int main()
{

    // here "map" is used for both
    // read-write operation

    Map map;

    // insertion
    map[132] = 3;
    map[34] = 5;
    map[42] = 7;
    map[-83] = 4;
    map[66] = 9;
    map[197] = 8;
    map[-2] = -88;

    // updation
    map[42] = 55;

    int arr[] = { 132, -34, 34, 42,
                 -83, 60, 66,  197,
                 -2, 56, 1,   -3442 };
    cout << "key: value\n";
    for (int i = 0; i < sizeof(arr) / 
         sizeof(int); i++) {
        // accessing value from its 
        // respective key

        // when we are searching for the
        // key and if it is not found then 
        //a new key is created with value 0
        // and then 0 is returned

        // if its found then its value is 
        //simply returned

        cout << '(' << arr[i] << ":"
             << map[arr[i]] << ") , ";
    }
    cout << endl;
    return 0;
}
