#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char bool;
#define true 1
#define false 0

typedef struct rectangle {
	int minX, minY;
	int maxX, maxY;
}rectangle;

typedef struct node {
	rectangle rect;
	struct node* parent;
	struct node** children;
	bool isMBL;
	int childrenCount;
}node;

int minChildrenCount = 1;
int maxChildrenCount = 2;

rectangle GenerateRectangle(int minX, int minY, int maxX, int maxY) {
	rectangle rect;
	rect.minX = minX;
	rect.minY = minY;
	rect.maxX = maxX;
	rect.maxY = maxY;
	return rect;
}

rectangle ResetRectangle(rectangle origin, rectangle target) {
	int minX = origin.minX < target.minX ? origin.minX : target.minX;
	int minY = origin.minY < target.minY ? origin.minY : target.minY;
	int maxX = origin.maxX > target.maxX ? origin.maxX : target.maxX;
	int maxY = origin.maxY > target.maxY ? origin.maxY : target.maxY;
	return GenerateRectangle(minX, minY, maxX, maxY);
}

void PrintRectangle(rectangle rect) {
	printf("minX[%d], minY[%d], maxX[%d], maxY[%d]\n",
			rect.minX, rect.minY,
			rect.maxX, rect.maxY);
}

int GetBound(rectangle rect) {
	int distanceX = rect.maxX - rect.minX;
	int distanceY = rect.maxY - rect.minY;
	return distanceX * distanceY;
}

int CalculateBound(rectangle origin, rectangle target) {
	int minX = origin.minX < target.minX ? origin.minX : target.minX;
	int minY = origin.minY < target.minY ? origin.minY : target.minY;
	int maxX = origin.maxX > target.maxX ? origin.maxX : target.maxX;
	int maxY = origin.maxY > target.maxY ? origin.maxY : target.maxY;
	return GetBound(GenerateRectangle(minX, minY, maxX, maxY));
}

node** AllocChildrenChildren() {
	node** newChildren = (node**)malloc(sizeof(node*) * maxChildrenCount);
	memset(newChildren, 0, sizeof(node*) * maxChildrenCount);
	return newChildren;
}

node* AllocNode(rectangle rect, bool isMBL) {
	node* newNode = (node*)malloc(sizeof(node));
	memset(newNode, 0, sizeof(node));

	newNode->rect.minX = rect.minX;
	newNode->rect.minY = rect.minY;
	newNode->rect.maxX = rect.maxX;
	newNode->rect.maxY = rect.maxY;

	newNode->isMBL = isMBL;
	newNode->childrenCount = 0;

	if (newNode->isMBL == true)
		newNode->children = AllocChildrenChildren();

	return newNode;
}

int ChooseMBLIndex(node* node, rectangle rect) {
	int index;
	int i = 0;
	int minBound = 0;
	int currentBound = 0;

	index = 0;
	minBound = CalculateBound(node->children[index]->rect, rect);
	for (i = 1; i < node->childrenCount; i++) {
		currentBound = CalculateBound(node->children[i]->rect, rect);
		if (currentBound < minBound) {
			index = i;
			minBound = currentBound;
		}
	}

	return index;
}

void InsertNode(node* head, rectangle rect) {
	int index;

	if (head->childrenCount > 0) {
		if (head->children[0]->isMBL == true) {
			index = ChooseMBLIndex(head, rect);
			return InsertNode(head->children[index], rect);
		}
	}

	if (head->childrenCount > maxChildrenCount) {
		// TODO Split
	}

	head->rect = ResetRectangle(head->rect, rect);
	head->children[head->childrenCount] = AllocNode(rect, false);
	head->childrenCount++;
}

void PrintNode(node* head) {
	int i;

	PrintRectangle(head->rect);
	for (i = 0; i < head->childrenCount; i++) {
		PrintNode(head->children[i]);
	}
}


int main(void) {
	node* head = AllocNode(GenerateRectangle(0, 0, 0, 0), true);
	InsertNode(head, GenerateRectangle(1, 1, 3, 3));
	InsertNode(head, GenerateRectangle(0, 0, 5, 5));

	PrintNode(head);

	return 0;
}