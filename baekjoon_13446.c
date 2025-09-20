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




Substring alpha_count(CharArrayWrapper* InputArray, int Cursor1, int Cursor2) {
	int count;
	
	for(count=1;
	Cursor2+count < InputArray->length && 
	count < Cursor2-Cursor1 && 
	InputArray->array[Cursor1+count] == InputArray->array[Cursor2+count];
	count++);
//	printf("(alpha_count)method count : %d\n", count);//debug
	
	Substring result = {InputArray, Cursor1, count};
	
	return result;
}

Substring alpha_searchMain() {
	int cursor, j;
	
	
	//InputArray1
	Substring string1 = {NULL, 0, 0};
	
	for(cursor=0;cursor<InputArray1.length-1;cursor++) {
		for(j=cursor+1;j<InputArray1.length;j++) {
			if (
			InputArray1.array[cursor] == InputArray1.array[j] &&
			(cursor == 0 || InputArray1.array[cursor-1] != InputArray1.array[j-1])) {
				//커서 두 개 획득. 탐색 함수 호출 
//				printf("(alpha_searchMain)method call compareString 1\n"); //debug
				Substring nowString = alpha_count(&InputArray1, cursor, j);
				
				if(compareString(&nowString, &string1)) {
					string1 = nowString;
				}
			}
		}
	}
	
	//InputArray2
	Substring string2 = {NULL, 0, 0};
	
	for(cursor=0;cursor<InputArray2.length-1;cursor++) {
		for(j=cursor+1;j<InputArray2.length;j++) {
			if (
			InputArray2.array[cursor] == InputArray2.array[j] && 
			(cursor == 0 || InputArray2.array[cursor-1] != InputArray2.array[j-1])) {
				//커서 두 개 획득. 탐색 함수 호출
//				printf("(alpha_searchMain)method call compareString 2\n"); //debug
//				printf(" "); //debug
				Substring nowString = alpha_count(&InputArray2, cursor, j);
				
				if(compareString(&nowString, &string2)) {
					string2 = nowString;
				} 
			}
		}
	}
	
	// merge/return
	if (string1.target == NULL || string2.target == NULL) {
		Substring nullString = {NULL, 0, 0};
		return nullString;
	}
	
	//병합된 서브스트링 구성 
	int length = string1.length + string2.length;
	
	char* array = (char*)malloc(sizeof(char) * length);
	
	int i;
	for(i=0;i<string1.length;i++) {
		array[i] = string1.target->array[string1.cursor+i];
	}
	for(i=0;i<string2.length;i++) {
		array[string1.length+i] = string2.target->array[string2.cursor+i];
	}
	
	
	CharArrayWrapper* wrapper = (CharArrayWrapper*)malloc(sizeof(CharArrayWrapper));
	wrapper->array = array;
	wrapper->length = length;
	Substring result = {wrapper, 0, length};
	
	return result;
}



Substring beta_count(int Cursor1, int Cursor2, int CursorGap) {
	int maxLength;
	
	for(maxLength=1; ;maxLength++) {
		if (
		!( Cursor1+CursorGap+maxLength < InputArray1.length ) || 
		!( Cursor2+CursorGap+maxLength < InputArray2.length ) || 
		!( maxLength < CursorGap-1 ) ||
		 !( InputArray1.array[Cursor1+maxLength] == InputArray1.array[Cursor1+CursorGap+maxLength] ) ||
		 !( InputArray1.array[Cursor1+maxLength] == InputArray2.array[Cursor2+maxLength] ) ||
		 !( InputArray1.array[Cursor1+maxLength] == InputArray2.array[Cursor2+CursorGap+maxLength]) ) {
			break;
		}

//		debug
//		if (!(Cursor1+CursorGap+maxLength < InputArray1.length)) {
//			printf("case 1\n");
//			break;
//		}
//		if (!(Cursor2+CursorGap+maxLength < InputArray2.length)) {
//			printf("case 2\n");
//			break;
//		}
//		if (!( maxLength < CursorGap-1 )) {
//			printf("case2.5\n");
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
	Substring result = {NULL, 0, 0};
	for(i=0;i<CursorGap-maxLength;i++) {
		if(InputArray1.array[Cursor1+maxLength+i] != InputArray2.array[Cursor2+maxLength+i]) {
//			printf("return\n"); //debug
			return result;
		}
	}
//	printf("(beta_count)method %02d:%02d %02d:%02d | %d\n", Cursor1, Cursor1+CursorGap, Cursor2, Cursor2+CursorGap, maxLength); //debug
	
	result.target = &STANDARD_ARRAY;
	result.cursor = Cursor1;
	result.length = CursorGap + maxLength;
	return result;
}

Substring beta_searchMain() {
	Substring top = {&STANDARD_ARRAY, 0,0};
	
	int cursor, j;
	
	for(cursor=0;cursor<InputArray1.length-1;cursor++) {
		for(j=cursor+1;j<InputArray1.length;j++) {
			if (
			InputArray1.array[cursor] == InputArray1.array[j] && 
			(cursor == 0 || InputArray1.array[cursor-1] != InputArray1.array[j-1])) {//cursor의 문자와 동일 문자 발견 
//				printf("(beta_searchMain)method; found cursor %d:%d\n", cursor, j);//debug
				int cursorGap = j-cursor;
				int i;
				
				for(i=0;i+cursorGap<InputArray2.length;i++) {
					if (
					InputArray2.array[i] == InputArray1.array[cursor] && 
					InputArray2.array[i+cursorGap] == InputArray1.array[cursor]) {
						//cursor 4개 획득. 탐색 함수 호출 
//						printf("  array2 cursor %d:%d\n", i, i+cursorGap);//debug
						Substring nowString = beta_count(cursor, i, j-cursor);
						
						if (compareString(&nowString, &top)) {
							top = nowString;
						}
					}
				}
//				printf("\n");//debug
			}
		}
	}
	return top;
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
	
	Substring a = alpha_searchMain();
	Substring b = beta_searchMain();
	Substring resultString;
	
	if (a.target != NULL && b.target != NULL) {
		if (compareString(&a, &b)) {
			resultString = a;
//			printf("alpha\n"); //debug
		}
		else {
			resultString = b;
//			printf("beta\n"); //debug
		}
		
//		printf("top length : %d\n", resultString.length);//debug
		
		for(i=0;i<resultString.length;i++) {
			printf("%c", resultString.target->array[resultString.cursor+i]);
		}
	}
	else {
		printf("-1");
	}
	
	
	
	
	
//	printf("\n\n");
	
	
	if (a.target != NULL) {
		free(a.target->array);
		free(a.target);
	}
	finish = clock();
	//debug
//	printf("\n");
//	printf("시작 시간: %6ld(ms)\n", start);
//	printf("종료 시간: %6ld(ms)\n", finish);
//	printf("소요 시간: %6ld(ms)\n", finish - start); 
}
