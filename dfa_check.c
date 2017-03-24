#include "stdio.h"
#include "stdlib.h"
#define FILE_NAME "record.dfa"

int count = 0;
char ch[20]; // �ַ���
int chs_num = 0;
int state[10]; // ״̬��
int states_num = 0;
int start; // ��ʼ״̬
int accept[10]; // ����״̬
int accept_num = 0;
int move[10][10]; // ״̬ת�Ʊ�
//char string[50];
int p = 0;

int check_dfa(int move[][10], char ch[], char input[], int ch_num, int states_num);
int check_start(int start,int state[],int states_num);
int check_accept(int states_num, int state[], int accept_num, int accept[]);
int check_move(int states_num, int chs_num, int move[][10]);
int judge_accept(int accept_num,int accept[],int end);
void print_string(int N,int temp,char string[],int last);

int main() {
	int num = 0;// ����
	int i = 0, j = 0;
	char input[20];
	int N = 0;

	FILE *fp = fopen(FILE_NAME, "r+");//��һ���ļ����ж�д
	
	// �ж��ļ��Ƿ���Դ���/��
	if (fp == NULL) {
		fp = fopen(FILE_NAME,"w+");//����һ���ļ����ж�д
		printf("file not exist and create it succeed !\n");
	}
	fseek(fp, 0, SEEK_END);// �ļ�ָ��ָ�����
	int flen = ftell(fp);
	fseek(fp,0,SEEK_SET);
	// ------------ write file ----------
	if (flen == 0) {// �ļ���û������
		printf("please input number of character and chs:\n");
		scanf("%d\n", &chs_num); // �ַ��� �����ո��ֹͣ
		fprintf(fp, "%d\n", chs_num);
		// ʹ��%cд��ʱ���س��ո�ᱻ��¼�룬ʹ��%s
		gets(ch);
		fprintf(fp, "%s\n", ch);
	
		printf("please input number of state and state:\n");
		scanf("%d\n", &states_num);// ״̬��
		fprintf(fp, "%d\n", states_num);
		for (i = 0; i < states_num -1; i++) {
			scanf("%d", &state[i]);
			fprintf(fp, "%d ", state[i]);
		}
		scanf("%d", &state[i]);
		fprintf(fp, "%d\n", state[i]);

		printf("please input start state: ");
		scanf_s("%d", &start);// ��ʼ̬
		fprintf(fp, "%d\n", start);

		printf("please input number of accept and state:\n");
		scanf_s("%d", &num);// ����̬
		fprintf(fp, "%d\n", num);
		for (i = 0; i < num-1; i++) {
			scanf("%d", &accept[i]);
			fprintf(fp, "%d ", accept[i]);
		}
		scanf("%d", &accept[i]);
		fprintf(fp, "%d\n", accept[i]);

		printf("please input move-state :\n");
		for (i = 0; i < states_num; i++) {// ״̬ת�ƶ�
			for (j = 0; j < chs_num - 1; j++) {
				scanf("%d", &move[i][j]);
				fprintf(fp, "%d ", move[i][j]);
			}
			scanf("%d", &move[i][j]);
			fprintf(fp, "%d\n", move[i][j]);
		}

		printf("succeed create! \n");
	}
	// ��ȡ�ļ�����
	// C�����ж�ȡ�ļ�ʱһ��Ҫ��д���ļ�ʱһ�£����磬д��ʱ�пո񣬶�ȡʱ��һ��Ҫ�пո񣬻س���ͬ����
	else {
		fscanf(fp, "%d\n", &chs_num);// �ַ���
		for (i = 0; i < chs_num; i++) {
			fscanf(fp, "%c ", &ch[i]);
		}
		fscanf(fp, "%d\n", &states_num);// ״̬��
		for (i = 0; i < states_num; i++) {
			fscanf(fp, "%d ", &state[i]);
		}
		fscanf(fp, "%d\n", &start);// ��ʼ״̬
		if (check_start(start, state, states_num) == -1) {
			exit(-1);
		}
		fscanf(fp, "%d\n", &accept_num);// ����״̬
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
	//system("pause"); // ��ֹ����������ֱ���˳�
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
	int flag = 0;// �������check�ַ���ǰ���ĸ���
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
				i = move[i][j]; // ��Ϊ����i��
				j = 0;
				break;
			}
			if (j == states_num - 1) {// �������� 
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
		last = temp;// ����
		temp = move[temp][i];
		print_string(N,temp,string,last);
		temp = last;
		strcpy(string,copy);
	}
}