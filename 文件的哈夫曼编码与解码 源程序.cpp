#include <fstream>
#include <stdio.h>
#include <iostream>
#include<string>

typedef struct char_count* count_node;
struct char_count {
	char ch;  // 字符
	long count;  // 出现的次数
	short bit;  // 分配的比特
};
count_node weight[126];  

typedef struct HNodeType* HNode;
struct HNodeType {
	count_node node; 
	HNode parent; 
	HNode lchild;  
	HNode rchild;  
};
HNode p;  

typedef struct charCode* code;
struct charCode {
	char ch; 
	short codeNum[1000];  
	int codeLen;  
};
code char_code[125];  

int n, num;  
int sumStrLen = 0;  
char str[100000];  
short codeStr[100000000];  

using namespace std;
string name;
void get_file() {

	cout << "请输入文件名称（可带路径）：";
	cin >> name;
	ifstream fin(name+".txt");
	fin >> str;
}


void tranCode(int len) {
	ofstream ofile(name+"_new.txt", std::ios::app);
	HNode q;
	q = p;
	ofile << "\n译码结果：" << endl;
	for (int i = 0; i < len; i++) {
		if (codeStr[i] == 0) {
			q = q->lchild;  // 进入左子树
			if (q->node->ch) {
				ofile << q->node->ch;
				q = p;
			}
		}
		if (codeStr[i] == 1) {
			q = q->rchild;  // 进入右子树
			if (q->node->ch) {
				ofile << q->node->ch;
				q = p;
			}
		}
	}
	ofile << endl;
}


void efficiency(int len) {
	
	ofstream ofile(name+".hfm", std::ios::app);
	int sumCodeLen = 0;
	for (int i = 0; i <= len; i++) {
		sumCodeLen += char_code[i]->codeLen;
	}
	ofile << "\n压缩效率：" << sumCodeLen << "/" << sumStrLen << "=" << sumCodeLen * 1.0 / sumStrLen << endl;
}

int finishCode() {
	ofstream ofile(name + ".ecd", std::ios::app);
	int m = 0;
	ofile << "\n编码结果：" << endl;
	for (int i = 0; i < sumStrLen; i++) {
		for (int j = 0; j < sumStrLen; j++) {
			if (char_code[j]->ch == str[i]) {
				for (int k = 0; k < char_code[j]->codeLen; k++) {
					codeStr[m] = char_code[j]->codeNum[k];
					ofile << codeStr[m];
					m++;
				}
				break;
			}
		}
	}
	ofile << endl;
	return m;
}


void getCode() {
	ofstream ofile(name+".hfm", std::ios::app);
	// 初始化
	for (int i = 0; i < 125; i++) {
		char_code[i] = new charCode();
		for (int j = 0; j < 1000; j++) {
			char_code[i]->codeNum[j] = -1;
		}
	}

	int cache[1000] = { 0 }, i = 0, j = 0;
	HNode q1, q2;
	q1 = p;
	// 所有左叶子
	while (q1->lchild) {
		char_code[i]->ch = q1->lchild->node->ch;
		j = 0;
		cache[j] = q1->lchild->node->bit;
		j++;
		q2 = q1;
		while (q2->node->bit) {
			cache[j] = q2->node->bit;
			j++;
			q2 = q2->parent;
		}
		int m = 0;
		for (int k = j - 1; k >= 0; k--) {
			char_code[i]->codeNum[m] = cache[k];
			m++;
		}
		char_code[i]->codeLen = m;
		i++;
		if (q1->rchild) {
			q1 = q1->rchild;
		}
		else {
			break;
		}
	}
	// 右叶子
	q1 = p;
	if (q1->rchild) {
		j = 0;
		int cache2[1000] = { 0 };
		while (q1->rchild) {
			cache[j] = q1->rchild->node->bit;
			j++;
			if (q1->rchild->node->ch) {
				char_code[i]->ch = q1->rchild->node->ch;
			}
			q1 = q1->rchild;
		}
		int m = 0;
		for (int k = j - 1; k >= 0; k--) {
			char_code[i]->codeNum[m] = cache[k];
			m++;
		}
		char_code[i]->codeLen = m;
	}

	// output
	ofile << "\n得出各字符的编码：" << endl;
	for (int k = 0; k <= i; k++) {
		if (char_code[k]->ch) {
			ofile << "char: " << char_code[k]->ch << " code: ";
			int k2 = 0;
			while (char_code[k]->codeNum[k2] != -1) {
				ofile << char_code[k]->codeNum[k2];
				k2++;
			}
		}
		ofile << endl;
	}

	// 把字符串进行编码
	int codeLen = finishCode();

	// 统计压缩效率
	efficiency(i);

	// 译码
	
}


void coder(HNode p) {
	if (p) {
		if (p->lchild) {
			p->lchild->node->bit = 0;
		}
		if (p->rchild) {
			p->rchild->node->bit = 1;
		}
		if (p->lchild) {
			coder(p->lchild);
		}
		if (p->rchild) {
			coder(p->rchild);
		}
	}
}


void outputHafftree(HNode p) {
	ofstream ofile(name + ".hfm", std::ios::app);
	ofile << "\n前序遍历哈夫曼树:" << endl;
	if (p) {
		// root
		if (p->node->ch != NULL) {
			ofile << "字符: " << p->node->ch << " 权重：" << p->node->count << endl;
		}
		else if (p->node->count) {
			ofile << "[新增节点] 权重：" << p->node->count << endl;
		}
		// left
		if (p->lchild) {
			outputHafftree(p->lchild);
		}
		// right
		if (p->rchild) {
			outputHafftree(p->rchild);
		}
	}
	ofile.close();
}


void HaffmanTree() {
	HNode hNl, hNr;
	hNl = new HNodeType();
	hNr = new HNodeType();
	hNl->lchild = hNl->rchild = NULL;  // 该节点（将成为左孩子）的左右子树置为NULL
	hNl->node = weight[1];  // 该节点（将成为左孩子）的核心节点置为weight中未使用的最小count的节点

	hNr->lchild = hNr->rchild = NULL;  // 该节点（将成为左孩子）的左右子树置为NULL

	if (n == 1) {  // 只有一个不同字符的情况
		hNr->node = NULL;  // 没有右孩子，右孩子置NULL

		p = hNl->parent = new HNodeType();  // 左孩子和右孩子的父亲置为一个新的节点，并使工作指针p指向它

		p->lchild = hNl;  // 新节点的左孩子置为旧的这个左孩子
		p->rchild = NULL;  // 新节点的右孩子置为NULL
		p->node = new char_count();  // 给新父亲分配空间
		p->node->ch = NULL;  // 新增的父亲的核心节点的ch置NULL
		p->node->count = p->lchild->node->count;  // 父亲权重为两个孩子权重的和
		p->parent = NULL;  // 置空哈夫曼树根节点中的父亲节点
	}
	else {
		hNr->node = weight[2];  // 该节点（将成为右孩子）的核心节点置为weight中未使用的最小count的节点

		for (int i = 3; i <= n; i++) {
			p = hNl->parent = hNr->parent = new HNodeType();  // 左孩子和右孩子的父亲置为一个新的节点，并使工作指针p指向它

			p->lchild = hNl;  // 新节点的左孩子置为旧的这个左孩子
			p->rchild = hNr;  // 新节点的右孩子置为旧的这个右孩子
			p->node = new char_count();  // 给新父亲分配空间
			p->node->ch = NULL;  // 新增的父亲的核心节点的ch置NULL
			p->node->count = p->lchild->node->count + p->rchild->node->count;  // 父亲权重为两个孩子权重的和

			hNl = new HNodeType();  // 给旧的左孩子（将再次成为一个左孩子）重新分配空间
			hNl->node = weight[i];  // 旧的左孩子（将再次成为一个左孩子）的核心节点置为weight中未使用的最小count的节点
			hNl->lchild = hNl->rchild = NULL;  // 旧的左孩子（将再次成为一个左孩子）的左右子树置为NULL

			hNr = p;  // 旧的右孩子成为父亲
		}
		// 生成哈夫曼树的根节点
		p = hNl->parent = hNr->parent = new HNodeType();  // 左孩子和右孩子的父亲置为一个新的节点，并使工作指针p指向它

		p->lchild = hNl;  // 新节点的左孩子置为旧的这个左孩子
		p->rchild = hNr;  // 新节点的右孩子置为旧的这个右孩子
		p->node = new char_count();  // 给新父亲分配空间
		p->node->ch = NULL;  // 新增的父亲的核心节点的ch置NULL
		p->node->count = p->lchild->node->count + p->rchild->node->count;  // 父亲权重为两个孩子权重的和
		p->parent = NULL;  // 置空哈夫曼树根节点中的父亲节点
	}
	// 打印

	outputHafftree(p);

	//分配比特
	coder(p);
}


void swap(int x, int y) {
	count_node t;
	t = weight[x];
	weight[x] = weight[y];
	weight[y] = t;
}

void siftdown(int i) {
	int t, flag = 0;
	while (i * 2 <= num && flag == 0) {
		if (weight[i]->count < weight[i * 2]->count) t = i * 2;
		else t = i;
		if (i * 2 + 1 <= num)
			if (weight[t]->count < weight[i * 2 + 1]->count) t = i * 2 + 1;
		if (t != i) {
			swap(t, i);
			i = t;
		}
		else flag = 1;
	}
}


void creat() {
	for (int i = num / 2; i >= 1; i--) siftdown(i);
}

void heapsort() {
	while (num > 1) {
		swap(1, num);
		num--;
		siftdown(1);
	}
}

void get_str_weight1() {
	ofstream ofile(name + ".hfm", std::ios::app);
	int i = 0;
	long count[126] = { 0 };
	while (str[i] != '\0') {
		count[str[i] - 32]++;
		sumStrLen++;
		i++;
	}
	sumStrLen *= 8;
	int j = 1;
	for (int i = 1; i < 126; i++) {
		if (count[i] > 0) {
			weight[j]->ch = i + 32;
			weight[j]->count = count[i];
			ofile << weight[j]->ch << " : 出现 " << weight[j]->count << " 次" << endl;
			j++;
			num++;
		}
	}
	n = num;
}




void task1() {
	get_file();
	get_str_weight1();
	creat();
	heapsort();
	ofstream ofile(name + ".hfm", std::ios::app);
	ofile << "\n建立最小堆（权重小的先出）：" << endl;
	for (int i = 1; i <= n; i++) {
		ofile << weight[i]->ch << " 权重：" << weight[i]->count << endl;
	}
	HaffmanTree();
	getCode();
	cout << "编码成功！" << endl;
}


int main() {
	for (int i = 0; i < 126; i++) {
		weight[i] = new char_count();
	}
	char choose;
	cout << "E----Encode a file\nD----Decode a file\nL----List Text file\nQ----Quit" << endl;
	while (cin >> choose) {
		if (choose == 'E') { task1(); }
		if (choose == 'D') { int codeLen = finishCode(); tranCode(codeLen); cout << "解码成功！" << endl; }
		if (choose == 'L') {
			char path1[1000];
			cout << "请输入文件路径：";
			cin >> path1;		
			ifstream fin(path1);
			string a;
			while (getline(fin, a)) { cout << a << endl; };
		}
		if (choose == 'Q') break ;
		cout << "E----Encode a file\nD----Decode a file\nL----List Text file\nQ----Quit" << endl;
	}
}