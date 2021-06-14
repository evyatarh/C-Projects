#include "p2p.h"

/****global variables****/
bool isShutDown;
int numOfsharedFiles;
int clientPort;
file_ent_t sharedFiles[P_BUFF_SIZE];
//pthread_mutex_t mutex;

/*****functions declerations*****/
void ackToTheNotify(int fdSocket);
void dirEnt(int fdSocket);
void ServerShutDown(int fdConnected);
void handleRequestS(int fdConnected);
void run();

/****************main function*****************/
int main(int argc, char* argv[]) {
  run();
  return EXIT_SUCCESS;
}

/********************run function******************/
void run(){
  int fdSocket = 0;
  int fdConnected = 0;
  struct sockaddr_in myaddr;
  //pthread_t pthread;
  //pthread_mutex_init( &mutex, NULL );

  isShutDown = false;
  numOfsharedFiles = 0;

  myaddr.sin_family = AF_INET;
  myaddr.sin_port = htons(P_SRV_PORT);
  myaddr.sin_addr.s_addr = INADDR_ANY;

  //open a socket with tcp connection (SOCK_STREAM)
  fdSocket = socket(AF_INET,SOCK_STREAM,0);

  if(fdSocket == -1) {
    perror("Server socket() Failed!");
    exit(EXIT_FAILURE);
  }

  if(inet_aton("127.0.0.1", &myaddr.sin_addr) != 1 ) {
    close(fdSocket);
    perror("Server inet_aton() Failed!");
    exit(EXIT_FAILURE);
  }

  printf("\nServer - server: opening socket on 127.0.0.1:%d\n", P_SRV_PORT);
  //publish server address
  if((bind(fdSocket,(struct sockaddr*)&myaddr,sizeof(myaddr))) == -1) {
    close(fdSocket);
    perror("Server  bind() Failed!");
    exit(EXIT_FAILURE);
  }
  //approve receiving requests; maximum queue size is 5
  if((listen(fdSocket, MAX_QUEUE_SIZE)) != 0) {
    close(fdSocket);
    perror("Server listen() Failed!");
    exit(EXIT_FAILURE);
  }

  //mutex
//pthread_mutex_lock( &mutex );
  clientPort = P_SRV_PORT + 1;
//pthread_mutex_unlock( &mutex );

  while(!isShutDown) {
    //connect to the request
    fdConnected = accept(fdSocket,NULL,NULL);

    if (fdConnected < 0){
      close(fdSocket);
      perror("Server accept() Failed!");
      exit(EXIT_FAILURE);
    }/*
    //the chiled starting conversation with the client
    if(pthread_create(&pthread,NULL,handleRequestS,(void*)&fdConnected)!=0){
      close(fdSocket);
      perror("Server pthread_create() Failed!");
      exit(EXIT_FAILURE);

   }
*/
    handleRequestS(fdConnected);
    close(fdConnected);
    //else, the father continue handle the other requests in the queue
  }
}


/********************handleRequestS function******************/
void handleRequestS(int fdConnected){
  msg_type_t msg_type;
  msg_finish_t msg_finish;
  msg_type.m_type = MSG_DUMP;

  while(1) {
    if(recv(fdConnected, &msg_type, sizeof(msg_type), MSG_PEEK) < 0)continue;

      switch (msg_type.m_type) {
        case MSG_NOTIFY:
          ackToTheNotify(fdConnected);
          break;
        case MSG_DIRREQ:
          dirEnt(fdConnected);
          break;
        case MSG_SHUTDOWN:
          ServerShutDown(fdConnected);
          break;
        case MSG_FINISH:
          if(recv(fdConnected,&msg_finish,sizeof(msg_finish),0) < 0){
            close(fdConnected);
            perror("Server recv() Failed!");
            exit(EXIT_FAILURE);
          }
          return;
        default:
          break;
      }
  }
}

/********************dirEnt function******************/
void dirEnt(int fdSocket) {
  //pthread_mutex_lock( &mutex );
  msg_dirreq_t msg_dirreq;
  msg_dirhdr_t msg_dirhdr;
  msg_dirent_t msg_dirent;


  //receiving the request of a shared list files from the client
  if(recv(fdSocket,&msg_dirreq,sizeof(msg_dirreq),0) < 0){
    close(fdSocket);
    perror("Server recv() Failed!");
    exit(EXIT_FAILURE);
  }

  printf("Server - dirreq: receiving MSG_DIRREQ\n");

  msg_dirhdr.m_type = MSG_DIRHDR;
  //mutex
  msg_dirhdr.m_count = numOfsharedFiles;


  //sending to the client the num of the shared files inside msg_dirhdr
  if(send(fdSocket,&msg_dirhdr, sizeof(msg_dirhdr),0) < 0 ){
    close(fdSocket);
    perror("Server send() Failed!");
    exit(EXIT_FAILURE);
  }
  printf("Server - dirreq: sending MSG_DIRHDR with count = %d\n",msg_dirhdr.m_count);

  msg_dirent.m_type = MSG_DIRENT;
  //sending to the client all the shared files in a msg_dirent message every iteration
  int i = 0;
  for(; i < numOfsharedFiles; i++){
    //pthread_mutex_lock( &mutex );
    strcpy(msg_dirent.m_name,sharedFiles[i].fe_name);
    msg_dirent.m_addr = sharedFiles[i].fe_addr;
    msg_dirent.m_port = sharedFiles[i].fe_port;
      //pthread_mutex_unlock( &mutex );

    if(send(fdSocket,&msg_dirent, sizeof(msg_dirent),0) < 0 ){
      close(fdSocket);
      perror("Server send() Failed!");
      exit(EXIT_FAILURE);
    }

  }
  //pthread_mutex_unlock( &mutex );
}

/********************ackToTheNotify function******************/
void ackToTheNotify(int fdSocket) {

  msg_ack_t msg_ack;
  msg_notify_t msg_notify;

  //receiving a client notify for a shared file he have
  if(recv(fdSocket,&msg_notify, sizeof(msg_notify), 0) < 0 ) {
      perror("Server recv() Failed while receiving notify");
      close(fdSocket);
      exit(EXIT_FAILURE);
  }
  printf("Server - notify: receiving MSG_NOTIFY from %d\n",msg_notify.m_port);

  msg_ack.m_type = MSG_ACK;
  //if this is the first time the client send a notify message

  if(msg_notify.m_port == 0) {
    printf("Server - notify: assigned port %d\n", clientPort);
    //pthread_mutex_lock( &mutex );
    msg_ack.m_port = (in_port_t)clientPort++;
    //pthread_mutex_unlock( &mutex );
  }
  else//sending the client the same port they have to communicat
      msg_ack.m_port = msg_notify.m_port;

    //pthread_mutex_lock( &mutex );
    strcpy(sharedFiles[numOfsharedFiles].fe_name, msg_notify.m_name);
    sharedFiles[numOfsharedFiles].fe_addr = msg_notify.m_addr;
    sharedFiles[numOfsharedFiles].fe_port = msg_ack.m_port;
    numOfsharedFiles++;
  //pthread_mutex_unlock( &mutex );

  //sending to the client msg_ack for the notify he got
  if(send(fdSocket,&msg_ack, sizeof(msg_ack),0) < 0 ){
      perror("Server send() Failed while sending an ack");
      close(fdSocket);
      exit(EXIT_FAILURE);
  }
  printf("Server - notify: sending MSG_ACK\n");
}


/********************ServerShutDown function******************/
void ServerShutDown(int fdConnected) {
//pthread_mutex_lock( &mutex );
  msg_shutdown_t msg_shutdown;

  printf("Server - notify: receiving MSG_SHUTDOWN\n");

  if(recv(fdConnected,&msg_shutdown, sizeof(msg_shutdown), 0) < 0) {
    close(fdConnected);
    perror("Server recv() Failed!");
    exit(EXIT_FAILURE);
  }

    close(fdConnected);
    printf("Server - notify: Shutting Down,Bye Bye!\n");
  //pthread_mutex_unlock( &mutex );

    printf("Server : EXIT SUCCESSFULLY\n");
    exit(EXIT_SUCCESS);
}
