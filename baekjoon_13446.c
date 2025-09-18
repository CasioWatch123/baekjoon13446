#include<stdio.h>
#include<time.h>
//char InputArray[48] = {};
//int InputLength;
//int LengthCount = 0;
//int LengthTopCursor = -1;
int ForCount = 0;

typedef struct {
	int* array;
	int length;
}IntArrayWrapper;

typedef struct {
	char* array;
	int length;
}CharArrayWrapper;

typedef struct {
	int cursor;
	int length;
}Substring;



int stringCompare(CharArrayWrapper* InputArray, Substring* String, Substring* ProtoString) {
	if (String->length>ProtoString->length && String->length>0) {
		return 1;
	}
	else if(String->length==ProtoString->length && String->length>0) {
		int i;
		for(i=0;i<String->length;i++) {
			if(InputArray->array[String->cursor+i]<InputArray->array[ProtoString->cursor+i]) {
				return 1;
			}
		}
	}
	return 0;	
}





void alpha_search(CharArrayWrapper* InputArray1, CharArrayWrapper* InputArray2, int FrontCursor, int CursorGap) {
	
	Substring resultString = {-1,0};
	
	//InputArray1에서 부분문자열 길이 계산 
	int maxLength1;
	
	for(maxLength1=1;
		 FrontCursor+CursorGap+maxLength1<InputArray1->length &&
		 maxLength1<CursorGap && 
		 InputArray1->array[FrontCursor+maxLength1]==InputArray1->array[FrontCursor+CursorGap+maxLength1];
		maxLength1++);
//	maxLength++;
	
	
	//InputArray2에서 부분문자열(길이) 탐색
	int i;
	int maxLength2=0;
	int cursor=-1;
	
	for(i=0;i<InputArray2->length-CursorGap;i++) {
		if (InputArray2->array[i] == InputArray1->array[FrontCursor] && 
			InputArray2->array[i] == InputArray2->array[i+CursorGap]) {
			int j;
			for(j=1;
				 i+CursorGap+j<InputArray2->length && 
				 j<CursorGap && 
				 InputArray2->array[i+j] == InputArray2->array[i+CursorGap+j];
				j++);
			if (j>maxLength2) {
				maxLength2 = j;
				cursor = i;
			}
		}
	}
	
	int maxLength = maxLength1>maxLength2 ? maxLength1:maxLength2;
	//탐색 값 기반 메인 로직 
	
	printf("mL1 %d\n",maxLength1);
	printf("mL2 %d\n",maxLength2);
}

void alpha_searchMain(CharArrayWrapper* InputArray1, CharArrayWrapper* InputArray2, Substring* TopLengthSubstring) {
	int cursor, j;
	for(cursor=0;cursor<InputArray1->length-1;cursor++) {
		for(j=cursor+1;j<InputArray1->length;j++) {
			if (InputArray1->array[cursor] == InputArray1->array[j] && 
			(cursor == 0 || InputArray1->array[cursor-1] != InputArray1->array[j-1])) {//cursor의 문자와 동일 문자 발견 
				Substring nowSubstring = {-1, -1};
//				alpha_search();//메인 탐색 로직 
				
				if (stringCompare(InputArray1, &nowSubstring, TopLengthSubstring)) {
					TopLengthSubstring->cursor = nowSubstring.cursor;
					TopLengthSubstring->length = nowSubstring.length;
				}
			}
		}
	}
}
int main(void) {
	long start, finish;

	char array1[47] = {};
	char array2[47] = {};
	
	scanf("%47s %47s",array1,array2);
	
	CharArrayWrapper inputArray1 = {array1};
	CharArrayWrapper inputArray2 = {array2};

	int i;
	for(i=0;inputArray1.array[i];i++);
	inputArray1.length = i;
	
	for(i=0;inputArray2.array[i];i++);
	inputArray2.length = i;
	
	start = clock();
	//main method block
	alpha_search(&inputArray1,&inputArray2,0,3);
//	printf("compare at 0|3 : %d\n", alpha_search(&inputArray1,&inputArray2,0,3).length);
	
	
	printf("\n");
	
	finish = clock();
	
	printf("시작 시간: %6ld(ms)\n", start);
	printf("종료 시간: %6ld(ms)\n", finish);
	printf("소요 시간: %6ld(ms)\n", finish - start); 
	
//commit test;
}
