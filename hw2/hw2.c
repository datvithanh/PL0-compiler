#include <stdio.h>
#include <string.h>
#include <ctype.h>
int main(int argc, char* argv[]){
	char c;
	int i, L;
	// printf("Nhap xau.. :"); fflush(stdin); gets(str);
    char* str;
    str = argv[1];
	i= 0; L = strlen(str);
    char* str1;
    if(str[i] == '+' || str[i] == '-'){
        if(str[i] == '+')
            str1 = "duong";
        else str1 = "am";
        ++i;
    }
    else str1 = "";
    if(isdigit(str[i])){ 	//Nếu ký hiệu đọc được là một chữ số
        i=i+1;		//Đọc ký hiệu tiếp trên xâu vào
        while(isdigit(str[i])) i = i + 1; //Vẫn là chữ số, đọc tiếp
        if(i==L) printf("%s la so nguyen %s \n",str, str1); //Đọc hết xâu vào
        else if(str[i]!='.') printf("%s khong duoc doan nhan (%d)\n",str,i+1);
        else { 	//Đã đọc được dãy số và dấu ‘.’
            i = i + 1;
            while(isdigit(str[i])) i = i + 1;
                if(i==L) printf("%s la so thuc %s dau phay tinh \n",str, str1);
                else printf("%s khong duoc doan nhan(%d)\n",str,i+1);
        }
    }	 else printf("%s khong duoc doan nhan (%d)\n",str,i+1);
}//main
