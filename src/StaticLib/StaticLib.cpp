#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>

#include "../include/lib_func.h"


// 2分木の初期化
void initialize(tree* t)
{
	if (t == NULL) return;

	t->root = NULL;
}

static void release_recursive(node* n)
{
	if (n == NULL) return;

	if (n->right) {
		release_recursive(n->right);
		n->right = NULL;
	}

	if (n->left) {
		release_recursive(n->left);
		n->left = NULL;
	}

	free(n);
}

// 使用メモリの全解放
void finalize(tree* t)
{
	if (t == NULL) return;

	release_recursive(t->root);
	t->root = NULL;
}

// keyの値を見てノードを追加する
static node* generate(int key, const char* value)
{
	node* p = (node*)malloc(sizeof(node));

	if (p == NULL) return NULL;

	p->key = key;
	int n = (int)strlen(value);
	memcpy(p->value, value, strlen(value) + 1);

	p->left = p->right = NULL;

	return p;
}

// keyの値を見てノードを追加する
bool add(tree* t, int key, const char* value)
{
	if (t == NULL) return false;

	node* p = generate(key, value);
	if (p == NULL) return false;// メモリ確保できなかった。

	if (t->root == NULL) {
		t->root = p;
		return true;
	}

	node* cur = t->root;
	node* parent = NULL;

	while (cur != NULL) {
		parent = cur;
		if (key < cur->key) {
			cur = cur->left;
		}
		else if (key > cur->key) {
			cur = cur->right;
		}
		else {
			// 同じキーなら値を上書き
			memcpy(cur->value, value, strlen(value) + 1);
			free(p);
			return true;
		}
	}

	if (key < parent->key) {
		parent->left = p;
	}
	else {
		parent->right = p;
	}
	// Todo: t->rootの下にkeyの値の大小でleftかrightを切り替えながらpを追加する処理を実装する

	return true;
}

// keyの値を見てノードを検索して、値を取得する
const char* find(const tree* t, int key)
{
	// ToDo: 実装する
	if (t == NULL) return NULL;

	node* cur = t->root;
	while (cur != NULL) {
		if (key < cur->key) {
			cur = cur->left;
		}
		else if (key > cur->key) {
			cur = cur->right;
		}
		else {
			return cur->value;
		}
	}

	return NULL;
}

// keyの小さな順にコールバック関数funcを呼び出す
static void inorder(const node* n, void (*func)(const node* p))
{
	if (n == NULL) return;
	inorder(n->left, func);
	func(n);
	inorder(n->right, func);
}

void search(const tree* t, void (*func)(const node* p))
{
	if (t == NULL || func == NULL) return;
	inorder(t->root, func);
}