#include <string.h>
#include <stdio.h>

#define LIMIT_F 8144  // max number of urls
#define LIMIT 2048  // number of index and temp urls
#define SIZEL 64   //url size
#define FLIMIT 50  //LIMIT divisons of file size

//log file with urls
FILE *furls;

char Word[LIMIT][SIZEL];
int StrCMP(WordA,WordB)
 char *WordA,*WordB;
 {  char LCWord[2][SIZEL],C;
    int UpperVsLowerDiff,I,J;
      strcpy(LCWord[0],WordA);
      strcpy(LCWord[1],WordB);
      UpperVsLowerDiff = 'a' - 'A';
     for (I = 0; I < 2; I++)
         for (J = 0; J < SIZEL; J++) {
           C = LCWord[I][J];
             if (C >= 'A' && C <= 'Z')
               LCWord[I][J] += UpperVsLowerDiff;
         }
       return strcmp(LCWord[0],LCWord[1]);
}


//final arrays to grab all urls
//
char _final_urls[LIMIT_F][SIZEL];
int total_urls=0;
char _final_urlss[LIMIT_F][SIZEL];
int total_urlss=0;

// filter each chunk of log from file and mark urls to filter to array
void filter(char *log){
  char *str;
  char *p;
  int x,i,start,starts,end,ends;
  int jumper;
  int http[LIMIT];
  int e_http[LIMIT];
  int https[LIMIT];
  int e_https[LIMIT];
  char urls[LIMIT][SIZEL];
  char urlss[LIMIT][SIZEL];






  char copy='X';
  int urlsi=0;
  int urlssi=0;
  //reset all arrays
  memset(&str,'\0', sizeof(str));
  memset(&urls[0], '\0', sizeof(urls));
  memset(&urlss[0], '\0', sizeof(urlss));
  memset(&http[0], '\0', sizeof(http));
  memset(&e_http[0], '\0', sizeof(e_http));
  memset(&https[0], '\0', sizeof(https));
  memset(&e_https[0], '\0', sizeof(e_https));

  printf("Log file imported , cleaning newlines and starting indexing of start and end of urls\n");
  str=log;

  while((p=strpbrk(str,"\n"))!=NULL) *p=' ';


  start=0; starts=0;

  for (i = 0 ; i < strlen(str); i++)
   {
     if ( str[i] == 'u' && str[i+1] == 'r' && str[i+2] == 'l' && str[i+3] == '?' && str[i+4] == 'q' && str[i+5] == '='  && str[i+6] == 'h' && str[i+7] == 't'
	     && str[i+8] == 't' && str[i+9] == 'p' && str[i+10] == ':' && str[i+11] == '/' && str[i+12] == '/')
	    {
		//<a href="/url?q=   Google Style
		if ( str[i-1] == '/' && str[i-2] == '"' && str[i-3] == '=' && str[i-4] == 'f' && str[i-5] == 'e' && str[i-6] == 'r' && str[i-7] == 'h') http[start++]=i+6;
		}
		// Bing or Yahoo Style
	 if ( str[i] == 'h' && str[i+1] == 't' && str[i+2] == 't' && str[i+3] == 'p' && str[i+4] == ':' && str[i+5] == '/'  && str[i+6] == '/')
	    {
		// href="http  -1=" -2== -3=f -4=e -5=r -6=h
		if ( str[i-1] == '"' && str[i-2] == '=' && str[i-3] == 'f' && str[i-4] == 'e' && str[i-5] == 'r' && str[i-6] == 'h') http[start++]=i;
		}
		//url?q=https://   href="/url?q=https:// Google Style
	 if ( str[i] == 'u' && str[i+1] == 'r' && str[i+2] == 'l' && str[i+3] == '?' && str[i+4] == 'q' && str[i+5] == '='  && str[i+6] == 'h' && str[i+7] == 't'
	     && str[i+8] == 't' && str[i+9] == 'p' && str[i+10] == 's' && str[i+11] == ':' && str[i+12] == '/' && str[i+13] == '/')
	    {
		if ( str[i-1] == '/' && str[i-2] == '"' && str[i-3] == '=' && str[i-4] == 'f' && str[i-5] == 'e' && str[i-6] == 'r' && str[i-7] == 'h')https[starts++]=i+6;
		}
		// Bing or Yahoo Style
	 if ( str[i] == 'h' && str[i+1] == 't' && str[i+2] == 't' && str[i+3] == 'p' && str[i+4] == 's' && str[i+5] == ':' && str[i+6] == '/' && str[i+7] == '/')
	    {
		if ( str[i-1] == '"' && str[i-2] == '=' && str[i-3] == 'f' && str[i-4] == 'e' && str[i-5] == 'r' && str[i-6] == 'h')https[starts++]=i;
		}
   }




  end=0;ends=0;



  for (x = 0 ; x < start ; x++)
   {

	for(jumper=http[x]+7 ; jumper < strlen(str) ; jumper++)
	 {
	 if (str[jumper] == '/' )
	   {
	   e_http[end++]=jumper;break;
	   }

	 }
   }


   for (x = 0 ; x < starts ; x++)
   {

	for(jumper=https[x]+8 ; jumper < strlen(str) ; jumper++)
	 {
	  if (str[jumper] == '/' )
	   {
	    e_https[ends++]=jumper;break;}
       }
   }





	urlsi=0;copy='\0';

	for ( x = 0 ; x < start ; x++)
	   {
	    for (jumper = http[x] ; jumper < e_http[x] ; jumper++)
		{
		urls[urlsi][copy++]=str[jumper];
		//printf("%c",str[jumper]); // DEBUG
	    }
	   //printf("\n"); //DEBUG
       urls[urlsi][copy]='\0';

		copy='\0';
		urlsi++;

	   }


	for (i = 0 ; i< urlsi ; i++)
	{
	  //if(urls[i][0] != NULL)	{
      if(urls[i][0] != ' ')	{
	  //dont print urls of search engines
	  if( strstr(urls[i],"yahoo") ||  strstr(urls[i],"bing") ||  strstr(urls[i],"microsoft") ||  strstr(urls[i],"google") ||  strstr(urls[i],"youtube") )
	  {}
	  else sprintf(_final_urls[total_urls++],"%s\n",urls[i]);
	  }

    }

	urlssi=0;copy='\0';

	for ( x = 0 ; x < starts ; x++)
	   {
	    for (jumper = https[x] ; jumper < e_https[x] ; jumper++)
		{
        urlss[urlssi][copy++]=str[jumper];
		//printf("%c",str[jumper]); //DEBUG
		}
        //printf("\n"); //DEBUG
		urlss[urlssi][copy]='\0';


		copy='\0';
		urlssi++;

	   }

      for (i = 0 ; i < urlssi ; i++)
	  {
	  //printf("%d %s \n",i,urlss[i]);
	  if(urlss[i][0] != ' ')	{
    //  if(urlss[i][0] != NULL)	{
	  //dont print urls of search engines
	  if( strstr(urls[i],"yahoo") ||  strstr(urls[i],"bing") ||  strstr(urls[i],"microsoft") ||  strstr(urls[i],"google") ||  strstr(urls[i],"youtube") )
	  {}
	  else sprintf(_final_urlss[total_urlss++],"%s\n",urlss[i]);
	   }
	  }
	  printf("Total http urls so far = %d \n",total_urls);
	  printf("Total https urls so far = %d \n",total_urlss);

}


// clean repeated urls and sort all and dump to file

void fwrite_urls(){
  int i,j;
  for (i = total_urls; i > 0; i--)
     {
        if (_final_urls[i - 1] == 'X')
       {
        continue;
       }

      for (j = i - 1; j > 0; j--)
      {
        if (strcmp(_final_urls[i - 1], _final_urls[j - 1]) == 0)
        {
            _final_urls[j - 1][0] = 'X';
        }
      }
    }

	qsort(_final_urls,total_urls,SIZEL,StrCMP);

	for (i = 0 ; i < total_urls ; i++)
	  {
       if(_final_urls[i][0] != 'X')	{fprintf(furls,"%s",_final_urls[i]);}
	  }
	  printf("Added %d http urls\n",total_urls);

    for (i = total_urlss; i > 0; i--)
     {
        if (_final_urlss[i - 1] == 'X')
       {
        continue;
       }

      for (j = i - 1; j > 0; j--)
      {
        if (strcmp(_final_urlss[i - 1], _final_urlss[j - 1]) == 0)
        {
            _final_urlss[j - 1][0] = 'X';
        }
      }
    }

	qsort(_final_urlss,total_urlss,SIZEL,StrCMP);

	for (i = 0 ; i < total_urlss ; i++)
	  {
       if(_final_urlss[i][0] != 'X')	{fprintf(furls,"%s",_final_urlss[i]);}
	  }
	  printf("Added %d https urls\n",total_urlss);

}









int main()
{
  char *str;
  long lSize;
  char * buffer;
  size_t result;
  FILE *pFile;
  int spliter;
  long sval;
  int i,x,j,k;
  int findex;
  int ffindex[FLIMIT];


  pFile = fopen ("OUT.txt","r");
  furls = fopen ("urls.txt","a+");
  if (pFile==NULL) {fputs ("File error",stderr); exit(1);}
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);
  fflush(pFile);

    if( lSize > 20000000)
	{
	printf("File cant be larger than 20 MegaBytes\n");
	exit(1);
	}

   if ( lSize < 200000)
   {
   spliter=0;
   printf("Less than 200k\n");
   }
   spliter=lSize/200000;
   printf("Spliter is %d ",spliter);

	if (spliter > 0 )
	{
	   findex=0;
	   sval=lSize/spliter;
	   printf("\n %d %d \n",lSize,sval);
	   for(i = 0 ; i < spliter ; i++)
	   {
	   findex=findex+sval;
	   ffindex[i]=findex;
	   printf("%d %d \n",i,ffindex[i]);
       }
       for(x=0; x < spliter ; x++)
       {

        if(x == 0)
         {
         printf("\nx is %d Will copy fom 0 to %d \n",x,sval);
	     fseek (pFile , 0 , SEEK_SET);
         buffer = (char*)malloc(sizeof(char)*sval);
         result = fread (buffer,1,sval,pFile);
		 str=buffer;
         filter(str);

         }
         else
         {
         printf("\nx is %d Will copy from %d to  %d \n",x,ffindex[x-1],ffindex[x]);
	     fseek (pFile , ffindex[x-1] , SEEK_SET);
         buffer = (char*)malloc(sizeof(char)*sval);
         result = fread (buffer,1,sval,pFile);
		 str=buffer;
		 filter(str);

          }

	 }
	}
	else
	if(spliter == 0)
	{
	fseek (pFile , 0 , SEEK_SET);
	buffer = (char*)malloc(sizeof(char)*lSize);
	result = fread (buffer,1,lSize,pFile);
	str=buffer;
	filter(str);
	}

    fwrite_urls();
    fclose(pFile);
    fclose(furls);

  return 0;
}

