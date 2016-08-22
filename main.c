#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void traduci_pezzo(FILE* fp, FILE* nfp, char* buff);
void traduci_parola(char* string);
void insert_char (char* string,int pos,char c);
void resolving(char* string,int* pos,int* len);

int main(int argc, char** argv)
{
    FILE *fp;
    FILE *nfp;
    char buff[500];


    fp=fopen(argv[1],"r");
    nfp=fopen(argv[2],"w");

    printf("Traduzione del codice in corso...\n");

    fprintf(nfp,"{\n");


    traduci_pezzo(fp,nfp,buff);


    printf("Traduzione completata.\n\n");


    fclose(fp);
    fclose(nfp);
    return 0;
}

void traduci_pezzo(FILE* fp, FILE* nfp, char* buff)
{
char buff2[250];
char buff3[250];
    while (fscanf(fp,"%s",buff)==1 && strcmp(buff,"end") && strcmp(buff,"else") && strcmp(buff,"elseif"))
    {

        if (!strcmp(buff,"if"))
        {
            fprintf(nfp,"if (");
            fgets(buff,500,fp);
            traduci_parola(buff);
            buff[strlen(buff)-1]=')';
            fprintf(nfp,"%s {\n",buff);
            traduci_pezzo(fp,nfp,buff);
        }
        else if (!strcmp(buff,"while"))
        {
            fprintf(nfp,"while (");
            fgets(buff,500,fp);
            traduci_parola(buff);
            buff[strlen(buff)-1]=')';
            fprintf(nfp,"%s {\n",buff);
            traduci_pezzo(fp,nfp,buff);
        }
        else if (!strcmp(buff,"for"))
        {
            fprintf(nfp,"for (");
            fscanf(fp,"%[^=]s",buff);
            fgetc(fp);
            fscanf(fp,"%[^:]s",buff2);
            fgetc(fp);
            fscanf(fp,"%s",buff3);
            fprintf(nfp,"%s=%s; %s<=%s; %s++ ) {\n",buff,buff2,buff,buff3,buff);
            traduci_pezzo(fp,nfp,buff);
        }
        else
        {
            traduci_parola(buff);
            fprintf(nfp,"%s ",buff);
        }
    }

    if (!strcmp(buff,"end")) fprintf(nfp,"}\n");
    else if (!strcmp(buff,"else")) {
    fprintf(nfp,"}\nelse {\n");
    traduci_pezzo(fp,nfp,buff);
    }
    else if (!strcmp(buff,"elseif")) {
    fprintf(nfp,"}\nelse if (");
    fgets(buff,500,fp);
    traduci_parola(buff);
    buff[strlen(buff)-1]=')';
    fprintf(nfp,"%s {\n",buff);
    traduci_pezzo(fp,nfp,buff);
    }

}




void traduci_parola(char* string) {
    int i;
    int len=strlen(string);
    for (i=0; i<len; i++)
    {
        if (i>0 && string[i]=='(') resolving(string,&i,&len);
        else if (string[i]=='[') while (string[i]!=']') i++;
        else if (string[i]=='~') string[i]='!';
    }
}


void insert_char (char* string,int pos,char c) {
char buff[500];
int i;
for (i=0;i<pos;i++) buff[i]=string[i];
buff[pos]=c;
for (i=pos;i<strlen(string);i++) buff[i+1]=string[i];
buff[strlen(string)+1]='\0';
strcpy(string,buff);
}


void resolving(char* string,int* pos,int* len){
string[*pos]='[';
*pos=*pos+1;
while (string[*pos]!=')') {
if (string[*pos]=='(') resolving(string,pos,len);
if (string[*pos]==',') {
string[*pos]=']';
insert_char(string,*pos+1,'[');
*pos=*pos+1;
}
*pos=*pos+1;
}
string[*pos]=']';
}
