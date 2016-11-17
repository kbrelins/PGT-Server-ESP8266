#include "misc.h"
#include "time.h"
#include "sntp.h"

double ln_range_degrees (double angle)
{
    double temp;

    if (angle >= 0.0 && angle < 360.0)
        return angle;

    temp = (int)(angle / 360);
    if (angle < 0.0)
        temp --;
    temp *= 360;
    return angle - temp;
}

//input deg ,output hour
double sidereal_timeGMT (double longitude)
{
    struct timeval now;
    double JD;
    double sidereal,tmp;
    double T;
    gettimeofday(&now,NULL);
    time_t jnow = time(nullptr);
    JD = (jnow+now.tv_usec/ 1000000.0)/86400.0+2440587.5;
    T = (JD - 2451545.0) / 36525.0;
    /* calc mean angle */
    sidereal = 280.46061837 + (360.98564736629 * (JD - 2451545.0)) + (0.000387933 * T * T) - (T * T * T / 38710000.0);
    /* add a convenient multiple of 360 degrees */
    sidereal = ln_range_degrees (sidereal+longitude);
    /* change to hours */
    sidereal *= 24.0 / 360.0;

    return sidereal;
}

//input deg
double calc_Ra( double lha,double longitude)

{
    double sid=sidereal_timeGMT(longitude)*15.0;
    lha=(lha*180.0)/M_PI;
    if  (sid>=lha) return ((sid-lha) *M_PI)/180.0                             ;
    else return  ((sid-lha+360.0) *(M_PI))/180.0;
}
double calc_lha(double ra,double longitude)
{
    double sid=sidereal_timeGMT(longitude)*15.0;

    return (sid-(ra*180.0/M_PI)) *(M_PI/180.0);
}


void lxprintde1(char* message,double ang)

{
    if (ang>M_PI) ang=ang-(M_PI*2.0);

    int x = ang*RAD_TO_DEG*3600.0;
    char c='+';
    if (x<0)
    {
        x=-x;
        c= '-';
    }
    int gra=x/3600;
    int temp=(x %3600);
    int min=temp/60;
    int sec=temp%60;
    sprintf(message,"%c%02d%c%02d:%02d#",c,gra,225,min,sec);

};
void lxprintra1(char *message,double ang)
{
    int x = ang*RAD_TO_DEG*3600.0/15.0;
    int gra=x/3600;
    int temp=(x %3600);
    int min=temp/60;
    int sec=temp%60;
    sprintf(message,"%02d:%02d:%02d#",gra,min,sec);
    //APPEND
};
void config_NTP(int zone,int dls)
{
    configTime(zone*3600, dls*3600 ,"0.es.pool.ntp.org","cuco.rediris.es", "hora.roa.es");
}

void enc_to_eq(double x,double y,double *a,double  *b,char *pier)
{
    *a=x;
    *pier=true;
    if (y<=90.0) *b=y;
    else if(y<270.0)
    {
        {
            *b=180-y ;
            if (x<180.0) *a+=180.0 ;
            else *a-=180.0;
            *pier=false;
        }

    }
    else  *b=y-360.0;

}


void eq_to_enc(double *x,double *y,double a,double  b,char pier)
{

    if (pier)
    {
        *x=a;
        if (b<0.0)  *y=b+360.0;
        else *y=b;
    }
    else
    {
        *y=180-b;
        if (b>=180.0) *x=b-180;
        else *x=180-b;
    }
}



