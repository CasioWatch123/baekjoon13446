#include<stdio.h>
#include<stdlib.h>

typedef struct {
	char* array;
	int length;
}CharArrayWrapper;

typedef struct {
	CharArrayWrapper target;
	int cursor;
	int length;
}Substring;

typedef struct {
	Substring strarr[3];
	int length;
}SubstringArrayWrapper;


int stringCompare(SubstringArrayWrapper* Strings1, SubstringArrayWrapper* Strings2) {
	char array1[48] = {};
	char array2[48] = {};
	Substring string1 = {{array1},0,0};
	Substring string2 = {{array2},0,0};
	
	int i,j,cursor;
	
	for(i=0;i<Strings1->length;i++) {
		Substring nowString = Strings1->strarr[i];
		for(j=0;j<Strings1->strarr[i].length;j++) {
			string1.target.array[string1.length++] = nowString.target.array[nowString.cursor+j];
		}
	}

	for(i=0;i<Strings1->length;i++) {
		Substring nowString = Strings2->strarr[i];
		for(j=0;j<Strings2->strarr[i].length;j++) {
			string2.target.array[string2.length++] = nowString.target.array[nowString.cursor+j];
		}
	}
	
	
	if (string1.length > string2.length) {
		return 1;
	}
	else if (string1.length == string2.length) {
		for(i=0;i<string1.length;i++) {
			if (string1.target.array[string1.cursor+i] < string2.target.array[string2.cursor+i]) {
				return 1;
			}
			else if (string1.target.array[string1.cursor+i] > string2.target.array[string2.cursor+i]) {
				return 0;
			}
		}
	}
	return 0;
}

Substring stringMerge(Substring* F, Substring* R) {
	Substring result;
	result.cursor = 0;
	result.length = F->length + R->length;
	
	result.target.length = result.length;
	result.target.array = (char*)malloc(sizeof(char) * (result.length + 1));
	
	int i;
	
	for(i=0;i<F->length;i++) {
		result.target.array[i] = F->target.array[F->cursor+i];
	}
	for(i=0;i<R->length;i++) {
		result.target.array[F->length+i] = R->target.array[R->cursor+i];
	}
	
	return result;
}


Substring searchMain(&CharArrayWrapper InputArray1, &CharArrayWrapper InputArray2) {
	int i,j;
	for(i=1;i<InputArray1->length;i++) {
		for(j=;j<InputArray2->length-1;j++) {
			if (InputArray1->array[i] == InputArray2->array[j] && 
			((i == 0 || j == 0) || InputArray1->array[i-1] == InputArray2->array[j-1])) {
				//alpha, beta 검색 로직 
			}
		}
	}
}
int main(void) {
	char a1[5];
	char a2[5];
	char a3[5];
	char a4[5];
	scanf("%s",a1);
	scanf("%s",a2);
	scanf("%s",a3);
	scanf("%s",a4);
	SubstringArrayWrapper wrapper1 = {{{{a1, 4}, 0, 4}, {{a2, 4}, 0, 4}}, 2};
	SubstringArrayWrapper wrapper2 = {{{{a3, 4}, 0, 4}, {{a4, 4}, 0, 4}}, 2};
	
	printf("%d", stringCompare(&wrapper1,&wrapper2));
	return 0;
}
