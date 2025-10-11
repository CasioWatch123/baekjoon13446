#include <stdio.h>
#include <stdlib.h>


typedef struct {
	char* array;
	int length;
}CharArrayWrapper;

typedef struct {
	char* array;
	int cursor;
	int length;
}Substring;

int singleStringCompare(Substring* New, Substring* Legacy) {
	if (New->length <= 0)
		return 0;
	if (New->length > Legacy->length)
		return 1;
	if (New->length < Legacy->length)
		return 0;
	
	int i;
	for(i=0;i<New->length;i++) {
		if (New->array[New->cursor+i] < Legacy->array[Legacy->cursor+i])
			return 1;
		if (New->array[New->cursor+i] > Legacy->array[Legacy->cursor+i])
			return 0;
	}
	
	return 0;
}

int multiStringCompare(Substring* NewAlpha, Substring* NewGammaBeta, Substring* LegacyAlpha, Substring* LegacyGammaBeta) {
	int NewLength = NewAlpha->length + NewGammaBeta->length;
	int LegacyLength = LegacyAlpha->length + LegacyGammaBeta->length;
	
	if (NewLength > LegacyLength)
		return 1;
	if (NewLength < LegacyLength)
		return 0;
	
	char array1[48] = {};
	char array2[48] = {};
	
	int i, count;
	
	count = 0;
	for(i=0;i<NewAlpha->length;i++)
		array1[count++] = NewAlpha->array[NewAlpha->cursor+i];
	for(i=0;i<NewGammaBeta->length;i++)
		array1[count++] = NewGammaBeta->array[NewGammaBeta->cursor+i];
	
	count = 0;
	for(i=0;i<LegacyAlpha->length;i++)
		array1[count++] = LegacyAlpha->array[LegacyAlpha->cursor+i];
	for(i=0;i<LegacyGammaBeta->length;i++)
		array1[count++] = LegacyGammaBeta->array[LegacyGammaBeta->cursor+i];
	
	
	for(i=0;i<NewLength;i++) {
		if (array1[i] < array2[i])
			return 1;
		if (array1[i] > array2[i])
			return 0;
	}
	
	return 0;
}

Substring stringMerge(Substring String1, Substring String2) {
	char* array = (char*)malloc(sizeof(char) * (String1.length+String2.length+1));
	Substring result = {array, 0, 0};
	
	if (String1.length <= 0 || String2.length <= 0) {
		return result;
	}
	
	int counter = 0;
	int i;
	for(i=0;i<String1.length;i++)
		array[counter++] = String1.array[String1.cursor+i];
	for(i=0;i<String2.length;i++)
		array[counter++] = String2.array[String2.cursor+i];
	
	result.length = String1.length + String2.length;
	
	return result;
}






Substring searchMain(CharArrayWrapper* InputArray1, CharArrayWrapper* InputArray2) {
	Substring topAlpha;
	Substring topGammaBeta;
	//search alpha
	int i,j,k;
	for(i=0;i<InputArray1->length-1;i++) {
		for(j=1;j<InputArray1->length;j++) {
			if (InputArray1->array[i] == InputArray1->array[j] && 
			(i == 0 || InputArray1->array[i-1] != InputArray1->array[j-1]) ) {
				for(k=1; ;k++) {
					if (
					 !(j+k < TargetArray->length) ||
					 !(i+k < j) ||
					 !(TargetArray->array[i+k] == TargetArray->array[j+k]) )
						break;
				}
				
				Substring nowAlpha = {InputArray1->array, k, i};
				
				//각각의 alpha에 따른 gamma, beta 탐색 진행 
				Substring nowGammaBeta = searchBetaGammaManager();//구현 필요 
				//최대 문자열 정보 갱신 
				if (multiStringCompare(&topAlpha, &topGammaBeta, &nowAlpha, &nowGammaBeta)) {
					topAlpha = nowAlpha;
					topGammaBeta = nowGammaBeta;
				}
			}
		}
	}
	
	return stringMerge(topAlpha, topGammaBeta);
}


int main(void) {
	//main method
}
