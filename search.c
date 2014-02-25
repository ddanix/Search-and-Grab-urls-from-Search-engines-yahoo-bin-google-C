    #include <stdio.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <stdlib.h>
    #include <netdb.h>
    #include <string.h>
	
    int create_tcp_socket();
    char *get_ip(char *host);
    char *build_get_query(char *host, char *page);
  	char *build_get_connect(char *host);
    void usage();
     
   
    #define PAGE "/"
    #define USERAGENT "Spyder 1.0"
   	#define NR_PAGES 10
	
	  char hosts[3][18]={"www.bing.com","search.yahoo.com","www.google.com"};
    char h_search[3][64]={"search?q=%s&go=&qs=ds&filt=all&first=%d&FORM=PORE","search?p=%s&ei=UTF-8&fr=yfp-t-901&xargs=0&b=%d","search?q=%s&start=%d"};
	
    int main(int argc, char **argv)
    {
      struct sockaddr_in *remote;
      int sock;
	  int i,p,q;
      int tmpres;
	  int port;
      char *ip;
      char *get;
      char buf[BUFSIZ+1];
	  char *buffed;
      char *host;
	  char *proxy;
      char *page;
	  char *query;
	  FILE *fout;
	  
     
      if(argc == 1 || argc < 4){
        usage();
        exit(2);
      }  
	  
      proxy = argv[1];
	  port = atoi(argv[2]);
	  query = argv[3];
     

	 
 
	  
	  for(i = 0 ; i <= 2 ; i++) // process all 3 search engines
	  {
	   for(p=1; p < NR_PAGES; p++) // do query for NR_pages on earch engine
	   { 
	  int sent = 0;
	  sock = create_tcp_socket();
      ip = get_ip(proxy);
      
	
      remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
      remote->sin_family = AF_INET;
      tmpres = inet_pton(AF_INET, ip, (void *)(&(remote->sin_addr.s_addr)));
	  fout=fopen("OUT.txt","a+");
	  
      if( tmpres < 0)  
      {
        perror("Can't set remote->sin_addr.s_addr");
        exit(1);
      }else if(tmpres == 0)
      {
        fprintf(stderr, "%s is not a valid IP address\n", ip);
        exit(1);
      }
      remote->sin_port = htons(port);
     
      if(connect(sock, (struct sockaddr *)remote, sizeof(struct sockaddr)) < 0){
        perror("Could not connect");
        exit(1);
      }
	  host=hosts[i];
	  printf("#%d##Searching on %s \n",i,host);
	 
	  get = build_get_connect(host);
	  printf("Query is:\n<<START>>\n%s<<END>>\n", get);
     
      //Send connect to proxy to the server
      
      send(sock, get+sent, strlen(get)-sent, 0);
      recv(sock, buf, BUFSIZ, 0);
      
	  printf("PROXy connected %s \n",buf);
	  
	  memset(get,0,sizeof(get));
	  memset(buf,0,sizeof(buf));
	  memset(page,0,sizeof(page));
	  
	  q=p*10; // * 10 to get 10 results on each page
	  
	  sprintf(page,h_search[i],query,q);
	  printf("query is %s \n",page);
      get = build_get_query(host, page);
      fprintf(stderr, "Query is:\n<<START>>\n%s<<END>>\n", get);
       
      //Send the query to the server
      tmpres=0;
      while(sent < strlen(get))
      {
        tmpres = send(sock, get+sent, strlen(get)-sent, 0);
        if(tmpres == -1){
          perror("Can't send query");
          exit(1);
        }
        sent += tmpres;
      }
      //now it is time to receive the page
      memset(buf, 0, sizeof(buf));
	  
      int htmlstart = 0;
      char * htmlcontent;
	 
	  while((tmpres = recv(sock, buf, BUFSIZ, 0)) > 0){
        if(htmlstart == 0)
        {
          
          htmlcontent = strstr(buf, "\r\n\r\n");
		 
          if(htmlcontent != NULL ){
            htmlstart = 1;
		
           htmlcontent += 4;
		   printf("Buff %s \n",buf);
          }
		  }else{
          htmlcontent = buf;
		  //printf("Buff %s \n",buf); break;
		
        }
	
        if(htmlstart){
		 fprintf(fout,"%s",htmlcontent);
        // fprintf(stdout, htmlcontent);
        }
     
        memset(buf, 0, tmpres);
        }
       if(tmpres < 0)
       {
        perror("Error receiving data");
       }
	   
	   free(get);
	   free(remote);
       free(ip);
	   close(sock);
	    } //for page
	  } //for hosts
	  
      
      
      
	  fclose(fout);
      return 0;
    }
     
    void usage()
    {
      fprintf(stderr, "USAGE: Spyder http_proxy proxy_port \"query_string\"\n");
    }
     
     
    int create_tcp_socket()
    {
      int sock;
      if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        perror("Can't create TCP socket");
        exit(1);
      }
      return sock;
    }
     
     
    char *get_ip(char *host)
    {
      struct hostent *hent;
      int iplen = 15; 
      char *ip = (char *)malloc(iplen+1);
      memset(ip, 0, iplen+1);
      if((hent = gethostbyname(host)) == NULL)
      {
        perror("Can't get IP");
        exit(1);
      }
      if(inet_ntop(AF_INET, (void *)hent->h_addr_list[0], ip, iplen) == NULL)
      {
        perror("Can't resolve host");
        exit(1);
      }
      return ip;
    }
     
    char *build_get_connect(char *host)
    {
      char *query;
      char *tpl = "CONNECT %s:80 HTTP/1.0\r\n\r\n\r"; 
      query = (char *)malloc(strlen(host));
      sprintf(query,tpl,host);
      return query;
    }
	
	
	char *build_get_query(char *host, char *page)
    {
      char *query;
      char *getpage = page;
      char *tpl = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
      if(getpage[0] == '/'){
        getpage = getpage + 1;
      }
      
      query = (char *)malloc(strlen(host)+strlen(getpage)+strlen(USERAGENT)+strlen(tpl)-5);
      sprintf(query, tpl, getpage, host, USERAGENT);
      return query;
    }
