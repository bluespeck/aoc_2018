#include <iostream>
#include <vector>

struct tree
{
    size_t numChildren;
    size_t numMeta;
    std::vector<tree*> children;
    std::vector<size_t> meta;
};

void parseTree(tree *current)
{
    std::cin >> current->numChildren;
    std::cin >> current->numMeta;
    for(size_t i = 0; i < current->numChildren; i++)
    {
        tree* node = new tree;
        parseTree(node);
        current->children.push_back(node);
    }
    for (size_t i = 0; i < current->numMeta; i++)
    {
        size_t meta;
        std::cin >> meta;
        current->meta.push_back(meta);
    }
}

void SumUpMetas(tree *node, size_t& sum)
{
    for (size_t i = 0; i < node->numMeta; i++)
        sum += node->meta[i];
    for (size_t i = 0; i < node->numChildren; i++)
        SumUpMetas(node->children[i], sum);
}

void SumUpNodes(tree* node, size_t& sum)
{
    if (node->numChildren == 0)
    {
        for (size_t i = 0; i < node->numMeta; i++)
            sum += node->meta[i];
    }
    else
    {
        for (size_t i = 0; i < node->numMeta; i++)
            if(node->numChildren > node->meta[i] - 1)
                SumUpNodes(node->children[node->meta[i] - 1], sum);
    }
}

void DestroyTree(tree* node)
{
    for (size_t i = 0; i < node->numChildren; i++)
        DestroyTree(node->children[i]);
    delete node;
}

int main()
{
    tree* root = new tree;
    parseTree(root);
    size_t sum = 0;
    SumUpMetas(root, sum);

    std::cout << sum << '\n';
    sum = 0;
    SumUpNodes(root, sum);
    std::cout << sum << '\n';

    DestroyTree(root);

    return 0;
}