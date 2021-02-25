#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){  // argc = parametre sayısı argv = paramatre arrayı


    /*-----------------------------------------------------------------------------------------------------------------------------------

    // TEK BIR SYSCALL DEFINE VAR VE O DA 3 PARAMETRE ALIYOR!

    // PARAMETRELER ----> 1. PARAMETRE KULLANICININ KAÇ PARAMETRE GIRDIGINE BAKARAK YAPILAN SECIM HAKKINDA INT BIR DEGER GONDERIYOR
    //              ----> 2. PARAMETRE BIR CHAR ARRAYI. ICINDE /proc/<pid>/status TUTUYOR VE ONU GONDERIYOR
    //              ----> 3. PARAMATRE BIR CHAR ARRAYI. ICINDE /proc/<pid>/cmdline TUTUYOR VE ONU GONDERIYOR
    //              
    //              ----> NOT: PID ' LER JENERIKTIR STATIK DEGILDIR. KULLANICININ GIRDIGI 3. INPUT PID YE DONUSMEKTEDIR
    
    -------------------------------------------------------------------------------------------------------------------------------------*/
	long int amma;   

	if(argc == 1){          // parametre olarak ./progressinfo var --->   argc yani paramatre sayısı 1.
		printf("\n1. If you script -sudo dmesg- into terminal, you can see right usage rules. \n\n");
		amma =syscall(336,1,"0","0");       // burdaki 0 ların önemi yok onlar status ve cmdline gösterilecek olsa dosya yolu olarak girileckti.
	}

    else if(argc == 2 && argv[1][0]=='-' && argv[1][1]=='a' && argv[1][2]=='l' && argv[1][3]=='l' && strlen(argv[1])==4  ){     // parametre olarak ./progressinfo ve -all var --->   argc yani paramatre sayısı 2. iki parametreli olarak sadece all bilgisi isteniyor. 
        printf("\n2. If you script -sudo dmesg- into terminal, you can see all processes some information. \n\n");
        amma = syscall(336,2,"0","0");
    }

    else if(argc == 3 && argv[1][1]=='p' && argv[1][0]=='-' && strlen(argv[1])==2){     // parametre olarak ./progressinfo ve -p <pid> var --->   argc yani paramatre sayısı 3. uc parametreli olarak bir processin status ve cmdline isteniyor.
        printf("\n3. If you script -sudo dmesg- into terminal, you can see process and its cmdline\n\n");

        char *proc="/proc/";
	    char *status="/status";
        char *cmdline="/cmdline";

	    char doc_way_status[128];
        char doc_way_cmdline[128];
	    
        int index=0;    // index of array
        int len=0;      // length of char array

        for(int i=0; i<128; i++){

		    doc_way_status[i]=0;
		    doc_way_cmdline[i]=0;
        }

        len=strlen(proc);

        for(int i=0; i<len; i++){

            doc_way_status[index]=proc[i];
            doc_way_cmdline[index]=proc[i];
            index++;         
        }

        len=strlen(argv[2]); // bu input <pid>

        for(int i=0; i<len; i++){

            doc_way_status[index]=argv[2][i];
            doc_way_cmdline[index]=argv[2][i];
            index++;
        }

        int temp=index; // status kelimesi cmdline kelimesinden 1 harf kısa oldugu için index templeniyor sonra tekrar kaldığı yerden devam etmek için kullanılacak.

        len=strlen(status);
        for(int i=0; i<len; i++){

            doc_way_status[index]=status[i];
            doc_way_cmdline[index]=cmdline[i];
            index++;    
        }
    
        len=strlen(cmdline);
        index=temp; // temp kullanıldı

        for(int i=0; i<len; i++){

            doc_way_cmdline[index]=cmdline[i];
            index++;
        }


        printf("FILE PATH: %s\n",doc_way_status);
        printf("FILE PATH: %s\n",doc_way_cmdline);
        printf("\n");

        amma = syscall(336,3,doc_way_status,doc_way_cmdline);

    }
    else if(argc == 3 && argv[1][1]=='k' && argv[1][0]=='-' && strlen(argv[1])==2){
        printf("\n4. If you script -sudo dmesg- into terminal, you can see kill process\n\n");
        //int r = system("ps -aux");

        amma = syscall(336,4,"0","0");

    }
    
    return 0;
}