# Problem

The task is to implement the function `uint64_t solve(const TreeProblem&)`, which returns the largest number of gifts that agents can protect in the best possible deployment. The problem instance is captured in the `TreeProblem` structure, which contains the following attributes:

- `max_group_size`: The maximum allowed size of the group of agents. This value is equal to 2 only in the bonus test. Otherwise, the value is always 1.
- `gifts`: A vector representing the number of gifts under every tree. It follows that the number of items in the vector also implies the number of Christmas trees in Whoville.
- `connections`: A vector of pairs representing which trees are at the opposite ends of a street. If both trees are guarded simultaneously, the guarding agents are supposed to be parts of the same group. Recall that Whoville’s streets are in the shape of a tree.
