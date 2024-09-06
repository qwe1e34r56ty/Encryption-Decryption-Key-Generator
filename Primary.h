#ifndef _PRIMARY_H_
#define _PRIMARY_H_

class Primary{
private:
    // byte 동적 배열 쓰려고 해도 메모리가 부족해서 숫자당 소수 여부 1비트로 저장
    enum {
        BIT_PER_BYTE = 8
    };
    static char* isPrimary;
    // 생성 금지
    Primary(){};
    static void initPrimaryTable() {
        for (int i = 0; i < (int)pow(2, 7); i++) {
             for (int j = 0; j < BIT_PER_BYTE; j++) {
                 int checkNum = BIT_PER_BYTE * i + j;
                 isPrimary[i] |= (char)pow(2, j);
                 if (i == 0 && j < 2) {
                     isPrimary[i] &= (~(char)pow(2, j));
                 }
                 for (int k = 2; k < checkNum; k++) { 
                     if ((checkNum) % k == 0) {
                         isPrimary[i] &= (~(char)pow(2, j));
                         break;
                     }
                 }
             }
         }
    }
    static int pow(int a, int b) {
        int ret = 1;
        for (int i = 0; i < b; i++) {
            ret *= a;
        }
        return ret;
    }
public:
    static int upperPrimaryNumber(int s) {
	if(isPrimary[0] == 0){
		initPrimaryTable();
	}
        	for (int i = s; i < (int)pow(2, 10); i++) {
            	if (isPrimary[i / BIT_PER_BYTE] & (char)pow(2, i % BIT_PER_BYTE)) {
            	return i;
            }
        }
        return -1;
    }
};

#endif