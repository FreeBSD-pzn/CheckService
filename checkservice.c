#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main( int argc, char *argv[] )
{
 int sock;
 int i,len;
 struct sockaddr_in sa;
 struct hostent *hp;
 struct servent *sp;
 char buf[BUFSIZ+1];
 char strbuf[256];
 char *myprg;
 char *host;
 char *srv;
 char *srvname;
 char *nothing = "nothing";

 myprg = argv[0];
 if( argc != 4 )
	{
	  len = fprintf( stderr, "\nUsage: %s hostname servicename servicetype.\n", myprg );
	  len -= 2; /* Exclude 0 from the lenght and correct to count from 0 */
	  for( i=0; i<len && i<255; i++)
		strbuf[i] = '-';
	  /* Ending C string with 0 */
	  strbuf[i] = '\0';
	  fprintf( stderr, "%s\n", strbuf );
	  fprintf( stderr, "\tfor instance: myhost xdmcp udp.\n" );
	  exit( 1 );
	}

 host = argv[1];
 srv  = argv[2];
 srvname = argv[3];

 if( (hp = gethostbyname( host )) == NULL )
	{
	  fprintf( stderr, "%s: No such HOST: %s.\n", myprg, host );
	  exit( 1 );
	}

 fprintf( stderr, "%s: HOST name : %s.\n", myprg, host );

 bcopy( (char *)hp->h_addr, (char *)&sa.sin_addr, hp->h_length );
 sa.sin_family = hp->h_addrtype;

 if( (sp = getservbyname( srv, srvname ) ) == NULL )
	{
	  fprintf( stderr, "%s: No such service NAME: %s, or service TYPE: %s.\n", myprg, srv, srvname );
	  exit( 1 );
	}

 fprintf( stderr, "%s: service name is         : %s.\n", myprg, sp->s_name );
 fprintf( stderr, "%s: port service resides at : %d.\n", myprg, sp->s_port );
 fprintf( stderr, "%s: protocol to use         : %s.\n", myprg, sp->s_proto );
 fprintf( stderr, "%s: aliases                 : %s.\n", myprg, (*sp->s_aliases)?*sp->s_aliases:nothing );

 sa.sin_port = sp->s_port;

 if( (sock = socket( hp->h_addrtype, SOCK_DGRAM, 0) ) < 0 )
	{
	  perror( "Socket" );
	  exit( 1 );
	}

 fprintf( stderr, "%s: Socket created.\n", myprg );

 if( connect( sock, (struct sockaddr *)&sa, sizeof( sa ) ) < 0 )
	{
	  perror( "Connection" );
	  close( sock );
	  exit( 1 );
	}

 fprintf( stderr, "Connection is made.\n" );

 fprintf( stderr, "-------------------------\nPress Enter to continue ...\n" );
 getchar();

 close( sock );
 exit( 0 );

}