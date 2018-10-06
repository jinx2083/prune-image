
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"
#include "stats.h"
#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"

#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <utility>

using namespace std;
using namespace cs221util;




/* given */
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
	{}

/* given */
twoDtree::~twoDtree(){
	clear();
}

/* given */
twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}

/* given */
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

   /**
    * Constructor that builds a twoDtree out of the given PNG.
    * Every leaf in the tree corresponds to a pixel in the PNG.
    * Every non-leaf node corresponds to a rectangle of pixels 
    * in the original PNG, represented by an (x,y) pair for the 
    * upper left corner of the rectangle and an (x,y) pair for 
    * lower right corner of the rectangle. In addition, the Node
    * stores a pixel representing the average color over the 
    * rectangle. 
    *
    * Every node's left and right children correspond to a partition
    * of the node's rectangle into two smaller rectangles. The node's
    * rectangle is split by the horizontal or vertical line that 
    * results in the two smaller rectangles whose sum of squared 
    * differences from their mean is as small as possible.
    *
    * The left child of the node will contain the upper left corner
    * of the node's rectangle, and the right child will contain the
    * lower right corner.
    *
   * This function will build the stats object used to score the 
   * splitting lines. It will also call helper function buildTree.
    */
twoDtree::twoDtree(PNG & imIn){ 

	//create stats object
	stats statsObj(imIn); 

	pair<int, int> ul(0,0);
	pair<int, int> lr(imIn.width()-1, imIn.height()-1);

	width = imIn.width();
	height = imIn.height();
	
	//call buildTree 	
	root = buildTree(statsObj, ul, lr);

}

typename twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr) {
	
	if(ul == lr){
		return new Node(ul, lr, s.getAvg(ul, lr));
	}

	Node* rootTemp = new Node(ul, lr, s.getAvg(ul, lr));
	buildTreeHelper(rootTemp, s, ul, lr);

	return rootTemp;
}


void twoDtree::buildTreeHelper(Node* subtree, stats&s, pair<int,int> ul, pair<int,int> lr){

	Node* rect1;
	Node* rect2;

	//cout<<"coordinates currently:( "<<ul.first<<", "<<ul.second<<") & ("<<lr.first<<", "<<lr.second<<")"<<endl;

	if(ul != lr){

		//cout<<__LINE__<<endl;

		pair<int,int> split = splitHelper(s, ul,lr);
		//cout<<__LINE__<<endl;

		if(split.first == lr.first){
			//horizontal split

			//rect1 = (ul.first, ul.second) & (lr.first, split.second)
			//rect2 = (ul.first, split.second+1) & (lr.first, lr.second) 
			//cout<<__LINE__<<endl;

			pair<int,int> rect1LR(lr.first, split.second);
			pair<int,int> rect2UL(ul.first, split.second+1); 



			//cout<<__LINE__<<endl;
			rect1 = new Node(ul, rect1LR, s.getAvg(ul, rect1LR));
			rect2 = new Node(rect2UL,lr, s.getAvg(rect2UL, lr));
			//cout<<__LINE__<<endl;

			
			subtree->left = rect1;
			buildTreeHelper(rect1, s, ul, rect1LR);
			subtree->right = rect2;
			buildTreeHelper(rect2, s, rect2UL, lr);

	
		} else {
			//vertical split


			pair<int, int> rect1LR(split.first, lr.second);
			pair<int, int> rect2UL(split.first+1, ul.second);


			rect1 = new Node(ul, rect1LR, s.getAvg(ul, rect1LR));
			rect2 = new Node(rect2UL, lr, s.getAvg(rect2UL, lr));


			subtree->left = rect1;
			buildTreeHelper(rect1, s, ul, rect1LR);

			subtree->right = rect2;
			buildTreeHelper(rect2, s, rect2UL, lr);

		}
	}
	
}




pair<int,int> twoDtree::splitHelper(stats&s, pair<int,int> ul, pair<int,int> lr){
	
	long minScore = LONG_MAX;
	pair<int, int> split = lr; //(0,0)

	//vertical split
	for(unsigned i = ul.first; i < lr.first; i++){

		pair<int,int> tempUpperLeft(i+1, ul.second);
		pair<int, int> tempLowerRight(i, lr.second);

		long score1 = s.getScore(ul, tempLowerRight);
		long score2 = s.getScore(tempUpperLeft, lr);

		if((score1+score2) < minScore){
			minScore = score1 + score2;
			split.first = i;
			split.second = lr.second;
		}
	}



	//get horizontal split
	for(unsigned i = ul.second; i < lr.second; i++){
		
		pair<int,int> tempLowerRight(lr.first, i);
		pair<int, int> tempUpperLeft(ul.first, i+1);

		long score1 = s.getScore(ul, tempLowerRight);
		long score2 = s.getScore(tempUpperLeft, lr);

		if((score1+score2) < minScore){
			minScore = score1 + score2;
			split.first = lr.first;
			split.second = i;
		}		
	}

	return split;

}



//}

   /*
    *  Prune function trims subtrees as high as possible in the tree.
    *  A subtree is pruned (cleared) if at least pct of its leaves are within 
    *  tol of the average color stored in the root of the subtree. 
    *  Pruning criteria should be evaluated on the original tree, not 
    *  on a pruned subtree. (we only expect that trees would be pruned once.)
    *  
    * You may want a recursive helper function for this one.
    */
void twoDtree::prune(double pct, int tol){

	if(root==NULL){
		return;

	}

	if(root->left == NULL && root->right == NULL){
		return;
	}

	pruneRecursive(root, pct, tol);

}


void twoDtree::pruneRecursive(Node* nd, double pct, int tol){

	//
	if(nd == NULL)
		return;

	if(nd != NULL && nd->left != NULL && root->right != NULL){

		//
		if(shouldPruneChildren(nd, pct, tol)){
			//prune subtrees
			clearHelper(nd->left);
			nd->left = NULL;
			clearHelper(nd->right);
			nd->right = NULL;
		}
		else {
			//prunceRecursive on children
			pruneRecursive(nd->left, pct, tol);
			pruneRecursive(nd->right, pct, tol);
		}


	}
}

bool twoDtree::shouldPruneChildren(Node* nd, double pct, int tol){

	int temptotal = 0;
	int tempTotalwithin = 0;

	int* totalLeafs = &temptotal;
	int* totalLeafsFallWithin = &tempTotalwithin;
	
	RGBAPixel subRootAvg = nd->avg;
	
	determineLeafsWithinTol(nd, subRootAvg, pct, tol, totalLeafs, totalLeafsFallWithin);


	float totalLeavesPct = float(*totalLeafsFallWithin)/float(*totalLeafs);

	if(totalLeafs == 0)
		false;

	if( totalLeavesPct >= pct){
		return true;
	}
	else {
		return false;
	}
}

void twoDtree::determineLeafsWithinTol(Node* nd, RGBAPixel subRootAvg, double pct, int tol, int *totalLeafs, int *totalLeafsFallWithin){

	if(nd == NULL){
		return;

	}
	else if(nd->left == NULL && nd->right == NULL){
		(*totalLeafs)++;
		//this is a leaf


		 long d = (nd->avg.r - subRootAvg.r)*(nd->avg.r - subRootAvg.r) + (nd->avg.g - subRootAvg.g)*(nd->avg.g - subRootAvg.g) + (nd->avg.b - subRootAvg.b)*(nd->avg.b - subRootAvg.b);
		 
		 if(d <= tol){
		 	(*totalLeafsFallWithin)++;
		 } 

	}
	else {
		determineLeafsWithinTol(nd->left, subRootAvg, pct, tol, totalLeafs, totalLeafsFallWithin);
		determineLeafsWithinTol(nd->right, subRootAvg, pct, tol, totalLeafs, totalLeafsFallWithin);
	}

}
   /**
    * Render returns a PNG image consisting of the pixels
    * stored in the tree. may be used on pruned trees. Draws
    * every leaf node's rectangle onto a PNG canvas using the 
    * average color stored in the node.
    */
PNG twoDtree::render(){
	/* your code here */
	PNG*  img = new PNG(width, height);
	traverse(img, root);

	return *img;

}


void twoDtree::traverse(PNG* img, Node* nd){

	//if node's left and right child are null
	if(nd == NULL)
		return;

	if(nd->left == NULL && nd->right == NULL){
		//it is a leaf by defintion
		for(int y = nd->upLeft.second; y < nd->lowRight.second+1; y++){
			for(int x = nd->upLeft.first; x < nd->lowRight.first+1; x++){

				RGBAPixel * px = img->getPixel(x,y); 
		 		px->r = nd->avg.r;
		 		px->g = nd->avg.g;
		 		px->b = nd->avg.b;
			}
		}
	} else {
		traverse(img, nd->left);
		traverse(img, nd->right);
	}
}

/**
   * Copies the parameter other twoDtree into the current twoDtree.
   * Does not free any memory. Called by copy constructor and op=.
   * You may want a recursive helper function for this one.
   * @param other The twoDtree to be copied.
   */
void twoDtree::copy(const twoDtree & orig){
    if (orig.root == NULL){
        root = NULL;
    }
    else {
        Node * p1 = orig.root;
        root = copyRecursive(root,p1);
        width = orig.width;
        height = orig.height;

    };
}

twoDtree::Node* twoDtree::copyRecursive(Node *root, Node *orig) {
    //base case: null node
    if (orig==NULL){
        return NULL;
    }
    else {
        root = new Node(orig->upLeft,orig->lowRight,orig->avg);
        root->left = copyRecursive(root->left,orig->left);
        root->right = copyRecursive(root->right,orig->right);
        return root;
    }
}

   /**
    * Destroys all dynamically allocated memory associated with the
    * current twoDtree class. Complete for PA3.
   * You may want a recursive helper function for this one.
    */
void twoDtree::clear() {
    //todo
    width = 0;
    height = 0;
    if(root ==NULL){
        return;
    }
    else clearHelper(root);
	/* your code here */
}


void twoDtree::clearHelper(twoDtree::Node *&nd) {
	
    if (nd!=NULL){
        
        clearHelper(nd->left);
        nd->left =NULL;
        
        clearHelper(nd->right);
        nd->right = NULL;
        
        delete nd;
    }
}




