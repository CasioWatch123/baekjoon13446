#include<stdio.h>
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

CharArrayWrapper InputArray1;
CharArrayWrapper InputArray2;

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





Substring beta_search(int Cursor1, int Cursor2, int CursorGap) {
	int maxLength;
	//
	for(maxLength=1; ;maxLength++) {
		if (!( Cursor1+CursorGap+maxLength < InputArray1.length ) || 
			!( Cursor2+CursorGap+maxLength < InputArray2.length ) ||
			 !( InputArray1.array[Cursor1+maxLength] == InputArray1.array[Cursor1+CursorGap+maxLength] ) ||
			 !( InputArray1.array[Cursor1+maxLength] == InputArray2.array[Cursor2+maxLength] ) ||
			 !( InputArray1.array[Cursor1+maxLength] == InputArray2.array[Cursor2+CursorGap+maxLength]) ) {
			break;
		}
		//debug
//		if (!(Cursor1+CursorGap+maxLength < InputArray1.length)) {
//			printf("case 1\n");
//			break;
//		}
//		if (!(Cursor2+CursorGap+maxLength < InputArray2.length)) {
//			printf("case 2\n");
//			break;
//		}
//		if (!(InputArray1.array[Cursor1+maxLength] == InputArray1.array[Cursor1+CursorGap+maxLength])) {
//			printf("case 3\n");
//			break;
//		}
//		if (!(InputArray1.array[Cursor1+maxLength] == InputArray2.array[Cursor2+maxLength])) {
//			printf("case 4\n");
//			break;
//		}
//		if (!(InputArray1.array[Cursor1+maxLength] == InputArray2.array[Cursor2+CursorGap+maxLength])) {
//			printf("case 5\n");
//			break;
//		}
	}
	
	
	//middle substring 비교 후 boolean return
	
	int i;
	Substring result = {&STANDARD_ARRAY, -1, -1};
	for(i=0;i<CursorGap-maxLength;i++) {
		if(InputArray1.array[Cursor1+i+1] != InputArray2.array[Cursor2+i+1]) {
			return result;
		}
	}
	
	result.cursor = Cursor1;
	result.length = CursorGap + maxLength;
	return result;
}

Substring beta_searchMain() {
	Substring top = {&STANDARD_ARRAY, 0,0};
	
	int cursor, j;
	
	for(cursor=0;cursor<InputArray1.length-1;cursor++) {
		for(j=cursor+1;j<InputArray1.length;j++) {
			if (InputArray1.array[cursor] == InputArray1.array[j] && 
			(cursor == 0 || InputArray1.array[cursor-1] != InputArray1.array[j-1])) {//cursor의 문자와 동일 문자 발견 
				int cursorGap = j-cursor;
				int i;
				for(i=0;i+cursorGap<InputArray2.length;i++) {
					if (InputArray2.array[i] == InputArray1.array[cursor] && 
						InputArray2.array[i+cursorGap] == InputArray1.array[cursor]) {
						//cursor 4개 획득. 탐색 함수 호출 
						Substring nowString = beta_search(cursor, i, j-cursor);
						printf("(alpha_searchMain) cursor: %d | methodnowString : %d:%c %d\n",cursor, nowString.cursor, nowString.target->array[nowString.cursor], nowString.length);
					}
				}
			}
		}
	}
	return top;
}
int main(void) {
	long start, finish;

	char array1[47] = {};
	char array2[47] = {};
	
	scanf("%47s %47s",array1,array2);
	
	InputArray1.array = array1;
	InputArray2.array = array2;

	int i;
	for(i=0;InputArray1.array[i];i++);
	InputArray1.length = i;
	
	for(i=0;InputArray2.array[i];i++);
	InputArray2.length = i;
	
	start = clock();
	//main method block
//	printf("result : %d %d\n",alpha_search(&inputArray1,&inputArray2,0,0,3));
//	printf("compare at 0|3 : %d\n", alpha_search(&inputArray1,&inputArray2,0,3).length);
	Substring a = beta_searchMain();
	
	printf("\n");
	
	finish = clock();
	
	printf("시작 시간: %6ld(ms)\n", start);
	printf("종료 시간: %6ld(ms)\n", finish);
	printf("소요 시간: %6ld(ms)\n", finish - start); 
	
//commit test;
}
