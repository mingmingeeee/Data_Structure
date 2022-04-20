#include "stack.h"
#include <string.h>

#define MAX 50

int main() {
	int num1, num2, num3;
	FILE *fp = fopen("Text.txt", "r");
	fscanf(fp, "%d", &num1);

	for (int i = 0; i < num1; i++) {
		Stack s = create();
		fscanf(fp, "%d", &num2);
		int sum = 0;
		int A[MAX] = { 0, };
		for (int j = 0; j < num2; j++) {
			int h = 0;
			fscanf(fp, "%d", &num3);

			if (j == 0) { push(s, num3); continue; }
			for (int k = s->top; k >= 0 && num3 >= s->contents[k]; k--) {
				h++;
				if (A[k] != 0)
					h += A[k];
				pop(s);
			}
			sum += h;
			push(s, num3);
			A[s->top] = h;
		}
		destroy(s);
		sum %= 1000000;
		printf("%d\n", sum);
	}
	fclose(fp);
}