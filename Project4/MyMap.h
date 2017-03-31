//// MyMap.h
//
//// Skeleton for the MyMap class template.  You must implement the first six
//// member functions.
//#ifndef MYMAP_H
//#define MYMAP_H
//
//template<typename KeyType, typename ValueType>
//class MyMap
//{
//public:
//    MyMap() {
//        this->m_root = nullptr;
//        this->m_size = 0;
//    }
//    
//    ~MyMap() {
//        this->deleteAllNodes(this->m_root);
//    }
//    
//    void clear() {
//        this->deleteAllNodes(this->m_root);
//        //deleteAllNodes should already have set the root to nullptr,
//        //but this is necessary for some reason
//        this->m_root = nullptr;
//        this->m_size = 0;
//    }
//    
//    int size() const {
//        return this->m_size;
//    }
//    
//    void associate(const KeyType& key, const ValueType& value) {
//        //increment the size
//        m_size++;
//        //if there's no elements so far
//        if (this->m_root == nullptr) {
//            this->m_root = new Node(key, value);
//            return;
//        }
//        //otherwise, perform the typical insertion algorithm
//        Node* curr = m_root;
//        //until the right place is found
//        while (true) {
//            //if the key is less than the current node's key
//            if (key == curr->key)  {
//                curr->value = value;
//                //decrement the size back down to its original size
//                m_size--;
//                return;
//            }
//            if (key < curr->key) {
//                //search the left subtree
//                if (curr->left != nullptr) curr = curr->left;
//                else {
//                    curr->left = new Node(key, value);
//                    return;
//                }
//            }
//            else if (key > curr->key) {
//                //otherwise search the right subtree
//                if (curr->right != nullptr) curr = curr->right;
//                else {
//                    curr->right = new Node(key, value);
//                    return;
//                }
//            }
//        }
//    }
//
//	  // for a map that can't be modified, return a pointer to const ValueType
//    const ValueType* find(const KeyType& key) const {
//        const Node* res = this->findNode(key, m_root);
//        return (res == nullptr) ? nullptr : &(res->value);
//    }
//
//	  // for a modifiable map, return a pointer to modifiable ValueType
//	ValueType* find(const KeyType& key)
//	{
//		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
//	}
//
//	  // C++11 syntax for preventing copying and assignment
//	MyMap(const MyMap&) = delete;
//	MyMap& operator=(const MyMap&) = delete;
//
//private:
//    struct Node {
//        KeyType key;
//        ValueType value;
//        Node* left;
//        Node* right;
//        Node(const KeyType& key, const ValueType& value) {
//            this->key = key;
//            this->value = value;
//            this->left = nullptr;
//            this->right = nullptr;
//        }
//    };
//    
//    //recursively find a node
//    Node* findNode(const KeyType& key, Node* node) const {
//        if (node == nullptr) return nullptr;
//        if (node->key == key) return node;
//        if (key < node->key) return findNode(key, node->left);
//        return findNode(key, node->right);
//    }
//    
//    //recursively delete all nodes using a postorder traversal
//    void deleteAllNodes(Node* node) {
//        if (node == nullptr) return;
//        deleteAllNodes(node->left);
//        deleteAllNodes(node->right);
//        delete node;
//        node = nullptr;
//        this->m_size--;
//    }
//    
//    Node* m_root;
//    int m_size;
//
//};
//
//#endif
////
////
////
////
////


#ifndef MYMAP_H
#define MYMAP_H
// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class MyMap
{
public:
    
    MyMap()
    {
        tree_root = nullptr; //start off as empty BST
        tree_size = 0;
    }
    
    ~MyMap()
    {
        // destructor; deletes all of the tree's nodes
        clear();
    }
    
    void clear()
    {
        // deletes all of the trees nodes producing an empty tree
        
        recursiveDump(tree_root);
        
        tree_root = nullptr;
        tree_size = 0;
    }
    
    int size() const
    {
        // return the number of associations in the map
        return tree_size;
    }
    
    
    // The associate method associates one item (key) with another (value). // If no association currently exists with that key, this method inserts // a new association into the tree with that key/value pair. If there is // already an association with that key in the tree, then the item
    // associated with that key is replaced by the second parameter (value). // Thus, the tree contains no duplicate keys.
    void associate(const KeyType& key, const ValueType& value)
    {
        associateNode(tree_root, key, value);
    }
    
    
    // for a map that can't be modified, return a pointer to const ValueType
    
    // If no association exists with the given key, return nullptr; otherwise,
    // return a pointer to the value associated with that key. This pointer can be // used to examine that value, and if the map is allowed to be modified, to
    // modify that value directly within the map (the second overload enables
    // this). Using a little C++ magic, we have implemented it in terms of the
    // first overload, which you must implement.
    const ValueType* find(const KeyType& key) const
    {
        Node* n = findNode(tree_root, key);
        
        if (n == nullptr)
        {
            //the key was not found in this tree...
            return nullptr;
        }
        
        return &(n->node_value);
    }
    
    // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
    }
    
    // C++11 syntax for preventing copying and assignment
    MyMap(const MyMap&) = delete;
    MyMap& operator=(const MyMap&) = delete;
    
private:
    
    class Node
    {
    public:
        KeyType node_key;
        ValueType node_value;
        Node* left;
        Node* right;
    };
    
    
    Node* tree_root; //BST root
    int tree_size;
    
    void associateNode(Node*& rootNode, const KeyType& key, const ValueType& value)
    {
        if (rootNode == nullptr)
        {
            rootNode = new Node;
            
            rootNode->node_key = key;
            rootNode->node_value = value;
            rootNode->left = nullptr;
            rootNode->right = nullptr;
            
            return;
        }
        
        //rootNode != nullptr
        
        if (key == rootNode->node_key)
        {
            rootNode->node_value = value; //if the key is already in the tree, we just replace its value
            return;
        }
        
        //we must determine which way to travel in the tree
        
        if (key < rootNode->node_key)
        {
            //the key is less than the key at rootNode, so we travel to the left child of the tree
            associateNode(rootNode->left, key, value);
            return;
        }
        
        if (key > rootNode->node_key)
        {
            //the key is greater than the key at rootNode, so we travel to the right child of the tree
            associateNode(rootNode->right, key, value);
            return;
        }
    }
    
    Node* findNode(Node* rootNode, const KeyType& key) const
    {
        //we want to find the key in the tree
        
        if (rootNode == nullptr) //we are either looking at an empty tree or we have come to a leaf, which means that the node is not
            //in this tree
        {
            return nullptr;
        }
        
        if (key == rootNode->node_key) //we check whether the key that we are looking for is the key at the node that we are currently on
        {
            return rootNode;
        }
        
        //the key is not the key at the node that we are currently on, so we have to move down the tree
        
        Node* res;
        if (key < rootNode->node_key)
        {
            //the key is less than the key at rootNode, so we travel to the left child of the tree
            res = findNode(rootNode->left, key);
        }
        
        if (key > rootNode->node_key)
        {
            //the key is greater than the key at rootNode, so we travel to the right child of the tree
            res = findNode(rootNode->right, key);
        }
        return res;
    }
    
    void recursiveDump(Node* rootNode)
    {
        if (rootNode == nullptr) //if the rootNode is a nullptr, there is nothing to delete
        {
            return;
        }
        
        recursiveDump(rootNode->left);
        recursiveDump(rootNode->right);
        
        delete rootNode;
        tree_size--;
    }
    
};

#endif
