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
	for(j=0;j<strlen(temp);j++){//harflerin hepsi küçültülür
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
	
		if(strcmp(hmap[adr].kelime,"Nan__Nan")==0){//adresteki kelime Nan__Nan ise kelime adrese konmamýþtýr
				return 0;
		}
		
		else if(strcmp(hmap[adr].kelime,aday.kelime)==0){
		//	printf("%s",aday.kelime);
			return 1;
		}
	
		else{//bölme boþ deðil ve kelimeyle ayný deðilse tekrar hashlenir
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

void put_on_hmap(struct dict *hmap,struct dict aday){//Errormap'e yeni eklemeler yapmak için kullanýlan fonksiyon
	int i=0;
	int key=horners_hash(aday);
	int hash2=hashfunc_2(key);
	int hash1=hashfunc_1(key);
	int adr=(hash1+i*hash2)%SIZE;
	while(i<SIZE){
		if(strcmp(hmap[adr].kelime,"Nan__Nan")==0){//adres eðer boþsa o kelime daha önce alýnmamýþtýr. Kelime hashmap'e eklenir
			hmap[adr]=aday;
			return;
		}
		else{//adres dolu ve kelimeyle ayný deðilse tekrar hashlenir
			i++;
			adr=(hash1+i*hash2)%SIZE;
		}
	}
}


	void read_from_Dictionary(struct dict *hmap){//SmallDictionary.txt den okuma yapýlýr
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

void print_hmap(struct dict *hmap){//Tüm hmap'in bastýrýlmasý için fonksiyon
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
int search_errormap(char *temp,struct dict *errormap){//Errormap'te kelimenin olup olmamasýna göre farklý deðerler döner
		int i=0;
		int j;
		for(j=0;j<strlen(temp);j++){//harflerin hepsi küçültülür
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
		
			if(strcmp(errormap[adr].kelime,"Nan__Nan")==0){//kelime yoksa -1 döner
				return -1;
			}
			else if(strcmp(errormap[adr].kelime,aday.kelime)==0){//kelime varsa errormapteki adresi döner
				return adr;
			}
			else{//bölme boþ deðil ve kelimeyle ayný deðilse tekrar hashlenir
				i++;
				adr=(hash1+i*hash2)%SIZE;
			}
			if(i==SIZE){//i=SIZE olduysa tüm map gezilmiþ fakat kelime bulunamamýþtýr(Eski ödevden kalan kýsým)
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
		for(j=0;j<strlen(girilen)+1;j++){//Derste gösterilen recurrence relation'a göre yaptým
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
	return 0;//kelime uzaklýðý num'a eþitse num deðeri, deðilse 0 döner 
}
	
void proceed_with_ED(struct dict *hmap,struct dict *girilen,int num){//Sözlükteki her kelimeyi girilen kelime ile karþýlaþtýrýr
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
		fflush(stdin);//komut satýrýnýn bufferýný temizlemek için
		if(choice==1){
		fflush(stdin);//komut satýrýnýn bufferýný temizlemek için
		gets(presentence);
		kelime=strtok(presentence," ");
		while(kelime!=NULL){
		
			if(search_hmap(kelime,hmap)==0){//Kelime ana sözlükte yoksa 0 döndürür
				
				if(search_errormap(kelime,errormap)==-1){//Kelime hatalý sözlükte yoksa -1 döner
					printf("\n%s sozlukte bulunamadi...\n",kelime);
					printf("\n------------------------------------\n");
					strcpy(girilen.kelime,kelime);
					proceed_with_ED(hmap,&girilen,1);
					
					
					if(girilen.count==0){//1 uzaklýkta kelime yoksa
						printf("\n1 uzaklikta kelime bulunamadi\n");
						printf("\n------------------------------------\n");
						proceed_with_ED(hmap,&girilen,2);
					
					
						if(girilen.count==0){//2 uzaklýkta bir kelime bulunamadýysa kelime yazýldýðý gibi basýlýr
							printf("\n2 uzaklikta kelime bulunamadi\n");
							printf("\n------------------------------------\n");
							strcpy(temp,kelime);}
							
						else{
							printf("\n------------------------------------\n");
							printf("(2 uzaklikta bulunanlar)");
							printf("\n Bunu mu demek istediniz:\n");
							for(i=0;i<girilen.count;i++){//2 uzaklýkta kelime bulunduysa
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
						
					else{//1 uzaklýkta kelime bulunduysa
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
				else{//Kelime hatalý sözlükte varsa önerilen deðerini döndürmeli
					strcpy(temp,errormap[search_errormap(kelime,errormap)].secilen);
					}
			}
			else{//Kelime sözlükte varsa sorun yok
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
	
