
#include <stdio.h>
#include <string.h>
#define MAX_WORD 4000
#define MAX_LEN 20

char dict[MAX_WORD][MAX_LEN];
char grid[MAX_LEN][MAX_LEN];
int n_words; //���� ������ ����� �ܾ��� ��
int n;

void read_dictionary() {

	FILE *fp1 = fopen("dictionary.txt", "r");
	int i = 0;
	while (fscanf(fp1, "%s", dict[i]) != EOF)
		i++;
	fclose(fp1);
	n_words = i;
	return;
}
void read_grid() {
	FILE *fp2 = fopen("input.txt", "r");
	fscanf(fp2, "%d", &n);
	for (int i = 0; i < n; i++) {
		fscanf(fp2, "%s", grid[i]);
	}
	fclose(fp2);
	return;
}

char get_char(int i, int j, int dir, int dist) {

	int data[][8] = { { -1,0 },{ 1,0 },{ 0,-1 },{ 0,1 },{ 1,1 },{ 1,-1 },{ -1,1 },{ -1,-1 } };

	for (int k = 0; k < 8; k++) {
		if (dir == k) {
			for (int f = 0; f < dist; f++) {
				i += data[k][0];
				j += data[k][1];
			}
			break;
		}
	}

	return grid[i][j];

}

int search(int i) {

	for (int k = 0; k < n; k++) {
		for (int j = 0; j < n; j++) {
			int len = strlen(dict[i]);
			if (grid[k][j] != dict[i][0])
				continue;
			for (int dir = 0; dir < 8; dir++) {
				int dist = 1; //�ܾ� ����
				for (; dist < len; dist++) { //���� ���� ���캻��
					char ch = get_char(k, j, dir, dist); //i,j����dir�������� distĭ ������ �� �ܾ� �������ִ� �Լ�
					if (ch == '\0' || ch != dict[i][dist]) //�� �̰ų� �ܾ �ٸ��ٸ� �� �ʿ� X
						break;
				}
				if (dist >= len) { //���������� �ʰ� ������ ã�Ҵ� !!! �� �޼��� ��� 
					return 1;
				}
			}
		}
	}

	return 0;
}

int main() {

	read_dictionary();
	read_grid();

	for (int i = 0; i < n_words; i++) {
		int result = search(i);
		if (result == 1)
			printf("%s\n", dict[i]);
	}


}
