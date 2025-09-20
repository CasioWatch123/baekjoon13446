#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define STANDARD_ARRAY InputArray1


typedef struct {
	char* array;
	int length;
}CharArrayWrapper;

typedef struct {
	CharArrayWrapper* target;
	int cursor;
	int length;
}Substring;

typedef struct {
	CharArrayWrapper* target;
	int a;
	int b;
	int length;
	int gap;
}Cursors;



CharArrayWrapper InputArray1;
CharArrayWrapper InputArray2;

int compareString(Substring* String, Substring* ProtoString) {
	if (String->target != NULL && String->length > ProtoString->length) {
//		printf("(compareString)method, return type 1\n"); //debug
		return 1;
	}
	else if(String->target != NULL && String->length == ProtoString->length) {
		int i;
		for(i=1;i<String->length;i++) {
			if (String->target->array[String->cursor+i] < ProtoString->target->array[ProtoString->cursor+i]) {
//				printf("(compareString)method, return type 2\n"); //debug
				return 1;
			}
			if (String->target->array[String->cursor+i] > ProtoString->target->array[ProtoString->cursor+i]) {
//				printf("(compareString)method, return type 3\n"); //debug
				return 0;
			}
		}
	}
//	printf("(compareString)method, return type 4\n");
	return 0;
}


Substring searchBeta(); //구현 필요 


Cursors searchEqualSubstring(CharArrayWrapper* TargetArray, CharArrayWrapper* Array2) {
	int i,j,k;
	Substring topString = {TargetArray, -1, -1};
	Cursors topCursors = {NULL};
	for(i=0;i<TargetArray->length-1;i++) {
		for(j=i+1;j<TargetArray->length;j++) {
			if (
			TargetArray->array[i] == TargetArray->array[j] && 
			TargetArray->array[i] == TargetArray->array[j]) {
				//TargetArray 내 동일 문자 2개 발견
				int alphaLength;
				
				for(alphaLength=1; //동일 부분 길이 계산 
				 j+alphaLength < TargetArray->length && 
				 i+alphaLength < j && 
				 TargetArray->array[i+alphaLength] == TargetArray->array[j+alphaLength];alphaLength++);
				 //문자 index를 cursors 구조체로 만들어 관리 
				 Cursors cursors = {TargetArray, i, j, alphaLength, j-i};
				 
			//gamma 탐색 / 탐색된 gamma 기반 beta 탐색
				//~ alpha-gamma ~ alpha ~ case
				int gammaLength;
				for(gammaLength=1;i+alphaLength+gammaLength-1<j;gammaLength++) {
					searchBeta(TargetArray, Array2, ; // 구현 후 수정 필요 
					
				}
			}
		}
	}
	return topCursors;
}


int main(void) {
	long start, finish;

	char array1[47] = {};
	char array2[47] = {};
	
	scanf("%s %s",array1,array2);
	
	InputArray1.array = array1;
	InputArray2.array = array2;

	int i;
	for(i=0;InputArray1.array[i];i++);
	InputArray1.length = i;
	
	for(i=0;InputArray2.array[i];i++);
	InputArray2.length = i;
	start = clock();
	
	//main method block
	Cursors a = searchEqualSubstring(&InputArray1);
	if(a.target != NULL) {
		printf("cursor1 : %d, length : %d, gap : %d\n", a.a, a.length, a.gap);
		for(i=0;i<a.length;i++){
			printf("%c", a.target->array[a.a+i]);
		}
	}
	

	
	
//	printf("\n\n");
	
	//debug
	printf("\n");
	printf("시작 시간: %6ld(ms)\n", start);
	printf("종료 시간: %6ld(ms)\n", finish);
	printf("소요 시간: %6ld(ms)\n", finish - start); 
}
