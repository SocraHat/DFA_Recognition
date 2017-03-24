#include "stdio.h"
#include "stdlib.h"
#define FILE_NAME "record.dfa"

int count = 0;
char ch[20]; // 字符集
int chs_num = 0;
int state[10]; // 状态集
int states_num = 0;
int start; // 开始状态
int accept[10]; // 接收状态
int accept_num = 0;
int move[10][10]; // 状态转移表
//char string[50];
int p = 0;

int check_dfa(int move[][10], char ch[], char input[], int ch_num, int states_num);
int check_start(int start,int state[],int states_num);
int check_accept(int states_num, int state[], int accept_num, int accept[]);
int check_move(int states_num, int chs_num, int move[][10]);
int judge_accept(int accept_num,int accept[],int end);
void print_string(int N,int temp,char string[],int last);

int main() {
	int num = 0;// 个数
	int i = 0, j = 0;
	char input[20];
	int N = 0;

	FILE *fp = fopen(FILE_NAME, "r+");//打开一个文件进行读写
	
	// 判断文件是否可以创建/打开
	if (fp == NULL) {
		fp = fopen(FILE_NAME,"w+");//创建一个文件进行读写
		printf("file not exist and create it succeed !\n");
	}
	fseek(fp, 0, SEEK_END);// 文件指针指向最后
	int flen = ftell(fp);
	fseek(fp,0,SEEK_SET);
	// ------------ write file ----------
	if (flen == 0) {// 文件中没有数据
		printf("please input number of character and chs:\n");
		scanf("%d\n", &chs_num); // 字符集 遇到空格就停止
		fprintf(fp, "%d\n", chs_num);
		// 使用%c写入时，回车空格会被误录入，使用%s
		gets(ch);
		fprintf(fp, "%s\n", ch);
	
		printf("please input number of state and state:\n");
		scanf("%d\n", &states_num);// 状态集
		fprintf(fp, "%d\n", states_num);
		for (i = 0; i < states_num -1; i++) {
			scanf("%d", &state[i]);
			fprintf(fp, "%d ", state[i]);
		}
		scanf("%d", &state[i]);
		fprintf(fp, "%d\n", state[i]);

		printf("please input start state: ");
		scanf_s("%d", &start);// 起始态
		fprintf(fp, "%d\n", start);

		printf("please input number of accept and state:\n");
		scanf_s("%d", &num);// 接收态
		fprintf(fp, "%d\n", num);
		for (i = 0; i < num-1; i++) {
			scanf("%d", &accept[i]);
			fprintf(fp, "%d ", accept[i]);
		}
		scanf("%d", &accept[i]);
		fprintf(fp, "%d\n", accept[i]);

		printf("please input move-state :\n");
		for (i = 0; i < states_num; i++) {// 状态转移额
			for (j = 0; j < chs_num - 1; j++) {
				scanf("%d", &move[i][j]);
				fprintf(fp, "%d ", move[i][j]);
			}
			scanf("%d", &move[i][j]);
			fprintf(fp, "%d\n", move[i][j]);
		}

		printf("succeed create! \n");
	}
	// 读取文件内容
	// C语言中读取文件时一定要与写入文件时一致，比如，写入时有空格，读取时就一定要有空格，回车符同理！！
	else {
		fscanf(fp, "%d\n", &chs_num);// 字符集
		for (i = 0; i < chs_num; i++) {
			fscanf(fp, "%c ", &ch[i]);
		}
		fscanf(fp, "%d\n", &states_num);// 状态集
		for (i = 0; i < states_num; i++) {
			fscanf(fp, "%d ", &state[i]);
		}
		fscanf(fp, "%d\n", &start);// 开始状态
		if (check_start(start, state, states_num) == -1) {
			exit(-1);
		}
		fscanf(fp, "%d\n", &accept_num);// 接收状态
		for (i = 0; i < accept_num; i++) {
			fscanf(fp, "%d ", &accept[i]);
		}
		if (check_accept(states_num, state, accept_num, accept) == -1) {
			exit(-1);
		}

		for (i = 0; i < states_num; i++) {
			for (j = 0; j < chs_num; j++) {
				fscanf(fp, "%d ", &move[i][j]);
			}
		}
		if (check_move(states_num, chs_num, move) == -1) {
			exit(-1);
		}
		// -------- choose ---------
		printf("please choose function : 0:check   1:print_string \n");
		scanf("%d",&N);
		if (N == 1) {
			printf("please input N to printf string \n");
			scanf("%d",&N);
			char string[50] = "";
			print_string(N,0,string,0);
		}
		else if(N==0){
			// -------- input ----------
			printf("please input a string to check ('#' means end): \n");//
			scanf("%s", input);
			int ch_num = 0;
			for (i = 0; i < sizeof(input) / sizeof(char); i++) {
				if (input[i] == '#')
					break;
				ch_num++;
			}
			// -------- check ----------
			int end = 0;
			end = check_dfa(move, ch, input, ch_num, states_num);
			printf("\nthe last state : %d \n", end);
			int result = judge_accept(accept_num,accept,end);
			if(result == 1)
				printf("succeed! \n");
			else
				printf("failed! \n");
		}
		else {
			printf("input illegal \n");//
			exit(0);
		}
	}
	// -------------------------
	fclose(fp);
	//system("pause"); // 防止程序运行完直接退出
	return 0;
}

int judge_accept(int accept_num, int accept[], int end) {
	for (int i = 0; i < accept_num; i++) {
		if (end == accept[i]) {
			return 1;
			break;
		}
	}
	return -1;
}

int check_dfa(int move[][10],char ch[],char input[],int ch_num,int states_num) {
	int i = 0,j = 0;
	int flag = 0;// 用来标记check字符串前进的个数
	int end = 0;
	while (flag<ch_num) {
		for (; j < states_num; j++) {
			if (ch[j] == input[flag]) {
				end = move[i][j];
				if (flag == 0)
					printf("%d --> %d ", i, move[i][j]);
				else
					printf(" --> %d ", move[i][j]);
				flag++;
				i = move[i][j]; // 因为后面i会
				j = 0;
				break;
			}
			if (j == states_num - 1) {// 出错的情况 
				printf("--> error\n");
				flag = ch_num;
				end = -1;
				return end;
			}
		}
	}
	return end;
}

int check_start(int start, int state[],int states_num) {
	int flag = 0;
	for (int i = 0; i < states_num; i++) {
		if (start == state[i]) {
			flag++;
		}
	}
	if (flag == 1)
		return flag;
	else {
		printf("start is illegal ! \n");
	}
	return -1;
}

int check_accept(int states_num, int state[], int accept_num, int accept[]) {
	int flag = 0;
	for (int i = 0; i < states_num; i++) {
		for (int j = 0; j < accept_num; j++) {
			if (accept[j] == state[i]) {
				flag++;
			}
		}
	}
	if (flag == accept_num) {
		return flag;
	}
	else {
		printf("accept is illegal ! \n");
	}
	return -1;
}

int check_move(int states_num,int chs_num, int move[][10]) {
	for (int i = 0; i < states_num; i++) {
		for (int j = 0; j < chs_num; j++) {
			if (move[i][j] < 0 || move[i][j]>100) {
				printf("dfa move is illegal! \ n");
				return -1;
			}
		}
	}
}

void print_string(int N,int temp,char string[],int last) {
	if (N<0 || temp < 0) {
		return;
	}
	N--;
	if (judge_accept(accept_num, accept, temp) == 1) {
		printf("%s\n", string);
		temp = last;
	}
	else if (judge_accept(accept_num, accept, temp) != 1 && N < 0) {
		return;
	}
	for (int i=0 ; i < strlen(ch); i++) {
		char copy[50] = "";
		strcpy(copy, string);
		//strncat(string,ch[i]);
		int len = strlen(string);
		string[len] = ch[i];
		string[len+1] = '\0';
		last = temp;// 备案
		temp = move[temp][i];
		print_string(N,temp,string,last);
		temp = last;
		strcpy(string,copy);
	}
}