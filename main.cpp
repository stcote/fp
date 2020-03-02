#include <QCoreApplication>
#include <QLocalSocket>
#include <QDate>
#include <stdio.h>

const QString FP_PIPE_NAME = "FP_Pipe";

const int NAME_MAX = 127;

typedef struct
{
    int key;
    char name[NAME_MAX+1];
    int  numItems;
    qint64 day;
} t_CheckIn;

//*****************************************************************************
//*****************************************************************************
int main( int argc, char *argv[] )
{
QLocalSocket sock;
t_CheckIn checkIn;
int checkInSize = sizeof(checkIn);
bool sentOK = false;

    //*** check for valid # parameters ***
    if ( argc < 4 )
    {
        printf( "Invalid # arguments\n" );
        return -1;
    }

//    printf( "1: %s\n2: %s\n3: %s\n", argv[1], argv[2], argv[3] );

    //*** connect to the checkin server socket ***
    sock.connectToServer( FP_PIPE_NAME, QIODevice::WriteOnly );

    //*** wait for the connection ***
    if ( !sock.waitForConnected( 5000 ) )
    {
        printf( "Timeout connecting to checkin server\n" );
        return -2;
    }

    //*** set up data to be sent to checkin server ***
    memset( &checkIn, 0, checkInSize );

    checkIn.key = atoi( argv[1] );
    qstrncpy( checkIn.name, argv[2], NAME_MAX );
    checkIn.numItems = atoi( argv[3] );
    checkIn.day = QDate::currentDate().toJulianDay();

    //*** send the data ***
    sentOK = (sock.write( (const char*)&checkIn, checkInSize ) == checkInSize );

    if ( !sentOK )
    {
        printf( "Error sending checkin data\n" );
        return -3;
    }

    printf( "Checkin data sent...\n" );

    //*** done with local socket ***
    sock.disconnectFromServer();

    return 0;
}
