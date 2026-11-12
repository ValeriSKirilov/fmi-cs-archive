//
// Created by Lenovo on 29.12.2025 г..
//

#include <iostream>
#include <stack>
#include <limits>
#include <vector>

const int MIN = INT_MIN, MAX = INT_MAX;

template <class T>
class BinarySearchTree
{
private:
    struct BinTreeNode
    {
        T value;
        BinTreeNode *left, *right;

        BinTreeNode(
            const T& val = T{},
            BinTreeNode* left = nullptr,
            BinTreeNode* right = nullptr
        ) : value(val), left(left), right(right)
        {
        }
    };

public:
    BinarySearchTree() noexcept
        : root(nullptr)
    {
    }

    BinarySearchTree(const BinarySearchTree& other)
        : root(copy(other.root))
    {
    }

    BinarySearchTree(BinarySearchTree&& other) noexcept
        : root(other.root)
    {
        other.root = nullptr;
    }

    BinarySearchTree& operator=(const BinarySearchTree& other)
    {
        if (this != &other)
        {
            deallocate(root);
            root = copy(other.root);
        }

        return *this;
    }

    BinarySearchTree& operator=(BinarySearchTree&& other) noexcept
    {
        if (this != &other)
        {
            deallocate(root);
            root = other.root;
            other.root = nullptr;
        }

        return *this;
    }

    ~BinarySearchTree()
    {
        deallocate(root);
    }

    int get_depth()
    {
        return get_depth(root);
    }

    int get_depth(const BinTreeNode* root)
    {
        if (root == nullptr)
            return 0;

        return 1 + std::max(
            get_depth(root->left),
            get_depth(root->right)
        );
    }

    bool contains(const T& element)
    {
        return contains(root, element);
    }

    void erase(const T& element)
    {
        return erase(root, element);
    }

    void print_inorder() const
    {
        print_inorder(root);
        std::cout << std::endl;
    }

    T extract_kth(int k) const
    {
        int cnt = 0;

        for (auto it = begin(); it != end(); ++it)
        {
            ++cnt;
            if (cnt == k)
                return *it;
        }

        return T{};
    }

    void balance()
    {
        std::vector<T> arr;
        for (auto it = begin(); it != end(); ++it)
            arr.push_back(*it);
        deallocate(root);

        root = create_tree(arr, 0, arr.size() - 1);

        return;
    }

    bool is_valid() const
    {
        return is_valid(root, MIN, MAX);
    }

    class ConstIterator
    {
    public:
        ConstIterator() = default;

    private:
        std::stack<const BinTreeNode*> stack;
        const BinarySearchTree<T>* tree_ref = nullptr;
        int advance_cnt = 0;

        explicit ConstIterator(const BinTreeNode* start, const BinarySearchTree<T>* ref) : tree_ref(ref)
        {
            push_left(start);
        }

        void push_leff(const BinTreeNode* node)
        {
            while (node)
            {
                stack.push(node);
                node = node->left;
            }
        }

        void advance()
        {
            if (stack.empty())
                return;

            ++advance_cnt;
            const BinTreeNode* node = stack.top();
            stack.pop();

            if (node->right)
                push_leff(node->right);
        }

        friend class BinarySearchTree<T>;
    };

    ConstIterator begin() const
    {
        return ConstIterator(root, this);
    }

    ConstIterator end() const
    {
        return ConstIterator(nullptr, this);
    }

    ConstIterator cbegin() const { return begin(); }
    ConstIterator cend() const { return end(); }

private:
    BinTreeNode* root;

    static BinTreeNode* copy(const BinTreeNode* root)
    {
        if (root == nullptr)
            return nullptr;

        return new BinTreeNode(
            root->value,
            copy(root->left),
            copy(root->right)
        );
    }

    static void deallocate(BinTreeNode*& root)
    {
        if (root == nullptr)
            return;

        deallocate(root->left);
        deallocate(root->right);

        delete root;
        root = nullptr;
    }

    static bool find(BinTreeNode* root, const T& element)
    {
        if (root == nullptr)
            return false;

        if (root->value == element)
            return true;

        if (root->value < element)
            return find(root->right, element);

        return find(root->left, element);
    }

    static void insert(BinTreeNode*& root, const T& element)
    {
        if (root == nullptr)
        {
            root = new BinTreeNode(element);
            return;
        }

        if (root->value < element)
            insert(root->right, element);
        else
            insert(root->left, element);
    }

    static void erase(BinTreeNode*& root, const T& element)
    {
        if (root == nullptr)
            return;

        if (root->value > element)
        {
            erase(root->left, element);
            return;
        }

        if (root->value < element)
        {
            erase(root->right, element);
            return;
        }

        const BinTreeNode* toDelete = root;
        if (root->left == nullptr && root->right == nullptr)
            root = nullptr;
        else if (root->left && root->right == nullptr)
            root = root->left;
        else if (root->left == nullptr && root->right)
            root = root->right;
        else
        {
            BinTreeNode* min_leaf = extract_min(root->right);
            min_leaf->left = root->left;
            min_leaf->right = root->right;
            root = min_leaf;
        }

        delete toDelete;
    }

    static BinTreeNode* extract_min(BinTreeNode*& root)
    {
        if (root->left)
            return extract_min(root->left);

        BinTreeNode* min_leaf = root;
        root = root->right;
        return min_leaf;
    }

    static void print_inorder(const BinTreeNode* root)
    {
        if (root == nullptr)
            return;

        print_inorder(root->left);
        std::cout << root->value << ' ';
        print_inorder(root->right);
    }

    bool is_valid(const BinTreeNode* root, int MIN, int MAX)
    {
        if (root == nullptr)
            return true;

        return MIN < root->value && root->value < MAX
            && is_valid(root->left, MIN, root->value)
            && is_valid(root->right, root->value, MAX);
    }

    BinTreeNode* create_tree(const std::vector<T>& arr, const int start, const int end)
    {
        if (arr.empty())
            return nullptr;

        if (end < start)
            return nullptr;

        int min_idx = start + (end - start) / 2;

        return new BinTreeNode(
            arr[min_idx],
            create_tree(arr, start, min_idx - 1),
            create_tree(arr, min_idx + 1, start)
        );
    }
};

int main()
{
    return 0;
}
