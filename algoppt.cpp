#include <iostream>
#include <vector>

const int MIN_DEGREE = 2;

class BTreeNode {
public:
    bool is_leaf;
    std::vector<int> keys;
    std::vector<BTreeNode*> children;

    BTreeNode(bool _is_leaf) : is_leaf(_is_leaf) {}

    void insert_non_full(int key);
    void split_child(int i, BTreeNode* y);
    void traverse();
    BTreeNode* search(int key);
};

class BTree {
public:
    BTreeNode* root;

    BTree() : root(nullptr) {}
    void insert(int key);
    void traverse();
    BTreeNode* search(int key);
    void remove(int key);
};

void BTreeNode::insert_non_full(int key) {
    int i = keys.size() - 1;

    if (is_leaf) {
        keys.push_back(0);
        while (i >= 0 && key < keys[i]) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = key;
    } else {
        while (i >= 0 && key < keys[i])
            i--;

        if (children[i + 1]->keys.size() == (2 * MIN_DEGREE - 1)) {
            split_child(i + 1, children[i + 1]);

            // After split, determining which of the two children is going to have the new key
            if (key > keys[i + 1])
                i++;
        }

        children[i + 1]->insert_non_full(key);
    }
}

void BTreeNode::split_child(int i, BTreeNode* y) {
    BTreeNode* z = new BTreeNode(y->is_leaf);
    z->keys.reserve(MIN_DEGREE - 1);
    for (int j = 0; j < MIN_DEGREE - 1; j++)
        z->keys.push_back(y->keys[j + MIN_DEGREE]);
    if (!y->is_leaf) {
        z->children.reserve(MIN_DEGREE);
        for (int j = 0; j < MIN_DEGREE; j++)
            z->children.push_back(y->children[j + MIN_DEGREE]);
    }

    y->keys.resize(MIN_DEGREE - 1);

    children.insert(children.begin() + i + 1, z);

    keys.insert(keys.begin() + i, y->keys[MIN_DEGREE - 1]);
}

void BTreeNode::traverse() {
    int i;
    for (i = 0; i < keys.size(); i++) {
        if (!is_leaf)
            children[i]->traverse();
        std::cout << keys[i] << " ";
    }

    if (!is_leaf)
        children[i]->traverse();
}

BTreeNode* BTreeNode::search(int key) {
    int i = 0;
    while (i < keys.size() && key > keys[i])
        i++;

    if (i < keys.size() && key == keys[i])
        return this;

    if (is_leaf)
        return nullptr;

    return children[i]->search(key);
}

void BTree::insert(int key) {
    if (root == nullptr) {
        root = new BTreeNode(true);
        root->keys.push_back(key);
    } else {
        if (root->keys.size() == (2 * MIN_DEGREE - 1)) {
            BTreeNode* new_root = new BTreeNode(false);
            new_root->children.push_back(root);
            new_root->split_child(0, root);
            root = new_root;
        }

        root->insert_non_full(key);
    }
}

void BTree::traverse() {
    if (root != nullptr)
        root->traverse();
}

BTreeNode* BTree::search(int key) {
    if (root != nullptr)
        return root->search(key);
    return nullptr;
}

void BTree::remove(int key) {
    std::cout << "Deletion not implemented in this example." << std::endl;
}

int main() {
    BTree btree;

    int choice, key;
    BTreeNode* result = nullptr;

    do {
        std::cout << "1. Insert key\n";
        std::cout << "2. Search key\n";
        std::cout << "3. Print B-tree\n";
        std::cout << "4. Quit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Enter key to insert: ";
                std::cin >> key;
                btree.insert(key);
                break;

            case 2:
                std::cout << "Enter key to search: ";
                std::cin >> key;
                result = btree.search(key);
                if (result != nullptr)
                    std::cout << "Key " << key << " found in the B-tree." << std::endl;
                else
                    std::cout << "Key " << key << " not found in the B-tree." << std::endl;
                break;

            case 3:
                std::cout << "B-tree traversal: ";
                btree.traverse();
                std::cout << std::endl;
                break;

            case 4:
                std::cout << "Exiting program.\n";
                break;

            default:
                std::cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 4);

    return 0;
}
