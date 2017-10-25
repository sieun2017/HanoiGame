#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<Windows.h>
#include<conio.h>
#include<fstream>
#include<time.h>

using namespace std;

#define Width 100 // ����ũ��
#define Height 30 // ����ũ��

struct Hanoi{
	int stick; //�� ��° ���뿡 �ִ���
	int width; //����
	int floor; //�� ���� �ִ���
	int color;
	int up;

	void drawfloor();
};

struct Player{
	int rank;
	int score;
	char name[20];
};

Hanoi *hanoi=0;
Player player[5];
int floor_cnt = 0; // �ϳ��� Ÿ���� ��, ������ ����ϸ� �� ���� �ܰ踦 �غ� �� ����
int cursor; // ���� Ŀ�� ��ġ (���°)
int cnt; // �ϳ��� ����ü �迭�� ����
int move_cnt = 0; //������ �� (������ ���)
int n=-1; //���� ���õ� ������ �ε��� ( -1�� ��� ���õ� ���� ����)
int coin = 0;
bool e=true; // �������� �ƴ���
int select_floor=0; //���� �÷����ϴ� ��
char block[7][15] = { 0, };
int random = 0;//���� ����
ifstream designfile;
char filename[5][3][8] = { { "3_1.txt", "3_2.txt", "3_3.txt" }, { "4_1.txt", "4_2.txt", "4_3.txt" }, { "5_1.txt", "5_2.txt", "5_3.txt" }, { "6_1.txt", "6_2.txt", "6_3.txt" }, { "7_1.txt", "7_2.txt", "7_3.txt" } };

void gotoxy(int x, int y)// Ŀ���� ��ġ �̵��ϴ� �Լ�
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void setcolor(int color, int bgcolor)
{
	color &= 0xf;
	bgcolor &= 0xf;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4) | color);
}
void Cursor(int n)// Ŀ���� �Ⱥ��̰��ϴ� �Լ�
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = n;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
BOOL IsKeyDown(int Key)
{
	return ((GetAsyncKeyState(Key) & 0x8000) != 0);
}

void PrintScreen(); // ����ȭ��
void RankScreen(); // ��ũȭ��
void saveInfo(); //���Ͽ� ���� ����(����, ��)
void getReadyInfo(); // ���ΰ� ��
void HanoiDraw(); // �ϳ��� Ÿ���� �׸��� �Լ�
void Selectfloor(); // �� ����
void StartGame(); // �ϳ��� Ÿ�� ����
void Store(); // ����(�� ����)
void setStartHanoi(); // �Ӽ� ����
void Up(); //������ �ø��� �Լ�
void Down(); //������ ������ �Լ�
boolean end(); //���� ��������
void ExplainScreen(); // ���Ӽ��� ȭ��
void scoreCompare(); // ������
void getRank(); // ���Ͽ��� ��ũ ������ ��������
void saveRank(); //��ũ ���Ͽ� ����
void rankInput(int); //��ŷ���� ���� �Է�

int main(){
	while (e){
		system("cls");
		system("Color 6F");
		system("mode con:cols=100 lines=30"); // �ܼ� âũ��
		Cursor(0);
		PrintScreen();
	}

	delete[] hanoi;
	return 0;
}

void Hanoi::drawfloor(){
	if (designfile.is_open()){
		for (int i = 0; i < floor; i++){
			designfile >> block[i];
		}
	}
	for (int i = 0; i < 15; i++){
		if (block[floor-1][i] < 'A'){
			color = (int)(block[floor-1][i] - '0');
		}
		else{
			color = (int)(block[floor-1][i] - 'A') + 10;
		}
		setcolor(color, color);
		cout << "��";
	}
	designfile.close();
}

void PrintScreen(){ //ù ���� ȭ��
	system("cls");
	gotoxy(30, 7);
	cout << "����������������������������������������";
	gotoxy(30, 8);
	cout << "��";
	gotoxy(67, 8);
	cout << "��";
	gotoxy(30, 9);
	cout << "��             HANOI TOWER            ��";
	gotoxy(30, 10);
	cout << "��";
	gotoxy(67, 10);
	cout << "��";
	gotoxy(30, 11);
	cout << "����������������������������������������";
	gotoxy(43, 13);
	cout << "1. �� �� �� ��";
	gotoxy(43, 15);
	cout << "2. �� �� �� ��";
	gotoxy(43, 17);
	cout << "3. �� ŷ �� ��";
	gotoxy(43, 19);
	cout << "4. �� �� �� ��";
	gotoxy(40, 22);
	cout << "[ ���ڸ� �Է��ϼ��� ]";
	gotoxy(88, 29);
	cout << "2512 ������";
	gotoxy(35, 28);

	char choose = _getch();

	switch (choose){
	case '1': Selectfloor();
		break;
	case '2': ExplainScreen();
		break;
	case '3': RankScreen();
		break;
	case '4': 
		e = false;
		return;
		break;
	default: PrintScreen();
	}
}

void RankScreen(){
	getRank();
	system("cls");
	system("Color F6");
	gotoxy(31, 7);
	cout << "��������������������������������������";
	gotoxy(31, 8);
	cout << "��               RANK               ��";
	gotoxy(31, 9);
	cout << "��������������������������������������";
	for (int i = 0; i < 5; i++){
		gotoxy(38, 11+i*2);
		printf("%4d %10s %5d\n",player[i].rank, player[i].name, player[i].score);
	}
	gotoxy(88, 29);
	cout << "2512 ������";

	char choose = _getch();
}

void saveInfo(){
	ofstream ofile;

	ofile.open("readyForGame.txt");

	ofile << floor_cnt << endl << coin;

	ofile.close();
}

void getReadyInfo(){ // ��, ����
	ifstream ifile;

	ifile.open("readyForGame.txt");

	if (ifile.is_open()){
		ifile >> floor_cnt;
		ifile >> coin;
	}

	ifile.close();
}

void getRank(){
	ifstream ifile;

	ifile.open("score.txt");

	if (ifile.is_open()){
		for (int i = 0; i < 5; i++){
			ifile >> player[i].rank;
			ifile >> player[i].name;
			ifile >> player[i].score;
		}
	}
}

void saveRank(){
	ofstream ofile("score.txt");

	for (int i = 0; i < 5; i++){
		ofile << i + 1 << "\t" << player[i].name << "\t" << player[i].score << endl;
	}

	ofile.close();
}

void Store(){
	char yn;
	char sentence[50] = { 0 };

	system("cls");
	system("Color 8F");
	gotoxy(31, 7);
	cout << "��������������������������������������";
	gotoxy(31, 8);
	cout << "��               STORE              ��";
	gotoxy(31, 9);
	cout << "��������������������������������������";
	if (floor_cnt != 7){
		gotoxy(43, 13);
		cout << "���� ���� : "<<coin;
		gotoxy(40, 15);
		setcolor(13, 8);
		cout << floor_cnt + 1 << "�� : " << (floor_cnt + 1) * 50 << " coin �ʿ�";
		setcolor(15, 8);
		gotoxy(28, 17);
		cout << "�����Ͻðڽ��ϱ�? (Y/N) / �ڷΰ���� ESC >> ";
		gotoxy(88, 29);
		cout << "2512 ������";
		yn=_getche();
		switch (yn){
		case 'Y':
		case 'y':
			if (coin >= (floor_cnt + 1) * 50){
				coin -= (floor_cnt + 1) * 50;
				floor_cnt += 1;
				saveInfo();
				strcpy(sentence, "�� �� �� ��");
			}
			else{
				strcpy(sentence, "�� �� �� ��");
			}
			system("cls");
			gotoxy(40, 14);
			cout << sentence;
			gotoxy(88, 29);
			cout << "2512 ������";
			Sleep(1000);
			return;
			break;
		case 'N':
		case 'n':
			break;
		case 27: 
			return;
			break;
		default: Store();
		}
	}
	else{
		gotoxy(40, 12);
		cout << "7������ ��� �����ؼ�";
		gotoxy(37, 13);
		cout << "�� �̻� ������ �� �����ϴ�";
		Sleep(1000);
	}
	gotoxy(35, 28);
}

void HanoiDraw(){
	system("cls");
	int x = 3;
	int y1 = 20, y2 = 20, y3 = 20;
	gotoxy(2,1);
	cout << move_cnt << "��";
	if (floor_cnt == 7 && select_floor==7){
		gotoxy(83, 1);
		cout << "�ְ����� : " << player[0].score;
	}
	gotoxy(88, 29);
	cout << "2512 ������";

	switch (cursor){
	case 0:
		for (int i = 0; i < cnt; i++){
			if (hanoi[i].up == 1){
				gotoxy(3, 9);
				hanoi[i].drawfloor();
				break;
			}
			else{
				gotoxy(17, 9);
				cout << "��";
			}
		}
		break;
	case 1:
		for (int i = 0; i < cnt; i++){
			if (hanoi[i].up == 1){
				gotoxy(35, 9);
				hanoi[i].drawfloor();
				break;
			}
			else{
				gotoxy(49, 9);
				cout << "��";
			}
		}
		break;
	case 2:
		for (int i = 0; i < cnt; i++){
			if (hanoi[i].up == 1){
				gotoxy(67, 9);
				hanoi[i].drawfloor();
				break;
			}
			else{
				gotoxy(81, 9);
				cout << "��";
			}
		}
		break;
	}

	setcolor(15, 6);
	for (int i = 11; i < 21; i++){
		gotoxy(17, i);
		cout << "||";
		gotoxy(49, i);
		cout << "||";
		gotoxy(81, i);
		cout << "||";
	}

	for (int i = cnt-1; i >=0; i--){
		if (hanoi[i].stick == 1&&hanoi[i].up==0){
			gotoxy(x,y1--);
			hanoi[i].drawfloor();
		}
		else if (hanoi[i].stick == 2 && hanoi[i].up == 0){
			gotoxy(x+32, y2--);
			hanoi[i].drawfloor();
		}
		else if(hanoi[i].stick == 3 && hanoi[i].up == 0){
			gotoxy(x+64, y3--);
			hanoi[i].drawfloor();
		}
	}

	setcolor(15, 6);
	gotoxy(0, 21);
	cout << "����������������������������������������������������������������������������������������������������";
}

void ExplainScreen(){
	char b;
	system("cls");
	system("Color 0F");
	gotoxy(0, 1);
	cout << "����������������������������������������������������������������������������������������������������";

	gotoxy(31, 5);
	cout << "��������������������������������������";
	gotoxy(31, 6);
	cout << "��            �� �� �� ��           ��";
	gotoxy(31, 7);
	cout << "��������������������������������������";

	gotoxy(7, 9);
	cout << "�ϳ��� Ÿ���� ������ ����� �ƴ� �ٸ� ��տ� ��� ������ �ű�� �����ϴ� �����Դϴ�";
	gotoxy(7, 11);
	cout << "��, �ѹ��� �Ѱ��� ���Ǹ� �ű� �� �ְ�, �ݵ�� ū ���� ���� ���� ������ ������ �մϴ�";
	gotoxy(7, 13);
	cout << "����Ű�� �̿��� ������ �÷��� �� �� �ֽ��ϴ�. ó������ 3������ �÷��� �� �� ������ ";
	gotoxy(10, 15);
	cout << "������ ��� �� ���� ���� �ϳ��� Ÿ���� �������� ������ �̿��Ͻ� �� �ֽ��ϴ�.";
	gotoxy(13, 17);
	cout << "�����߿� ESC�� ������ ������ �� ������ ������� ������ �������ּ���";
	gotoxy(2, 20);
	cout << "��7�� �ϳ��̸� �÷����ؼ� ���� ������ ������ ��ŷ�� �̸��� �ø� �� �ֽ��ϴ�! 1� �����غ������";
	gotoxy(20, 22);
	cout << " �� �� : Ŀ�� �̵� , �� : ���� �ø��� , �� : ���� ������ ";
	gotoxy(31, 24);
	cout << "�ƹ�Ű�� ������ �������� �̵��մϴ�..";
	gotoxy(13, 25);
	gotoxy(0, 28);
	cout << "����������������������������������������������������������������������������������������������������";
	gotoxy(88, 29);
	cout << "2512 ������";
	b = _getch();
}

void Selectfloor(){
	char choose;
	getReadyInfo();
	system("cls");

	gotoxy(0, 1);
	cout << " �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ";

	gotoxy(30, 7);
	cout << "�÷����� �ϳ��� Ÿ���� ���� �������ּ���";
	switch (floor_cnt){
	case 7: gotoxy(44, 21);
		cout << "6. 7��";
	case 6: gotoxy(44, 19);
		cout << "5. 6��";
	case 5: gotoxy(44, 17);
		cout << "4. 5��";
	case 4: gotoxy(44, 15);
		cout << "3. 4��"; 
	case 3: gotoxy(44, 13);
		cout << "2. 3��";
		gotoxy(36, 11);
		cout << "1. ����(�� ���� �� ����)";
		break;
	}
	gotoxy(0, 28);
	cout << " �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ";
	gotoxy(88, 29);
	cout << "2512 ������";
	choose = _getch();
	if (choose == '1'){
		Store();
	}
	else if (choose > '1' && choose < (floor_cnt+'0')){
		cnt=(int)(choose-'0')+1;
		hanoi=new Hanoi[cnt];
		select_floor = cnt;
		setStartHanoi();
		srand((unsigned int)time(NULL));
		random = rand() % 3;
		designfile.open(filename[select_floor - 3][random]);
		HanoiDraw();
		StartGame();
		if (floor_cnt == 7 && select_floor==7){
			floor_cnt--;
		}
		saveInfo();
	}
	else{
		Selectfloor();
	}
}

void setStartHanoi(){
	getRank();
	for (int i = 0; i < cnt; i++){
		hanoi[i].stick = 1;
		hanoi[i].width=(i+1)*2+1;
		hanoi[i].floor = i + 1;
		hanoi[i].color = 9 + i;
		hanoi[i].up = 0;
	}
	cursor = 0;
	move_cnt = 0;
	n = -1;
}

void StartGame(){
	char key;
	while (end()){
		key = _getch();
		switch (key) {
		case 75:
			if (cursor > 0) cursor -= 1;
			break;
		case 77:
			if (cursor < 2) cursor += 1;
			break;
		case 72:
			Up();
			break;
		case 80:
			Down();
			break;
		case 27:
			setStartHanoi();
			return;
			break;
		}
		HanoiDraw();
	}
	int coin2 = select_floor*30;
	if (move_cnt <= pow(2,select_floor)*5/4){ //�̵����� �ּ� �̵����� 5/4 ���ϰ� �Ǹ� �߰� ����
		coin2 += (int)((pow(2, select_floor)*5/4 - move_cnt) * 10);
	}
	coin += coin2;
	Sleep(500);
	system("cls");
	gotoxy(38,14);
	cout <<"ȹ������ : "<<coin2<<" �����߽��ϴ�";
	Sleep(1000);
	if(select_floor==7)
		scoreCompare();

	setStartHanoi(); // ������ ������ �ٽ� �ʱ�ȭ
	return;
}

void Up(){
	for (int i = 0; i < cnt; i++){
		if (hanoi[i].stick == cursor + 1 && n == -1){
			hanoi[i].up = 1;
			n = i;
			return;
		}
	}
}

void Down(){
	int j=0;
	for (int i = 0; i < cnt; i++){
		if ((hanoi[i].stick == cursor + 1) && (hanoi[n].up == 1)){
			if (hanoi[n].width <= hanoi[i].width){
				hanoi[n].up = 0;
				move_cnt++;
				hanoi[n].stick = cursor + 1;
				n = -1;
				return;
			}
			else return;
		}
		if (hanoi[i].stick == cursor + 1 && hanoi[i].up == 0) j++;
	}

	if (j == 0){ // �� ��뿡 �ƹ� ������ ������
		hanoi[n].up = 0;
		move_cnt++;
		hanoi[n].stick = cursor + 1;
		n = -1;
		return;
	}
}

boolean end(){
	int s;
	if (hanoi[0].stick != 1) s = hanoi[0].stick;
	else return true;

	for (int i = 1; i < cnt; i++){
		if (hanoi[i].stick != s) return true;
	}

	return false;
}

void scoreCompare(){
	getRank();
	for (int i = 0; i < 5; i++){
		if (player[i].score>move_cnt){
			rankInput(i);
			saveRank();

			system("cls");
			gotoxy(40, 14);
			cout<<"�� ŷ �� �� �� ��";
			Sleep(1000);
			break;
		}
	}
}

void rankInput(int a){
	char n[20];

	system("cls");
	gotoxy(20, 14);
	cout << move_cnt<<"���� "<< a+1 <<"���Դϴ� �̸��� �Է����ּ��� >> ";
	gets_s(n);

	for (int i = 4; i >= a; i--){
		strcpy(player[i].name , player[i - 1].name);
		player[i].score = player[i - 1].score;
	}

	strcpy(player[a].name, n);
	player[a].score = move_cnt;
}