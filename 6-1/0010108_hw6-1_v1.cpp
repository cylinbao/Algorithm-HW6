#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <map>
using namespace std;

#define VERTEXMAX 50
#define white 0
#define black 1
//#define Debug3

class vertex{
	public:
		vertex(int arg1){index = arg1; state = white; preVtx = -1;}
		int index, state, preVtx;
		map<int, int> edgeList;//use endIdx as key and weight as value
};

vector<vertex*> vtxVec; 
stack<int> augPath;
int resMtx[VERTEXMAX][VERTEXMAX];
int caseNum, vertexNum, edgeNum, source, target;

void iniResMtx();
void iniVtxVec();
int calMaxFlow();//return max flow
int findAugPath();//return 0 if there is no augmenting path

int main(int argc, char *argv[])
{
	int i, j, begin, end, weight, output;

	cin >> caseNum;

	for(i=caseNum; i>0; i--){
		cin >> vertexNum >> edgeNum >> source >> target;
		iniResMtx();
		iniVtxVec();
		for(j=edgeNum; j>0; j--){
			cin >> begin >> end >> weight;
			resMtx[begin][end] = weight;
			vtxVec[begin]->edgeList[end] = weight;
			#ifdef Debug1
			cout << "resMtx[" << begin << "][" << end << "] = " << resMtx[begin][end]
					 << endl;
			cout << "vtcVec[" << begin << "]->edgeList[" << end << "] = "
					 << vtxVec[begin]->edgeList[end] << endl;
			#endif
		}
		output = calMaxFlow();
		cout << output << endl;
	}

	return 0;
}

int calMaxFlow()
{
	int augFlow, maxFlow, vtxIdx1, vtxIdx2;
	vertex *p_vtx;
	map<int, int>::iterator it;

	augFlow = findAugPath();
	#ifdef Debug2
		cout << augFlow << endl;
		while(augPath.size() > 0){
			cout << augPath.top() << endl;
			augPath.pop();
		}
	#endif
	while(augFlow != 0){
		vtxIdx1 = source;
		while(augPath.size() > 0){
			vtxIdx2 = augPath.top();
			augPath.pop();

			p_vtx = vtxVec[vtxIdx1];
			#ifdef Debug3
			cout << "Path: " << vtxIdx1 << " to " << vtxIdx2 << " weight= " 
					 << p_vtx->edgeList[vtxIdx2] << endl;
			#endif
			if(p_vtx->edgeList[vtxIdx2] > augFlow){
				p_vtx->edgeList[vtxIdx2] -= augFlow;
				#ifdef Debug3
				cout << "Change Path: " << vtxIdx1 << " to " << vtxIdx2 << " weight= " 
					 	 << p_vtx->edgeList[vtxIdx2] << endl;
				#endif
			}
			else{
				p_vtx->edgeList.erase(p_vtx->edgeList.find(vtxIdx2));
				#ifdef Debug3
				if(p_vtx->edgeList.find(vtxIdx2) == p_vtx->edgeList.end())
					cout << "Delete Path: " << vtxIdx1 << " to " << vtxIdx2 << endl;
				else
					cout << "Delete Path: " << vtxIdx1 << " to " << vtxIdx2 
							 << " fail" << endl;
				#endif
			}

			p_vtx = vtxVec[vtxIdx2];
			if(p_vtx->edgeList.find(vtxIdx1) == p_vtx->edgeList.end()){
				p_vtx->edgeList[vtxIdx1] = augFlow;
				#ifdef Debug3
				cout << "Add Path: " << vtxIdx2 << " to " << vtxIdx1 << " weight= " 
					 	 << p_vtx->edgeList[vtxIdx1] << endl;
				#endif
			}
			else{
				p_vtx->edgeList[vtxIdx1] += augFlow;
				#ifdef Debug3
				cout << "Chnage Path: " << vtxIdx2 << " to " << vtxIdx1 << " weight= " 
					 	 << p_vtx->edgeList[vtxIdx1] << endl;
				#endif
			}

			vtxIdx1 = vtxIdx2;
		}

		augFlow = findAugPath();
		#ifdef Debug3
		cout << "Find aug Flow: " << augFlow << endl;
		#endif
	}

	maxFlow = 0;
	p_vtx = vtxVec[target];
	for(it=p_vtx->edgeList.begin(); it!=p_vtx->edgeList.end(); it++){
		maxFlow += it->second;
	}

	return maxFlow;
}

int findAugPath()
{
	int augFlow(0), vtxIdx, preIdx;
	bool flag(false);
	vertex *p_vtx1, *p_vtx2;
	queue<vertex*> bfsQ, pushRecQ;
	map<int, int>::iterator it;

	bfsQ.push(vtxVec[source]);
	vtxVec[source]->state = black;
	pushRecQ.push(vtxVec[source]);

	while(bfsQ.size() > 0){
		p_vtx1 = bfsQ.front();
		bfsQ.pop();
		for(it=p_vtx1->edgeList.begin(); it!=p_vtx1->edgeList.end(); it++){
			p_vtx2 = vtxVec[it->first];	
			if(p_vtx2->index == target){
				p_vtx2->preVtx = p_vtx1->index;
				flag = true;
				augFlow = it->second;
				pushRecQ.push(p_vtx2);
				break;
			}
			else if(p_vtx2->state == white){
				p_vtx2->state = black;
				p_vtx2->preVtx = p_vtx1->index;
				bfsQ.push(p_vtx2);
				pushRecQ.push(p_vtx2);
			}
		}
		if(flag == true)
			break;
	}

	while(augPath.size() > 0){
		augPath.pop();
	}

	if(flag == true){
		p_vtx1 = vtxVec[target];
		while(p_vtx1->preVtx != -1){
			preIdx = p_vtx1->preVtx;
			if(augFlow > vtxVec[preIdx]->edgeList[p_vtx1->index])
				augFlow = vtxVec[preIdx]->edgeList[p_vtx1->index];

			augPath.push(p_vtx1->index);	
			p_vtx1 = vtxVec[preIdx];
		}
	}

	while(pushRecQ.size() > 0){
		p_vtx1 = pushRecQ.front();
		pushRecQ.pop();
		p_vtx1->state = white;
		p_vtx1->preVtx = -1;
	}

	return augFlow;
}

void iniResMtx()
{
	int i, j;
	for(i=VERTEXMAX-1; i>=0; i--){
		for(j=VERTEXMAX-1; j>=0; j--){
			resMtx[i][j] = 0;
		}
	}
}

void iniVtxVec()
{
	int i;
	vertex *p_vtx;

	vtxVec.clear();
	
	for(i=0; i<vertexNum; i++){
		p_vtx = new vertex(i);
		vtxVec.push_back(p_vtx);
	}
}
