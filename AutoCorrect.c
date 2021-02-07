#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SIZE 700

struct dict{
	char kelime[50];//kelime
	char secilen[50];
	char onerilenler[20][50];//Onerilen kelimeler bu arrayde tutulur
	int count;
};

int search_hmap(char *temp,struct dict *hmap){//hashmap'te arama fonksiyonu
	int i=0;
	int j;
	for(j=0;j<strlen(temp);j++){//harflerin hepsi k���lt�l�r
		if(temp[j]<'a')
			temp[j]=temp[j]+32;
	}
	struct dict bos_dizi[5];
	struct dict aday;
	strcpy(aday.kelime,temp);
	int key=horners_hash(aday);
	int hash2=hashfunc_2(key);
	int hash1=hashfunc_1(key);
	int adr=(hash1+i*hash2)%SIZE;
	while(i<SIZE){
	
		if(strcmp(hmap[adr].kelime,"Nan__Nan")==0){//adresteki kelime Nan__Nan ise kelime adrese konmam��t�r
				return 0;
		}
		
		else if(strcmp(hmap[adr].kelime,aday.kelime)==0){
		//	printf("%s",aday.kelime);
			return 1;
		}
	
		else{//b�lme bo� de�il ve kelimeyle ayn� de�ilse tekrar hashlenir
			i++;
			adr=(hash1+i*hash2)%SIZE;
		}
	}
	}
	
	
	int horners_hash(struct dict aday){//horner metodu
	int toplam=0;
	int i;

	for(i=0;i<strlen(aday.kelime);i++){
		if(aday.kelime[i]<'a')
			aday.kelime[i]=aday.kelime[i]+32;}

	for(i=0;i<strlen(aday.kelime);i++){
		toplam+=aday.kelime[i]*pow(3,i);
	}

	return (toplam);
}

int hashfunc_2(int key){
	return (1+(key%(SIZE-1)));
}

int hashfunc_1(int key){
	return (key%SIZE);
}

void put_on_hmap(struct dict *hmap,struct dict aday){//Errormap'e yeni eklemeler yapmak i�in kullan�lan fonksiyon
	int i=0;
	int key=horners_hash(aday);
	int hash2=hashfunc_2(key);
	int hash1=hashfunc_1(key);
	int adr=(hash1+i*hash2)%SIZE;
	while(i<SIZE){
		if(strcmp(hmap[adr].kelime,"Nan__Nan")==0){//adres e�er bo�sa o kelime daha �nce al�nmam��t�r. Kelime hashmap'e eklenir
			hmap[adr]=aday;
			return;
		}
		else{//adres dolu ve kelimeyle ayn� de�ilse tekrar hashlenir
			i++;
			adr=(hash1+i*hash2)%SIZE;
		}
	}
}


	void read_from_Dictionary(struct dict *hmap){//SmallDictionary.txt den okuma yap�l�r
	FILE *fp;
	char path[50];
	fp=fopen("smallDictionary.txt","r");
	struct dict aday;
	while(!feof(fp)){
		fscanf(fp,"%s",aday.kelime);
		put_on_hmap(hmap,aday);
	}
	fclose(fp);
}

void print_hmap(struct dict *hmap){//T�m hmap'in bast�r�lmas� i�in fonksiyon
	int i;
	int j=0;
	for(i=0;i<SIZE;i++){
		if(strcmp(hmap[i].kelime,"Nan__Nan")==0)
			printf("No data in segment(%d)",i);
		else{
			printf("%s",hmap[i].kelime);
		}
		printf("\n");
	}
}
int search_errormap(char *temp,struct dict *errormap){//Errormap'te kelimenin olup olmamas�na g�re farkl� de�erler d�ner
		int i=0;
		int j;
		for(j=0;j<strlen(temp);j++){//harflerin hepsi k���lt�l�r
			if(temp[j]<'a')
				temp[j]=temp[j]+32;
		}
		struct dict bos_dizi[5];
		struct dict aday;
		strcpy(aday.kelime,temp);
		int key=horners_hash(aday);
		int hash2=hashfunc_2(key);
		int hash1=hashfunc_1(key);
		int adr=(hash1+i*hash2)%SIZE;
		while(i<SIZE){
		
			if(strcmp(errormap[adr].kelime,"Nan__Nan")==0){//kelime yoksa -1 d�ner
				return -1;
			}
			else if(strcmp(errormap[adr].kelime,aday.kelime)==0){//kelime varsa errormapteki adresi d�ner
				return adr;
			}
			else{//b�lme bo� de�il ve kelimeyle ayn� de�ilse tekrar hashlenir
				i++;
				adr=(hash1+i*hash2)%SIZE;
			}
			if(i==SIZE){//i=SIZE olduysa t�m map gezilmi� fakat kelime bulunamam��t�r(Eski �devden kalan k�s�m)
				printf("------------------------------------------\n");
				printf("Kelime=(%s) tabloda bulunmuyor\n",aday.kelime);
			}
		}
	}

int find_min(int x,int y,int z){
	if(x<=z && x<=y)
		return x;
	if(y<=z && y<=x)
		return y;
	if(z<=x && z<=y)
		return z;	
}



int ED(char *girilen,char *kelime,int num){
	int matrix[strlen(kelime)+1][strlen(girilen)+1];
	int i,j;
	int flag;
	for(i=0;i<strlen(kelime)+1;i++){
		flag=1;
		for(j=0;j<strlen(girilen)+1;j++){//Derste g�sterilen recurrence relation'a g�re yapt�m
			if(j==0){
				matrix[i][j]=i;
			}
			else if(i==0){
				matrix[i][j]=j;
			}
			else if(kelime[i-1]==girilen[j-1]){
				matrix[i][j]=matrix[i-1][j-1];
			}
			else{
				matrix[i][j]=find_min(matrix[i-1][j-1],matrix[i-1][j],matrix[i][j-1])+1;
			}
		}
		
	}
	if(num==matrix[strlen(kelime)][strlen(girilen)])
	return num;
	return 0;//kelime uzakl��� num'a e�itse num de�eri, de�ilse 0 d�ner 
}
	
void proceed_with_ED(struct dict *hmap,struct dict *girilen,int num){//S�zl�kteki her kelimeyi girilen kelime ile kar��la�t�r�r
	int i;
	girilen->count=0;
	for(i=0;i<SIZE;i++){
		if(strcmp(hmap[i].kelime,"Nan__Nan")!=0){
			if(ED(girilen->kelime,hmap[i].kelime,num)==num){
				strcpy(girilen->onerilenler[girilen->count],hmap[i].kelime);
				girilen->count++;
			}
		}
		
	}
}


int main(){
	struct dict hmap[SIZE];
	struct dict errormap[SIZE];
	int i;

	for(i=0;i<SIZE;i++)
		strcpy(hmap[i].kelime,"Nan__Nan");

	for(i=0;i<SIZE;i++)
		strcpy(errormap[i].kelime,"Nan__Nan");
	
	read_from_Dictionary(hmap);
		
	char newsentence[200];
	char presentence[200];
	char oneri[5][50];
	char temp[50];
	strcpy(newsentence,"");
	
	int secim;
	int j;
	char *kelime;
	struct dict girilen;
	int while_flag=1;
	int choice;
	while(while_flag==1){
		newsentence[0]='\0';
		printf("\nCumle girmek icin 1'e\nCikmak icin 2'ye basiniz\n");
		scanf("%d",&choice);
		fflush(stdin);//komut sat�r�n�n buffer�n� temizlemek i�in
		if(choice==1){
		fflush(stdin);//komut sat�r�n�n buffer�n� temizlemek i�in
		gets(presentence);
		kelime=strtok(presentence," ");
		while(kelime!=NULL){
		
			if(search_hmap(kelime,hmap)==0){//Kelime ana s�zl�kte yoksa 0 d�nd�r�r
				
				if(search_errormap(kelime,errormap)==-1){//Kelime hatal� s�zl�kte yoksa -1 d�ner
					printf("\n%s sozlukte bulunamadi...\n",kelime);
					printf("\n------------------------------------\n");
					strcpy(girilen.kelime,kelime);
					proceed_with_ED(hmap,&girilen,1);
					
					
					if(girilen.count==0){//1 uzakl�kta kelime yoksa
						printf("\n1 uzaklikta kelime bulunamadi\n");
						printf("\n------------------------------------\n");
						proceed_with_ED(hmap,&girilen,2);
					
					
						if(girilen.count==0){//2 uzakl�kta bir kelime bulunamad�ysa kelime yaz�ld��� gibi bas�l�r
							printf("\n2 uzaklikta kelime bulunamadi\n");
							printf("\n------------------------------------\n");
							strcpy(temp,kelime);}
							
						else{
							printf("\n------------------------------------\n");
							printf("(2 uzaklikta bulunanlar)");
							printf("\n Bunu mu demek istediniz:\n");
							for(i=0;i<girilen.count;i++){//2 uzakl�kta kelime bulunduysa
								printf("%d. %s ",i,girilen.onerilenler[i]);
							}
							printf("\nLUTFEN SECMEK ISTEDIGINIZ ONERININ !INDEXINI! GIRINIZ(0'DAN BASLAYARAK)");
							scanf("%d",&secim);
							printf("\n------------------------------------\n");
							fflush(stdin);
							strcpy(girilen.secilen,girilen.onerilenler[secim]);
							put_on_hmap(errormap,girilen);
							strcpy(temp,girilen.secilen);}
						}
						
					else{//1 uzakl�kta kelime bulunduysa
						printf("\n------------------------------------\n");
						printf("(1 uzaklikta bulunanlar)");
						printf("\n Bunu mu demek istediniz:\n");
						for(i=0;i<girilen.count;i++){
								printf("%d. %s ",i,girilen.onerilenler[i]);
							}
							printf("\nLUTFEN SECMEK ISTEDIGINIZ ONERININ !INDEXINI! GIRINIZ(0'DAN BASLAYARAK)");
							scanf("%d",&secim);
							printf("\n------------------------------------\n");
							fflush(stdin);
							strcpy(girilen.secilen,girilen.onerilenler[secim]);
							put_on_hmap(errormap,girilen);
							strcpy(temp,girilen.secilen);
					}
				}
				else{//Kelime hatal� s�zl�kte varsa �nerilen de�erini d�nd�rmeli
					strcpy(temp,errormap[search_errormap(kelime,errormap)].secilen);
					}
			}
			else{//Kelime s�zl�kte varsa sorun yok
				strcpy(temp,kelime);	
			}
			strcat(newsentence,temp);
			strcat(newsentence," ");
			kelime=strtok(NULL," ");	
	}
	printf("\n%s",newsentence);	
	printf("\n------------------------------------------\n");
}

	else if(choice==2){
		while_flag=0;
		printf("\nProgram Kapaniyor\n");
	}
	
	else{
		printf("\n------------------------------------\n");
		printf("\nHatali tus girisi yaptiniz\n");
		printf("\n------------------------------------\n");
	}
	}
}
	
