#include <string.h>
#include <stdio.h>

#define LIMIT 512

#define Version 0.1

#define Author danix@zonmail.pt

int main()
{
  char *str;  // Copy here all text from search engine
  char *p;
 /* EXAMPLE BING SEARCH USED const char str[] = "<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">\
 <html lang="en" xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" xmlns:Web="http://schemas.live.com/Web/"><head><meta content="text/html; charset=utf-8" http-equiv="\
 content-type" /><script type="text/javascript">//<![CDATA[ si_ST=new Date //]]></script><script type="text/javascript">//<![CDATA[ sb_gh=function(){return location.hash},\
 sb_sh=function(n){location.hash=n};function _ge(n){return _d.getElementById(n)}_w=window,_d=document,sb_de=_d.documentElement,sb_ie=!!_w.ActiveXObject,sb_i6=sb_ie&&!_\
 w.XMLHttpRequest,sb_st=_w.setTimeout,sb_ct=_w.clearTimeout,sb_gt=function(){return+new Date};sj_evt=new function(){function i(n){return t[n]||(t[n]=[])}var t={},\
 n=this;n.fire=function(n){for(var r=i(n),u=r.e=arguments,t=0;t<r.length;t++)r[t].d?sb_st(sj_wf(r[t],u),r[t].d):r[t](u)},n.bind=function(n,t,r,u){var f=i(n);t.d=u,\
 f.push(t),r&&f.e&&t(f.e)},n.unbind=function(n,i){for(var u=0,r=t[n];r&&u<r.length;u++)if(r[u]==i){r.splice(u,1);break}}};window.onerror||(window.onerror=function(n,t,i)\
 {var r="";r=typeof n=="object"&&n.srcElement&&n.srcElement.src?"\"ScriptSrc = '"+escape(n.srcElement.src.replace(/'/g,""))+"'\"":'"'+escape(n.replace(/"/g,""))+'","Meta":\
 "'+escape(t)+'","Line":'+i,(new Image).src=_G.lsUrl+'&Type=Event.ClientInst&DATA=[{"T":"CI.Error","FID":"CI","Name":"JSError","Text":'+r+"}]",typeof sj_evt!="undefined"\
 &&sj_evt.fire("ErrorInstrumentation",r)});_G={ST:(si_ST?si_ST:new Date),Mkt:"en-GB",RTL:false,Ver:"9_00_0_2547166",IG:"fae6e1a3485c420f9a4ebed0ded95899",EventID:"\
 774c29b70d3548a9916bb6664dfdfa73",V:"web",P:"SERP",DA:"LTSv2",CID:"17C648ABACCD6F2D3FDD4D48ADEC6F1F",SUIH:"dGVUq553m0vNI3iNJTcuvQ",PCId:"1",cUrl:"http:\/\/c.bing.com\
 \/c.gif?DI=15074",akamaiSyncUrl:"http:\/\/cms.abmr.net\/pix?cid=1237&amp;uid=",IP:"9517211",gpUrl:"\/fd\/ls\/GLinkPing.aspx?"};_G.lsUrl="/fd/ls/l?IG="+_G.IG+"&CID=\
 "+_G.CID+"&PM=Y";curUrl="http:\/\/www.bing.com\/search";function si_T(a){if(document.images){_G.GPImg=new Image;_G.GPImg.src=_G.gpUrl+'IG='+_G.IG+'&CID='+_\
 G.CID+'&PM=Y'+'&'+a;}return true;};function si_PP(n,t){var r,f,i,e,u;if(!_G.PPS){for(f=["FC","BC","H","BP",0];r=f.shift();)f.push('"'+r+'":'+(_G[r+"T"]?_G[r+"T"]-_G.ST:-1));\
 if(i=[],_w.performance&&performance.timing)for(i=["PL","loadEventEnd","requestStart","C","loadEventEnd","responseStart","U","unloadEventEnd","unloadEventStart","D",\
 "domainLookupEnd","domainLookupStart","TL","connectEnd","connectStart",0];r=i.shift();)e=performance.timing[i.shift()],u=performance.timing[i.shift()],\
 i.push('"'+r+'":'+"; */
  int x,i,start,starts,end,ends;
  int c;
  int jumper;
  int http[LIMIT];
  int e_http[LIMIT];
  int https[LIMIT];
  int e_https[LIMIT];
  start=0; starts=0;
 // char urls[10][LIMIT];
 // char urlss[10][LIMIT];
  //
  long lSize;
  char * buffer;
  size_t result;
  FILE *pFile;
  
  memset(str,0,0);
  
  pFile = fopen ("OUT.txt","r");  // OUT.txt file with bing search results raw text
  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

  
  // Get size of file and copy all text to buffer
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);
  buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
  result = fread (buffer,1,lSize,pFile);
  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
  fclose (pFile);
  // copy content of buffer to my internal buffer
  str=buffer;
  // release buffer
  free(buffer);

  //clean newlines to make buffer be readed as a whole string
   while((p=strpbrk(str,"\n"))!=NULL) *p=' ';  

  // create index of http and https starting urls
  for (i = 0 ; i < strlen(str); i++)
   {
     if ( str[i] == 'h' && str[i+1] == 't' && str[i+2] == 't' && str[i+3] == 'p' && str[i+4] == ':' )
	    {printf("start here %d \n",i); http[start++]=i;}
	 if ( str[i] == 'h' && str[i+1] == 't' && str[i+2] == 't' && str[i+3] == 'p' && str[i+4] == 's' && str[i+5] == ':')	
	    {printf("start here %d \n",i); https[starts++]=i;}
   }
   
  end=0;ends=0; 
  
  // create index of http ending urls
  for (x = 0 ; x < start ; x++)
   {
    printf("Start here HTTP URLS %d \n",http[x]);
	for(jumper=http[x] ; jumper < strlen(str) ; jumper++)
	 {
	  if (str[jumper] == '"' )
	   { printf("stop here %d \n",jumper); e_http[end++]=jumper;break;}
	  
	 }
   }
   // create index of https ending urls
   for (x = 0 ; x < starts ; x++)
   {
    printf("Start here HTTPS URLS %d \n",https[x]);
	for(jumper=https[x] ; jumper < strlen(str) ; jumper++)
	 {
	  if (str[jumper] == '"' )
	   { printf("stop here %d \n",jumper); e_https[ends++]=jumper;break;}
	  
	 }
   }
   
  
   // print http urls
	printf("HERE ARE your HTTP URLS : \n");
	for ( x = 0 ; x < start ; x++)
	   {
	    for (jumper = http[x] ; jumper < e_http[x] ; jumper++)
		{
		
		 putchar(str[jumper]);
		}
		printf("\n");
	   } 	
		
	 // print https urls	
	printf("HERE ARE your HTTPS URLS : \n");
	for ( x = 0 ; x < starts ; x++)
	   {
	    for (jumper = https[x] ; jumper < e_https[x] ; jumper++)
		{
		
		 putchar(str[jumper]);
		}
		printf("\n");
	   }
	   
   return(0);
}
