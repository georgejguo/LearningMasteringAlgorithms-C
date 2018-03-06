#include <stdlib.h>
#include <string.h>

#include "bistree.h"

static void destroy_right(BisTree * tree, BiTreeNode * node);

static void rotate_left(BiTreeNode ** node)
{
	BiTreeNode *left, *grandchild;

	left = bitree_left(*node);

	if (((AvlNode *) bitree_data(left))->factor == AVL_LFT_HEAVY) {

		/// 执行 LL ( Left-Left ) 旋转操作

		bitree_left(*node) = bitree_right(left);
		bitree_right(left) = *node;
		((AvlNode *) bitree_data(*node))->factor = AVL_BALANCED;
		((AvlNode *) bitree_data(left))->factor = AVL_BALANCED;
		*node = left;

	} else {

		/// 执行 LR ( Left-Right ) 旋转操作

		grandchild = bitree_right(left);
		bitree_right(left) = bitree_left(grandchild);
		bitree_left(grandchild) = left;
		bitree_left(*node) = bitree_right(grandchild);
		bitree_right(grandchild) = *node;

		switch (((AvlNode *) bitree_data(grandchild))->factor) {

		case AVL_LFT_HEAVY:

			((AvlNode *) bitree_data(*node))->factor =
			    AVL_RGT_HEAVY;
			((AvlNode *) bitree_data(left))->factor = AVL_BALANCED;
			break;

		case AVL_BALANCED:

			((AvlNode *) bitree_data(*node))->factor = AVL_BALANCED;
			((AvlNode *) bitree_data(left))->factor = AVL_BALANCED;
			break;

		case AVL_RGT_HEAVY:

			((AvlNode *) bitree_data(*node))->factor = AVL_BALANCED;
			((AvlNode *) bitree_data(left))->factor = AVL_LFT_HEAVY;
			break;

		}

		((AvlNode *) bitree_data(grandchild))->factor = AVL_BALANCED;
		*node = grandchild;
	}

	return;
}

/**
 执行右旋转

 @param node 二叉树结点
 */
static void rotate_right(BiTreeNode ** node)
{
	BiTreeNode *right, *grandchild;

	right = bitree_right(*node);

	if (((AvlNode *) bitree_data(right))->factor == AVL_RGT_HEAVY) {

		/// 执行 RR ( Right-Right ) 旋转操作

		bitree_right(*node) = bitree_left(right);
		bitree_left(right) = *node;
		((AvlNode *) bitree_data(*node))->factor = AVL_BALANCED;
		((AvlNode *) bitree_data(right))->factor = AVL_BALANCED;
		*node = right;

	} else {

		/// 执行 RR ( Right-Right ) 旋转操作

		grandchild = bitree_left(right);
		bitree_left(right) = bitree_right(grandchild);
		bitree_right(grandchild) = right;
		bitree_right(*node) = bitree_left(grandchild);
		bitree_left(grandchild) = *node;

		switch (((AvlNode *) bitree_data(grandchild))->factor) {

		case AVL_LFT_HEAVY:

			((AvlNode *) bitree_data(*node))->factor = AVL_BALANCED;
			((AvlNode *) bitree_data(right))->factor =
			    AVL_RGT_HEAVY;
			break;

		case AVL_BALANCED:

			((AvlNode *) bitree_data(*node))->factor = AVL_BALANCED;
			((AvlNode *) bitree_data(right))->factor = AVL_BALANCED;
			break;

		case AVL_RGT_HEAVY:

			((AvlNode *) bitree_data(*node))->factor =
			    AVL_LFT_HEAVY;
			((AvlNode *) bitree_data(right))->factor = AVL_BALANCED;
			break;

		}

		((AvlNode *) bitree_data(grandchild))->factor = AVL_BALANCED;
		*node = grandchild;
	}

	return;
}

static void destroy_left(BisTree * tree, BiTreeNode * node)
{
	BiTreeNode **position;

	if (bitree_size(tree) == 0)
		return;

	if (node == NULL)
		position = &tree->root;
	else
		position = &node->left;

	if (*position != NULL) {
		destroy_left(tree, *position);
		destroy_right(tree, *position);

		if (tree->destroy != NULL) 
			tree->destroy(((AvlNode *) (*position)->data)->data);

		free((*position)->data);
		free(*position);
		*position = NULL;

		tree->size--;
	}
	return;
}

static void destroy_right(BisTree * tree, BiTreeNode * node)
{
	BiTreeNode **position;

	if (bitree_size(tree) == 0)
		return;

	if (node == NULL) 
		position = &tree->root;
	else
		position = &node->right;

	if (*position != NULL) {
		destroy_left(tree, *position);
		destroy_right(tree, *position);

		if (tree->destroy != NULL) 
			tree->destroy(((AvlNode *) (*position)->data)->data);

		free((*position)->data);
		free(*position);
		*position = NULL;

		tree->size--;
	}
	return;
}

static int insert(BisTree * tree, BiTreeNode ** node, const void *data,
		  int *balanced)
{
	AvlNode *avl_data;
	int cmpval, retval;

	if (bitree_is_eob(*node)) {
		if ((avl_data = (AvlNode *) malloc(sizeof(AvlNode))) == NULL)
			return -1;

		avl_data->factor = AVL_BALANCED;
		avl_data->hidden = 0;
		avl_data->data = (void *)data;

		return bitree_ins_left(tree, *node, avl_data);

	} else {
		cmpval =
		    tree->compare(data, ((AvlNode *) bitree_data(*node))->data);

		if (cmpval < 0) {
			if (bitree_is_eob(bitree_left(*node))) {
				if ((avl_data =
				     (AvlNode *) malloc(sizeof(AvlNode))) ==
				    NULL)
					return -1;

				avl_data->factor = AVL_BALANCED;
				avl_data->hidden = 0;
				avl_data->data = (void *)data;

				if (bitree_ins_left(tree, *node, avl_data) != 0)
					return -1;

				*balanced = 0;
			} else {
				if ((retval =
				     insert(tree, &bitree_left(*node), data,
					    balanced)) != 0)
					return retval;
			}

			if (!(*balanced)) {
				switch (((AvlNode *) bitree_data(*node))->
					factor) {

				case AVL_LFT_HEAVY:

					rotate_left(node);
					*balanced = 1;
					break;

				case AVL_BALANCED:

					((AvlNode *) bitree_data(*node))->
					    factor = AVL_LFT_HEAVY;
					break;

				case AVL_RGT_HEAVY:

					((AvlNode *) bitree_data(*node))->
					    factor = AVL_BALANCED;
					*balanced = 1;

				}
			}

		} else if (cmpval > 0) {

			/// 处理右子树

			if (bitree_is_eob(bitree_right(*node))) {

				if ((avl_data =
				     (AvlNode *) malloc(sizeof(AvlNode))) ==
				    NULL)
					return -1;

				avl_data->factor = AVL_BALANCED;
				avl_data->hidden = 0;
				avl_data->data = (void *)data;

				if (bitree_ins_right(tree, *node, avl_data) !=
				    0)
					return -1;

				*balanced = 0;

			} else {

				if ((retval =
				     insert(tree, &bitree_right(*node), data,
					    balanced)) != 0)
					return retval;
			}

			/// 确保树依然保持平衡
			if (!(*balanced)) {

				switch (((AvlNode *) bitree_data(*node))->
					factor) {

				case AVL_LFT_HEAVY:

					((AvlNode *) bitree_data(*node))->
					    factor = AVL_BALANCED;
					*balanced = 1;
					break;

				case AVL_BALANCED:

					((AvlNode *) bitree_data(*node))->
					    factor = AVL_RGT_HEAVY;
					break;

				case AVL_RGT_HEAVY:

					rotate_right(node);
					*balanced = 1;

				}
			}

		} else {

			/// 处理找到节点的情况

			///如果找到非隐藏节点直接返回 1
			if (!((AvlNode *) bitree_data(*node))->hidden)
				return 1;

			///插入新数据并取消隐藏
			if (tree->destroy != NULL) {
				/// 销毁隐藏节点的数据
				tree->destroy(((AvlNode *) bitree_data(*node))->
					      data);
			}

			((AvlNode *) bitree_data(*node))->data = (void *)data;
			((AvlNode *) bitree_data(*node))->hidden = 0;

			///如果是隐藏节点数据替换，则不需要再次平衡树
			*balanced = 1;
		}
	}

	return 0;
}

/**
 在 tree 所指定二叉树中隐藏一个 node 所指定结点

 @param tree 二叉搜索树
 @param node 指定的结点
 @param data 结点数据
 @return 成功返回0；否则返回-1
 */
static int hide(BisTree * tree, BiTreeNode * node, const void *data)
{
	int cmpval, retval;

	/// 没找到节点返回 -1
	if (bitree_is_eob(node))
		return -1;

	cmpval = tree->compare(data, ((AvlNode *) bitree_data(node))->data);

	if (cmpval < 0) {

		/// 处理左子树
		retval = hide(tree, bitree_left(node), data);

	} else if (cmpval > 0) {

		/// 处理右子树
		retval = hide(tree, bitree_right(node), data);

	} else {

		/// 将节点标记为隐藏
		((AvlNode *) bitree_data(node))->hidden = 1;
		retval = 0;
	}

	return retval;
}

/**
 在 tree 所指定二叉搜索树中查找 node 指定的节点

 @param tree 二叉搜索树
 @param node 指定的结点
 @param data 找到的结点数据
 @return 找到返回0；否则返回-1
 */
static int lookup(BisTree * tree, BiTreeNode * node, void **data)
{
	int cmpval, retval;

	/// 没找到节点返回 -1
	if (bitree_is_eob(node))
		return -1;

	cmpval = tree->compare(*data, ((AvlNode *) bitree_data(node))->data);

	if (cmpval < 0) {

		/// 处理左子树
		retval = lookup(tree, bitree_left(node), data);

	} else if (cmpval > 0) {

		/// 处理右子树
		retval = lookup(tree, bitree_right(node), data);

	} else {

		if (!((AvlNode *) bitree_data(node))->hidden) {

			/// 将找到的节点数据返回
			*data = ((AvlNode *) bitree_data(node))->data;
			retval = 0;

		} else {

			/// 没找到节点返回 -1
			return -1;
		}
	}

	return retval;
}

void bistree_init(BisTree * tree,
		  int (*compare) (const void *key1, const void *key2),
		  void (*destroy) (void *data))
{
	bitree_init(tree, destroy);
	tree->compare = compare;

	return;
}

void bistree_destroy(BisTree * tree)
{
	destroy_left(tree, NULL);

	memset(tree, 0, sizeof(BisTree));

	return;
}

int bistree_insert(BisTree * tree, const void *data)
{
	int balanced = 0;
	return insert(tree, &bitree_root(tree), data, &balanced);
}

int bistree_remove(BisTree * tree, const void *data)
{
	return hide(tree, bitree_root(tree), data);
}

int bistree_lookup(BisTree * tree, void **data)
{
	return lookup(tree, bitree_root(tree), data);

}
