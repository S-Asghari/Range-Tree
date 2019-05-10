#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int n;
const int MAX = 10002;
pair <double, double> coordinate [MAX];
pair <double, double> coordinateSbyX [MAX];
int operation_num;

pair <double, double> temp[MAX];	//Just defined for sorting!


void merge(int start, int end, pair <double, double>* Coordinate)
{
	int mid = (start + end)/2;
	int i = start, j = mid, p = start;
	while( (i < mid) || (j < end) )	
	{
		if( (j == end) || ( (i < mid) && (Coordinate[i].second < Coordinate[j].second) ) )
		{
			temp[p] = Coordinate[i];
			p++;
			i++;
		}
		else
		{
			temp[p] = Coordinate[j];
			p++;
			j++;
		}
	}
	
	for(int k = start; k < end; k++)
		Coordinate[k] = temp[k];
}


void sort(int start, int end, pair <double, double>* Coordinate)
{
	if(start + 1 == end)
		return;
	sort(start, (start + end)/2, Coordinate);
	sort((start + end)/2, end, Coordinate);	
	merge(start, end, Coordinate);
}

void mergeX(int start, int end, pair <double, double>* CoordinateSbyX)
{
	int mid = (start + end)/2;
	int i = start, j = mid, p = start;
	while( (i < mid) || (j < end) )	
	{
		if( (j == end) || ( (i < mid) && (CoordinateSbyX[i].first < CoordinateSbyX[j].first) ) )
		{
			temp[p] = CoordinateSbyX[i];
			p++;
			i++;
		}
		else
		{
			temp[p] = CoordinateSbyX[j];
			p++;
			j++;
		}
	}
	
	for(int k = start; k < end; k++)
		CoordinateSbyX[k] = temp[k];
}


void sortX(int start, int end, pair <double, double>* CoordinateSbyX)
{
	if(start + 1 == end)
		return;
	sortX(start, (start + end)/2, CoordinateSbyX);
	sortX((start + end)/2, end, CoordinateSbyX);	
	mergeX(start, end, CoordinateSbyX);
}

class node_1D
{
	public:
		double x;
		double y;
		node_1D * right;
		node_1D * left;
		node_1D * parent;
		double max;
};

class node_2D
{
	public:
		double val;
		node_2D * right;
		node_2D * left;
		node_2D * parent;
		node_1D * secondary;
		double max;
};

node_1D * Build_1D_RT(pair <double, double>* Coordinate, int N)
{
	queue <node_1D*> Queue;
	for(int i = 0; i < N; i++)
	{
		node_1D * newNode = new node_1D;
		newNode->x = Coordinate[i].first;
		newNode->y = Coordinate[i].second;
		newNode->max = Coordinate[i].second;
		newNode->left = NULL;
		newNode->right = NULL;
		newNode->parent = NULL;
		
		Queue.push(newNode);
	}
	while(Queue.size() != 1)
	{
		node_1D * node1 = new node_1D;
		node_1D * node2 = new node_1D;
		node_1D * root = new node_1D;
		node1 = Queue.front();
		Queue.pop();
		node2 = Queue.front();
		Queue.pop();
		
		if(node1->y > node2->y)
		{
			Queue.push(node1);
			node1 = node2;
			node2 = Queue.front();
			Queue.pop();
		}
		
		root->left = node1;
		root->right = node2;
		root->y = node1->max;
		root->max = node2->max;
			
		node1->parent = root;
		node2->parent = root;
		Queue.push(root);
	}
	return Queue.front();
}


node_2D * Build_2D_RT(pair <double, double> * Coordinate, pair <double, double> * CoordinateSbyX, int N)
{
	node_1D * P = new node_1D;
	P = Build_1D_RT(Coordinate, N);
	node_2D * v = new node_2D;
	if(P->left == NULL && P->right == NULL)
	{
		v->val = P->x;
		v->left = NULL;
		v->right = NULL;
		v->max = P->x;
		v->secondary = P;
	}
	else
	{
		int mid = (N-1)/2;
		double Xmid = CoordinateSbyX[mid].first;
		pair <double, double> split_SbyY_left[MAX/2];
		pair <double, double> split_SbyY_right[MAX/2];
		
		int index1 = 0, index2 = 0;
		for(int i = 0; i < N; i++)
		{
			if(Coordinate[i].first <= Xmid)
				split_SbyY_left[index1++] = Coordinate[i];
			else
				split_SbyY_right[index2++] = Coordinate[i];
		}
		///////////
		pair <double, double> split_SbyX_left[MAX/2];
		pair <double, double> split_SbyX_right[MAX/2];
		for(int i = 0; i <= mid; i++)
		{
			split_SbyX_left[i] = CoordinateSbyX[i];
		}
		for(int i = mid + 1; i < N; i++)
		{
			split_SbyX_right[i-mid-1] = CoordinateSbyX[i];
		}
		///////////
		v->val = Xmid;
		v->left = Build_2D_RT(split_SbyY_left, split_SbyX_left, mid+1);
		v->right = Build_2D_RT(split_SbyY_right, split_SbyX_right, N-mid-1);
		v->max = v->right->max;
		v->left->parent = v;
		v->right->parent = v;
		v->secondary = P;
	}
	return v;
}

node_2D * Find_Split_Node(node_2D * root, double x1, double x2)
{
	node_2D * Proot = root;
	while( ( (x1 <= Proot->val  && x2 <= Proot->val) || (x1 > Proot->val && x2 > Proot->val) ) &&
			 (Proot->left != NULL && Proot->right != NULL))
	{
		if(x1 <= Proot->val  && x2 <= Proot->val)
			Proot = Proot->left;
		else
			Proot = Proot->right;		
	}
	return Proot;
}

node_1D * Find_Split_Node(node_1D * root, double y1, double y2)
{
	node_1D * Proot = root;
	while( ( (y1 <= Proot->y  && y2 <= Proot->y) || (y1 > Proot->y && y2 > Proot->y) ) &&
			 (Proot->left != NULL && Proot->right != NULL))
	{
		if(y1 <= Proot->y  && y2 <= Proot->y)
			Proot = Proot->left;
		else
			Proot = Proot->right;		
	}
	return Proot;
}

void Range_Query1D(node_1D * root, double y1, double y2, vector <pair <double, double> > &result)
{
	node_1D * Vsplit = Find_Split_Node(root, y1, y2);
	
	if(Vsplit->left == NULL && Vsplit->right == NULL)
	{
		if(Vsplit->y >= y1 && Vsplit->y <= y2)
		{
			result.push_back(make_pair(Vsplit->x, Vsplit->y));
		}
	}
	else
	{
		node_1D * v = Vsplit->left;
		while(v->left != NULL && v->right != NULL)
		{
			if(y1 <= v->y)
			{
				result.push_back(make_pair(v->x, v->y));
				v = v->left;
			}
			else
			{
				v = v->right;
			}
		}
		if(v->y >= y1 && v->y <= y2)
			result.push_back(make_pair(v->x, v->y));
			
	///////////////////////
		
		node_1D * w = Vsplit->right;
		while(w->left != NULL && w->right != NULL)
		{
			if(w->y <= y2)
			{
				result.push_back(make_pair(w->x, w->y));
				w = w->right;
			}
			else
			{
				w = w->left;
			}
		}
		if(w->y >= y1 && w->y <= y2)
			result.push_back(make_pair(w->x, w->y));	
			
	}
}

vector < pair <double, double> > Range_Query2D(node_2D * root, double x1, double x2, double y1, double y2)
{
	node_2D * Vsplit = Find_Split_Node(root, x1, x2);
	
	vector < pair <double, double> > result;
	
	if(Vsplit->left == NULL && Vsplit->right == NULL)
	{
		if(Vsplit->val >= x1 && Vsplit->val <= x2)
			if(Vsplit->secondary->y >= y1 && Vsplit->secondary->y <= y2)
				
				result.push_back(make_pair(Vsplit->val, Vsplit->secondary->y));
	}
	else
	{
		node_2D * v = Vsplit->left;
		while(v->left != NULL && v->right != NULL)
		{
			if(x1 <= v->val)
			{
				Range_Query1D(v->right->secondary, y1, y2, result);
				v = v->left;
			}
			else
			{
				v = v->right;
			}
		}
		if(v->val >= x1 && v->val <= x2)
			if(v->secondary->y >= y1 && v->secondary->y <= y2)
			{	
				result.push_back(make_pair(v->val, v->secondary->y));
			}
			
	///////////////////////
		
		node_2D * w = Vsplit->right;
		while(w->left != NULL && w->right != NULL)
		{
			if(w->val <= x2)
			{
				Range_Query1D(w->left->secondary, y1, y2, result);
				w = w->right;
			}
			else
			{
				w = w->left;
			}
		}
		if(w->val >= x1 && w->val <= x2)
			if(w->secondary->y >= y1 && w->secondary->y <= y2)
			{	
				result.push_back(make_pair(w->val, w->secondary->y));
			}	
			
	}
	return result;
}

void print(node_1D * root)
{
	if(root->left != NULL && root->right != NULL)
	{
		cout << root->y << ' ' << root->left->y << ' ' << root->right->y << endl;
		print(root->left);
		print(root->right);
	}
}

void print(node_2D * root)
{
	if(root->left != NULL && root->right != NULL)
	{
		cout << root->val << ' ' << root->left->val << ' ' << root->right->val << endl;
		cout << "***" << endl;
		print(root->secondary);
		cout << "***" << endl;
		print(root->left);
		print(root->right);
	}
}


int main()
{
	cin >> n;
	int counter = 0;
	while(counter != n)
	{
		cin >> coordinate[counter].first;
		coordinateSbyX[counter].first = coordinate[counter].first;
		counter++;
	}
	
	counter = 0;
	while(counter != n)
	{
		cin >> coordinate[counter].second;
		coordinateSbyX[counter].second = coordinate[counter].second;
		counter++;
	}
	
	sort(0, n, coordinate);
	sortX(0, n, coordinateSbyX);
	
	node_1D * root = new node_1D;
	root = Build_1D_RT(coordinate, n);

	node_2D * primary_root = new node_2D;
	primary_root = Build_2D_RT(coordinate, coordinateSbyX, n);
	
	//print(primary_root);
	
	cin >> operation_num;
	double x1, x2, y1, y2;
	while(operation_num--)
	{
		cin >> x1 >> y1 >> x2 >> y2; 
		
		vector < pair <double, double> > result = Range_Query2D(primary_root, x1, x2, y1, y2);
		int m = result.size();
		if(m == 0)
		{
			cout << "None" << endl;
		}
		else
		{
			pair <double, double> RES [MAX];
			for(int i = 0 ; i < result.size(); i++)
			{
				RES[i] = result[i];
			}
			sort(0, m, RES);
			for(int i = 0; i < m; i++)
			{
				cout << RES[i].first << ' ';
			}
			cout << endl;
			for(int i = 0; i < m; i++)
			{
				cout << RES[i].second << ' ';
			}
			cout << endl;
		}
	}
	
	return 0;
}

