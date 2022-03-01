#include<math.h>
#include<Eigen/Dense>
#include <stdio.h>
#include<iostream>
#include <iomanip>


#ifndef MAX
#define MAX(a,b)  ((a) < (b) ? (b) : (a))

#endif


/*************************************************************
 * 一般实矩阵的奇异值分解，参见《c 常用算法程序集》徐世良P169
 *************************************************************/
#ifndef _SVD_H
#define _SVD_H
#define MAX_ITERA 60
#define MIN_DOUBLE (1e-30)

#include "stdlib.h"
#include "math.h"

int dluav(double a[],int m,int n,double u[],double v[],double eps,int ka);
void damul(double a[],double b[],int m,int n,int k,double c[]);
void ppp(double a[],double e[],double s[],double v[],int m,int n);
void sss(double fg[2],double cs[2]);

#endif


/*********************************************************************
 * 矩阵的奇异值分解，参见《c 常用算法程序集》徐世良P169
 * 参数说明：
 * a m*n的实矩阵，返回时其对角线给出奇异值（非递增顺序），其余元素为0
 * m,n 矩阵A的行数和列数
 * u m*m的矩阵，存放左奇异向量
 * v n*n的矩阵，存放右奇异向量
 * eps 双精度实型变量，给定精度要求
 * ka 整形变量，其值为max(m,n)+1
 * 返回值：如果返回标志小于0，则说明出现了迭代MAX_ITERA次还未求得某个
 * 奇异值的情况，此时矩阵A的分解式为UAV，如果返回标志大于0，则说明
 * 程序正常运行
 ********************************************************************/
int dluav(double a[],int m,int n,double u[],double v[],double eps,int ka)
{
	int i,j,k,l,it,ll,kk,ix,iy,mm,nn,iz,ml,ks;
	double d,dd,t,sm,sml,eml,sk,ek,b,c,shh,fg[2],cs[2];
	double *s,*e,*w;
	s=(double*)malloc(ka*sizeof(double));
	e=(double*)malloc(ka*sizeof(double));
	w=(double*)malloc(ka*sizeof(double));
	for(i=1;i<=m;i++)
	{
		ix=(i-1)*m+i-1;
		u[ix]=0;
	}
	for(i=1;i<=n;i++)
	{
		iy=(i-1)*n+i-1;
		v[iy]=0;
	}
	it=MAX_ITERA;k=n;
	if(m-1<n)
		k=m-1;
	l=m;
	if(n-2<m) l=n-2;
	if(l<0) l=0;
	ll=k;
	if(l>k) ll=l;
	if(ll>=1)
	{
		for(kk=1;kk<=ll;kk++)
		{
			if(kk<=k)
			{
				d=0.0;
				for(i=kk;i<=m;i++)
				{
					ix=(i-1)*n+kk-1;d=d+a[ix]*a[ix];
				}
				s[kk-1]=sqrt(d);
				//if(s[kk-1]!=0.0)
				if(fabs(s[kk-1])>MIN_DOUBLE)
				{
					ix=(kk-1)*n+kk-1;
					//if(a[ix]!=0.0)
					if(fabs(a[ix])>MIN_DOUBLE)
					{
						s[kk-1]=fabs(s[kk-1]);
						if(a[ix]<0.0) s[kk-1]=-s[kk-1];
					}
					for(i=kk;i<=m;i++)
					{
						iy=(i-1)*n+kk-1;
						a[iy]=a[iy]/s[kk-1];
					}
					a[ix]=1.0+a[ix];
				}
				s[kk-1]=-s[kk-1];
			}
			if(n>=kk+1)
			{
				for(j=kk+1;j<=n;j++)
				{
					//if((kk<=k)&&(s[kk-1]!=0.0))
					if((kk<=k)&&(fabs(s[kk-1])>MIN_DOUBLE))
					{
						d=0.0;
						for(i=kk;i<=m;i++)
						{
							ix=(i-1)*n+kk-1;
							iy=(i-1)*n+j-1;
							d=d+a[ix]*a[iy];
						}
						d=-d/a[(kk-1)*n+kk-1];
						for(i=kk;i<=m;i++)
						{
							ix=(i-1)*n+j-1;
							iy=(i-1)*n+kk-1;
							a[ix]=a[ix]+d*a[iy];
						}
					}
					e[j-1]=a[(kk-1)*n+j-1];
				}
			}
			if(kk<=k)
			{
				for(i=kk;i<=m;i++)
				{
					ix=(i-1)*m+kk-1;iy=(i-1)*n+kk-1;
					u[ix]=a[iy];
				}
			}
			if(kk<=l)
			{
				d=0.0;
				for(i=kk+1;i<=n;i++)
					d=d+e[i-1]*e[i-1];
				e[kk-1]=sqrt(d);
				//if(e[kk-1]!=0.0)
				if(fabs(e[kk-1])>MIN_DOUBLE)
				{
					//if(e[kk]!=0.0)
					if(fabs(e[kk])>MIN_DOUBLE)
					{
						e[kk-1]=fabs(e[kk-1]);
						if(e[kk]<0.0)
							e[kk-1]=-e[kk-1];
					}
					for(i=kk+1;i<=n;i++)
						e[i-1]=e[i-1]/e[kk-1];
					e[kk]=1.0+e[kk];
				}
				e[kk-1]=-e[kk-1];
				//if((kk+1<=m)&&(e[kk-1]!=0.0))
				if((kk+1<=m)&&(fabs(e[kk-1])>MIN_DOUBLE))
				{
					for(i=kk+1;i<=m;i++) w[i-1]=0.0;
					for(j=kk+1;j<=n;j++)
						for(i=kk+1;i<=m;i++)
							w[i-1]=w[i-1]+e[j-1]*a[(i-1)*n+j-1];
					for(j=kk+1;j<=n;j++)
						for(i=kk+1;i<=m;i++)
						{
							ix=(i-1)*n+j-1;
							a[ix]=a[ix]-w[i-1]*e[j-1]/e[kk];
						}
				}
				for(i=kk+1;i<=n;i++)
					v[(i-1)*n+kk-1]=e[i-1];
			}
		}
	}
	mm=n;
	if(m+1<n) mm=m+1;
	if(k<n) s[k]=a[k*n+k];
	if(m<mm) s[mm-1]=0.0;
	if(l+1<mm) e[l]=a[l*n+mm-1];
	e[mm-1]=0.0;
	nn=m;
	if(m>n) nn=n;
	if(nn>=k+1)
	{
		for(j=k+1;j<=nn;j++)
		{
			for(i=1;i<=m;i++)
				u[(i-1)*m+j-1]=0.0;
			u[(j-1)*m+j-1]=1.0;
		}
	}
	if(k>=1)/////////////////////////////////
	{
		for(ll=1;ll<=k;ll++)
		{
			kk=k-ll+1;iz=(kk-1)*m+kk-1;
			//if(s[kk-1]!=0.0)
			if(fabs(s[kk-1])>MIN_DOUBLE)
			{
				if(nn>=kk+1)
					for(j=kk+1;j<=nn;j++)
					{
						d=0.0;
						for(i=kk;i<=m;i++)
						{
							ix=(i-1)*m+kk-1;
							iy=(i-1)*m+j-1;
							d=d+u[ix]*u[iy]/u[iz];
						}
						d=-d;
						for(i=kk;i<=m;i++)
						{
							ix=(i-1)*m+j-1;
							iy=(i-1)*m+kk-1;
							u[ix]=u[ix]+d*u[iy];
						}
					}
					for(i=kk;i<=m;i++)
					{
						ix=(i-1)*m+kk-1;
						u[ix]=-u[ix];
					}
					u[iz]=1.0+u[iz];
					if(kk-1>=1)//////////////////////////////////////
						for(i=1;i<=kk-1;i++)
							u[(i-1)*m+kk-1]=0.0;
			}
			else
			{
				for(i=1;i<=m;i++)
					u[(i-1)*m+kk-1]=0.0;
				u[(kk-1)*m+kk-1]=1.0;
			}
		}
	}
	for(ll=1;ll<=n;ll++)
	{
		kk=n-ll+1;iz=kk*n+kk-1;
		//if((kk<=l)&&(e[kk-1]!=0.0))/////////////////////////////
		if((kk<=l)&&(fabs(e[kk-1])>MIN_DOUBLE))
		{
			for(j=kk+1;j<=n;j++)
			{
				d=0.0;
				for(i=kk+1;i<=n;i++)
				{
				ix=(i-1)*n+kk-1;iy=(i-1)*n+j-1;
				d=d+v[ix]*v[iy]/v[iz];
				}
				d=-d;
				for(i=kk+1;i<=n;i++)
				{
					ix=(i-1)*n+j-1;iy=(i-1)*n+kk-1;
					v[ix]=v[ix]+d*v[iy];
				}
			}
		}
		for(i=1;i<=n;i++)
			v[(i-1)*n+kk-1]=0.0;
		v[iz-n]=1.0;
	}
	for(i=1;i<=m;i++)
		for(j=1;j<=n;j++)
			a[(i-1)*n+j-1]=0.0;
	ml=mm;
	it=MAX_ITERA;
	while(1==1)//////////////////////////////////
	{
		if(mm==0)
		{
			ppp(a,e,s,v,m,n);
			free(s);free(e);free(w);
			return l;
		}
		if(it==0)
		{
			ppp(a,e,s,v,m,n);
			free(s);free(e);free(w);
			return -1;
		}
		kk=mm-1;
		//while((kk!=0)&&(fabs(e[kk-1])!=0.0))
		while((kk!=0)&&(fabs(e[kk-1])>MIN_DOUBLE))
		{
			d=fabs(s[kk-1])+fabs(s[kk]);
			dd=fabs(e[kk-1]);
			if(dd>eps*d)
				kk=kk-1;
			else
				e[kk-1]=0.0;
		}
		if(kk==mm-1)
		{
			kk=kk+1;
			if(s[kk-1]<0.0)
			{
				s[kk-1]=-s[kk-1];
				for(i=1;i<=n;i++)
				{
					ix=(i-1)*n+kk-1;
					v[ix]=-v[ix];
				}
			}
			while((kk!=ml)&&(s[kk-1]<s[kk]))
			{
				d=s[kk-1];s[kk-1]=s[kk];s[kk]=d;
				if(kk<n)
					for(i=1;i<=n;i++)
					{
						ix=(i-1)*n+kk-1;iy=(i-1)*n+kk;
						d=v[ix];v[ix]=v[iy];v[iy]=d;
					}
					if(kk<m)
						for(i=1;i<=m;i++)
						{
							ix=(i-1)*m+kk-1;
							iy=(i-1)*m+kk;
							d=u[ix];u[ix]=u[iy];u[iy]=d;
						}
						kk=kk+1;
			}
			it=MAX_ITERA;
			mm=mm-1;
		}
		else
		{
			ks=mm;
			//while((ks>kk)&&(fabs(s[ks-1])!=0.0))
			while((ks>kk)&&(fabs(s[ks-1])>MIN_DOUBLE))
			{
				d=0.0;
				if(ks!=mm)
					d=d+fabs(e[ks-1]);
				if(ks!=kk+1) d=d+fabs(e[ks-2]);
				dd=fabs(s[ks-1]);
				if(dd>eps*d)
					ks=ks-1;
				else
					s[ks-1]=0.0;
			}
			if(ks==kk)
			{
				kk=kk+1;
				d=fabs(s[mm-1]);
				t=fabs(s[mm-2]);
				if(t>d)
					d=t;
				t=fabs(e[mm-2]);
				if(t>d)
					d=t;
				t=fabs(s[kk-1]);
				if(t>d)
					d=t;
				t=fabs(e[kk-1]);
				if(t>d)
					d=t;
				sm=s[mm-1]/d;sml=s[mm-2]/d;
				eml=e[mm-2]/d;
				sk=s[kk-1]/d;ek=e[kk-1]/d;
				b=((sml+sm)*(sml-sm)+eml*eml)/2.0;
				c=sm*eml;c=c*c;shh=0.0;
				//if((b!=0.0)||(c!=0.0))
				if((fabs(b)>MIN_DOUBLE)||(fabs(c)>MIN_DOUBLE))
				{
					shh=sqrt(b*b+c);
					if(b<0.0)
						shh=-shh;
					shh=c/(b+shh);
				}
				fg[0]=(sk+sm)*(sk-sm)-shh;
				fg[1]=sk*ek;
				for(i=kk;i<=mm-1;i++)
				{
					sss(fg,cs);
					if(i!=kk)
						e[i-2]=fg[0];
					fg[0]=cs[0]*s[i-1]+cs[1]*e[i-1];
					e[i-1]=cs[0]*e[i-1]-cs[1]*s[i-1];
					fg[1]=cs[1]*s[i];
					s[i]=cs[0]*s[i];
					//if((cs[0]!=1.0)||(cs[1]!=0.0))
					if((fabs(cs[0]-1.0)>MIN_DOUBLE)||(fabs(cs[1])>MIN_DOUBLE))
						for(j=1;j<=n;j++)
						{
							ix=(j-1)*n+i-1;
							iy=(j-1)*n+i;
							d=cs[0]*v[ix]+cs[1]*v[iy];
							v[iy]=-cs[1]*v[ix]+cs[0]*v[iy];
							v[ix]=d;
						}
					sss(fg,cs);
					s[i-1]=fg[0];
					fg[0]=cs[0]*e[i-1]+cs[1]*s[i];
					s[i]=-cs[1]*e[i-1]+cs[0]*s[i];
					fg[1]=cs[1]*e[i];
					e[i]=cs[0]*e[i];
					if(i<m)
						//if((cs[0]!=1.0)||(cs[1]!=0.0))
						if((fabs(cs[0]-1.0)>MIN_DOUBLE)||(fabs(cs[1])>MIN_DOUBLE))
							for(j=1;j<=m;j++)
							{
								ix=(j-1)*m+i-1;
								iy=(j-1)*m+i;
								d=cs[0]*u[ix]+cs[1]*u[iy];
								u[iy]=-cs[1]*u[ix]+cs[0]*u[iy];
								u[ix]=d;
							}
				}
				e[mm-2]=fg[0];
				it=it-1;
			}
			else
			{
				if(ks==mm)
				{
					kk=kk+1;
					fg[1]=e[mm-2];e[mm-2]=0.0;
					for(ll=kk;ll<=mm-1;ll++)
					{
						i=mm+kk-ll-1;
						fg[0]=s[i-1];
						sss(fg,cs);
						s[i-1]=fg[0];
						if(i!=kk)
						{
							fg[1]=-cs[1]*e[i-2];
							e[i-2]=cs[0]*e[i-2];
						}
						//if((cs[0]!=1.0)||(cs[1]!=0.0))
						if((fabs(cs[0]-1.0)>MIN_DOUBLE)||(fabs(cs[1])>MIN_DOUBLE))
							for(j=1;j<=n;j++)
							{
								ix=(j-1)*n+i-1;
								iy=(j-1)*n+mm-1;
								d=cs[0]*v[ix]+cs[1]*v[iy];
								v[iy]=-cs[1]*v[ix]+cs[0]*v[iy];
								v[ix]=d;
							}
					}
				}
				else
				{
					kk=ks+1;
					fg[1]=e[kk-2];
					e[kk-2]=0.0;
					for(i=kk;i<=mm;i++)
					{
						fg[0]=s[i-1];
						sss(fg,cs);
						s[i-1]=fg[0];
						fg[1]=-cs[1]*e[i-1];
						e[i-1]=cs[0]*e[i-1];
						//if((cs[0]!=1.0)||(cs[1]!=0.0))
						if((fabs(cs[0]-1.0)>MIN_DOUBLE)||(fabs(cs[1])>MIN_DOUBLE))
							for(j=1;j<=m;j++)
							{
								ix=(j-1)*m+i-1;
								iy=(j-1)*m+kk-2;
								d=cs[0]*u[ix]+cs[1]*u[iy];								
								u[iy]=-cs[1]*u[ix]+cs[0]*u[iy];
								u[ix]=d;
							}
					}
				}
			}
			}
			}
			free(s);free(e);free(w);
			return l;
}
void ppp(double a[],double e[],double s[],double v[],int m,int n)
{
	int i,j,p,q;
	double d;
	if(m>=n)
		i=n;
	else
		i=m;
	for(j=1;j<=i-1;j++)
	{
		a[(j-1)*n+j-1]=s[j-1];
		a[(j-1)*n+j]=e[j-1];
	}
	a[(i-1)*n+i-1]=s[i-1];
	if(m<n)
		a[(i-1)*n+i]=e[i-1];
	for(i=1;i<=n-1;i++)
	for(j=i+1;j<=n;j++)
	{
		p=(i-1)*n+j-1;
		q=(j-1)*n+i-1;
		d=v[p];v[p]=v[q];v[q]=d;
	}
	return;
}
void sss(double fg[2],double cs[2])
{
	double r,d;
	//if((fabs(fg[0])+fabs(fg[1]))==0.0)
	if((fabs(fg[0])+fabs(fg[1]))<MIN_DOUBLE)
	{
		cs[0]=1.0;cs[1]=0.0;d=0.0;
	}
	else
	{
		d=sqrt(fg[0]*fg[0]+fg[1]*fg[1]);
		if(fabs(fg[0])>fabs(fg[1]))
		{
			d=fabs(d);
			if(fg[0]<0.0)
				d=-d;
		}
		if(fabs(fg[1])>=fabs(fg[0]))
		{
			d=fabs(d);
			if(fg[1]<0.0)
				d=-d;
		}
		cs[0]=fg[0]/d;
		cs[1]=fg[1]/d;
	}
	r=1.0;
	if(fabs(fg[0])>fabs(fg[1]))
		r=cs[1];
	else
		//if(cs[0]!=0.0)
		if(fabs(cs[0])>MIN_DOUBLE)
			r=1.0/cs[0];
	fg[0]=d;
	fg[1]=r;
	return;
}
//矩阵相乘
void damul(double a[],double b[],int m,int n,int k,double c[])
{
	int i,j,l,u;
	for(i=0;i<=m-1;i++)
		for(j=0;j<=k-1;j++)
		{
			u=i*k+j;
			c[u]=0;
			for(l=0;l<=n-1;l++)
				c[u]=c[u]+a[i*n+l]*b[l*k+j];
		}
	return;
}



 void matrixMul(double *const A, double *const B, double *const C,int PointNum,int two,int two_B)
{
    double sub = 0.0;
    int i, j, k;
    for (i = 0; i < PointNum; i++)
    {
        for (j = 0; j < two_B; j++)
        {
            sub = 0.0;
            for (k = 0; k < two; k++)
            {
                sub += A[i*two+k]*B[k*two_B+j];
            }
            C[i*two_B+j] = sub;
        }
    }
}
 void rodrigues2(double src[], double dst[])///Matrix to Vector
{
    double R[9], U[9], V[9], rx, ry, rz;
    double theta, s, c;
    double eps = 0.000001;
    dluav(src,3,3,U,V,eps,4);//SVD分解

    matrixMul(U,V, R,3,3,3);
    rx = R[7] - R[5];
    ry = R[2] - R[6];
    rz = R[3] - R[1];

    s = sqrt((rx*rx + ry*ry + rz*rz)*0.25);
    c = (R[0] + R[4] + R[8] - 1)*0.5;
    c = c > 1. ? 1. : c < -1. ? -1. : c;
    theta = acos(c);

    if( s < 1e-5 )
    {
        double t;
        if( c > 0 )
          rx = ry = rz = 0;
        else
        {
            t = (R[0] + 1)*0.5;
            rx = sqrt(MAX(t,0.));
            t = (R[4] + 1)*0.5;
            ry = sqrt(MAX(t,0.))*(R[1] < 0 ? -1. : 1.);
            t = (R[8] + 1)*0.5;
            rz = sqrt(MAX(t,0.))*(R[2] < 0 ? -1. : 1.);
            if( fabs(rx) < fabs(ry) && fabs(rx) < fabs(rz) && (R[5] > 0) != (ry*rz > 0) )
                    rz = -rz;
            theta /= sqrt(rx*rx + ry*ry + rz*rz);
            rx *= theta;
            ry *= theta;
            rz *= theta;
        }
    }
    else
    {
        double vth = 1/(2*s);
        vth *= theta;
        rx *= vth; ry *= vth; rz *= vth;
    }
    dst[0]=rx;
    dst[1]=ry;
    dst[2]=rz;
}


 double* juzhen( double theta[6])//DH参数
{	const double pi= 3.1415926;
	double a[6]={0,0, 134 ,0 ,0 ,0}; 
	double alpha[6]={pi/2, -pi/2 ,0 ,-pi/2, pi/2, -pi/2};
	//double theta[6]={pi/2 ,0, 0, 0 ,pi/2, -pi/2};
	double d[6]={0 , 0 ,0 ,132.5, 0 ,23};
 /*double a[6]={0.0 ,0.0, 134 ,0 ,0 ,0}; 
	double alpha[6]={0, -pi/2 ,0 ,-pi/2,pi/2,-pi/2};
	//double theta[6]={0 ,0, 0, 0 ,0, pi/2};
	double d[6]={120 , 0,0 ,132.5, 0,23};*/
	double *a0=a;
	double *alpha0=alpha;
	double *theta0=theta;
	double *d0=d;
		//printf("%f",*(p+2));
	//for(int i=0;i<6;i++)
	int i=0;
	 Eigen::Matrix< double , 4 , 4> T1,T2,T3,T4,T5,T6;   
	T1<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	 /* */  
	i=1;
	T2<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	i=2;
	T3<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	i=3;
	T4<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	i=4;
	T5<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	i=5;
	T6<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	
		Eigen::Matrix4d T1_6 = Eigen::Matrix4d::Random();
		T1_6=T1*T2*T3*T4*T5*T6;
		std::cout <<T1  << std::endl;
		std::cout <<T2  << std::endl;
		std::cout <<T3  << std::endl;
		std::cout <<T4  << std::endl;
		std::cout <<T5  << std::endl;
		std::cout <<T6  << std::endl;
		std::cout << "Here is T1_6:\n" <<T1_6 << std::endl;
		Eigen::Matrix3d Rotation_Matrix= Eigen::Matrix3d::Random();
		Rotation_Matrix=T1_6.block<3,3>(0,0); 
		std::cout <<"Here is Rotation_Matrix:\n" << Rotation_Matrix << std::endl << std::endl;

	    
		double c[9]={Rotation_Matrix(0,0), Rotation_Matrix(0,1), Rotation_Matrix(0,2),
                 Rotation_Matrix(1,0), Rotation_Matrix(1,1), Rotation_Matrix(1,2),
                 Rotation_Matrix(2,0), Rotation_Matrix(2,1), Rotation_Matrix(2,2)};
		double V3[3];
		 rodrigues2(c,V3); //Matrix to Vector
		std::cout << std::endl;
		printf("here is Rotation_ Vector:\n");
	 for(int i=0; i<3; i++)
    {
        std::cout << V3[i] << std::endl;
    }
	double* temp = V3; 


	return temp;	
}


 double *MatrixToVector(double *matrix1)
 {
	double rollmatrix1[9]={	*matrix1	,*(matrix1+1),*(matrix1+2),
							*(matrix1+4),*(matrix1+5),*(matrix1+6),
							*(matrix1+8),*(matrix1+9),*(matrix1+10)};
	 double V3[3];
		 rodrigues2(rollmatrix1,V3); //Matrix to Vector
		std::cout << std::endl;
		printf("here is Rotation_ Vector:\n");
	 for(int i=0; i<3; i++)
    {
        std::cout << V3[i] << std::endl;
    }
	double* temp = V3; 


	return temp;
 }
 double* InverseKinematics1( double roll[12])//改进后的DH参数~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~·
{	const double pi= 3.1415926;
	double a[6]={0,0, -425 ,-392.5 ,0 ,0}; 
	double alpha[6]={0, pi/2 ,0 ,0,pi/2,-pi/2};
	
	double d[6]={89.2 , 0,0 ,109.3, 94.75,82.5};
	double *a0=a;
	double *alpha0=alpha;
	double *d0=d;
		
	 Eigen::Matrix< double , 4 , 4> T0_6,T0_6T;
	Eigen::Matrix< double , 4 , 1> T1,P0_5; 
	T0_6<<	roll[0],roll[1],roll[2],roll[3],
			roll[4],roll[5],roll[6],roll[7],
			roll[8],roll[9],roll[10],roll[11],
			0,		0,		0,		1;
	
	/////////////////////////////////////////////////////theta1求解///////////////////////////////////////
	T1<<0,0,-(*(d0+5)),1;
	
	P0_5=T0_6*T1;
	double P0_5x=P0_5(0,0);
	double P0_5y=P0_5(1,0);

	double angle1=atan2(P0_5y,P0_5x);
	double num1=(*(d0+3))/sqrt(P0_5x*P0_5x+P0_5y*P0_5y);
	if(fabs(num1-1)<=0.000001)
	{num1=1;}
	double angle2=acos(num1);
	
	
	
	double theta1_1=angle1+angle2+pi/2;
	double theta1_2=angle1-angle2+pi/2;
		if(theta1_1>=pi)
	{theta1_1-=2*pi;}
		if(theta1_2>=pi)
	{theta1_2-=2*pi;}
	double resultTheta1[8]={theta1_1,theta1_1,theta1_1,theta1_1,theta1_2,theta1_2,theta1_2,theta1_2};
	/////////////////////////////////////////////////////theta5求解//////////////////////////////////////////

	double theta5_1=acos(    (T0_6(0,3)*sin(theta1_1)-T0_6(1,3)*cos(theta1_1)-(*(d0+3)))/(*(d0+5))   );
	double theta5_2=-theta5_1;
	double theta5_3=acos(    (T0_6(0,3)*sin(theta1_2)-T0_6(1,3)*cos(theta1_2)-(*(d0+3)))/(*(d0+5))   );
	double theta5_4=-theta5_3;
	double resultTheta5[8]={theta5_1,theta5_1,theta5_2,theta5_2,theta5_3,theta5_3,theta5_4,theta5_4};

	/////////////////////////////////////////////////////theta6求解//////////////////////////////////////////
	T0_6T=T0_6.inverse();
	std::cout << "T0_6.transpose()\n" <<T0_6.transpose()<< std::endl;
	std::cout << "T0_6.inverse()\n" <<T0_6.inverse()<< std::endl;

	double theta6_1=atan2(	(-(T0_6T(1,0))*sin(theta1_1)+(T0_6T(1,1))*cos(theta1_1))/sin(theta5_1),
							(+(T0_6T(0,0))*sin(theta1_1)-(T0_6T(0,1))*cos(theta1_1))/sin(theta5_1)   );
	//std::cout << "fenmu\n" <<(-(T0_6T(1,0))*sin(theta1_1)+(T0_6T(1,1))*cos(theta1_1))/sin(theta5_1)<< std::endl;
	//std::cout << "fenzi\n" <<(+(T0_6T(0,0))*sin(theta1_1)-(T0_6T(0,1))*cos(theta1_1))/sin(theta5_1)<< std::endl;
	double theta6_2=atan2(	(-(T0_6T(1,0))*sin(theta1_1)+(T0_6T(1,1))*cos(theta1_1))/sin(theta5_2),
							(+(T0_6T(0,0))*sin(theta1_1)-(T0_6T(0,1))*cos(theta1_1))/sin(theta5_2)   );
	double theta6_3=atan2(	(-(T0_6T(1,0))*sin(theta1_2)+(T0_6T(1,1))*cos(theta1_2))/sin(theta5_3),
							(+(T0_6T(0,0))*sin(theta1_2)-(T0_6T(0,1))*cos(theta1_2))/sin(theta5_3)   );
	double theta6_4=atan2(	(-(T0_6T(1,0))*sin(theta1_2)+(T0_6T(1,1))*cos(theta1_2))/sin(theta5_4),
							(+(T0_6T(0,0))*sin(theta1_2)-(T0_6T(0,1))*cos(theta1_2))/sin(theta5_4)   );
	double resultTheta6[8]={theta6_1,theta6_1, theta6_2, theta6_2,theta6_3,theta6_3,theta6_4,theta6_4};
		/*printf("theta1_1:%lf,theta1_2:%lf\n",theta1_1,theta1_2);
		std::cout << "theta5_1\n" <<theta5_1 << std::endl;
		std::cout << "theta5_2\n" <<theta5_2 << std::endl;
		std::cout << "theta5_3 \n" <<theta5_3 << std::endl;
		std::cout << "theta5_4 \n" <<theta5_4 << std::endl;
		std::cout << "theta6_1 \n" <<theta6_1 << std::endl;
		std::cout << "theta6_2 \n" <<theta6_2 << std::endl;
		std::cout << "theta6_3 \n" <<theta6_3 << std::endl;
		std::cout << "theta6_4 \n" <<theta6_4 << std::endl;*/
	/////////////////////////////////////////////////////theta3求解///////////////////////////////////////
		int i1=0;
		double theta[6];
		theta[0]=resultTheta1[i1],theta[4]=resultTheta5[i1],theta[5]=resultTheta6[i1];
		double*theta0=theta;
		Eigen::Matrix< double , 4 , 4> T0_1,T1_2,T2_3,T3_4,T4_5,T5_6,T4_6,T1_4;   


		int i=0;
	T0_1<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;

		i=4;
	
	T4_5<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	
		i=5;
	T5_6<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	
	T4_6=T4_5*T5_6;
	
	T1_4=(T0_1.inverse())*T0_6*(T4_6.inverse());
	
	double P1_4xzSquare=T1_4(0,3)*T1_4(0,3)+T1_4(2,3)*T1_4(2,3);

	double num2=(P1_4xzSquare-(*(a0+2))*(*(a0+2))-(*(a0+3))*(*(a0+3)))/(2*(*(a0+2))*(*(a0+3))) ;
	//std::cout << "P1_4xzSquare\n" <<P1_4xzSquare << std::endl;
	//std::cout <<(P1_4xzSquare-(*(a0+2))*(*(a0+2))-(*(a0+3))*(*(a0+3)))<< std::endl;
	//std::cout << "num2\n" <<num2 << std::endl;
	if(num2>1&&fabs(num2-1)<0.01)
	{num2=1;}
	double theta3_1=+acos(  num2   );
	double theta3_2=-theta3_1;
	
	
	double theta2_1=atan2(-T1_4(2,3),-T1_4(0,3))-asin(-(*(a0+3))*sin(theta3_1)/sqrt(P1_4xzSquare));
	theta[1]=theta2_1,theta[2]=theta3_1;
	i=1;
	T1_2<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	i=2;
	T2_3<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	T3_4=(T1_2*T2_3).inverse()*T1_4;
	double theta4_1=atan2(T3_4(1,0),T3_4(0,0));

	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`theta2_2\theta3_2\theta4_2~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	double theta2_2=atan2(-T1_4(2,3),-T1_4(0,3))-asin(-(*(a0+3))*sin(theta3_2)/sqrt(P1_4xzSquare));
	theta[1]=theta2_2,theta[2]=theta3_2;
	i=1;
	T1_2<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	i=2;
	T2_3<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	T3_4=(T1_2*T2_3).inverse()*T1_4;
	double theta4_2=atan2(T3_4(1,0),T3_4(0,0));
	
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~theta3_3-theta3_4~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 i1=2;
	 theta[0]=resultTheta1[i1],theta[4]=resultTheta5[i1],theta[5]=resultTheta6[i1];
	  i=0;
	T0_1<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;

		i=4;
	
	T4_5<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	
		i=5;
	T5_6<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	
	T4_6=T4_5*T5_6;
	
	T1_4=(T0_1.inverse())*T0_6*(T4_6.inverse());
	
	P1_4xzSquare=T1_4(0,3)*T1_4(0,3)+T1_4(2,3)*T1_4(2,3);

	num2=(P1_4xzSquare-(*(a0+2))*(*(a0+2))-(*(a0+3))*(*(a0+3)))/(2*(*(a0+2))*(*(a0+3))) ;
	if(num2>1&&fabs(num2-1)<0.01)
	{num2=1;}
	double theta3_3=+acos(  num2   );
	double theta3_4=-theta3_3;
	

	double theta2_3=atan2(-T1_4(2,3),-T1_4(0,3))-asin(-(*(a0+3))*sin(theta3_3)/sqrt(P1_4xzSquare));
	theta[1]=theta2_3,theta[2]=theta3_3;
	i=1;
	T1_2<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	i=2;
	T2_3<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	T3_4=(T1_2*T2_3).inverse()*T1_4;
	double theta4_3=atan2(T3_4(1,0),T3_4(0,0));

	double theta2_4=atan2(-T1_4(2,3),-T1_4(0,3))-asin(-(*(a0+3))*sin(theta3_4)/sqrt(P1_4xzSquare));
	theta[1]=theta2_4,theta[2]=theta3_4;
	i=1;
	T1_2<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	i=2;
	T2_3<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	T3_4=(T1_2*T2_3).inverse()*T1_4;
	double theta4_4=atan2(T3_4(1,0),T3_4(0,0));
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~theta3_5-theta3_6~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 i1=4;
	 theta[0]=resultTheta1[i1],theta[4]=resultTheta5[i1],theta[5]=resultTheta6[i1];
	  i=0;
	T0_1<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;

		i=4;
	
	T4_5<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	
		i=5;
	T5_6<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	
	T4_6=T4_5*T5_6;
	
	T1_4=(T0_1.inverse())*T0_6*(T4_6.inverse());
	
	P1_4xzSquare=T1_4(0,3)*T1_4(0,3)+T1_4(2,3)*T1_4(2,3);

	num2=(P1_4xzSquare-(*(a0+2))*(*(a0+2))-(*(a0+3))*(*(a0+3)))/(2*(*(a0+2))*(*(a0+3))) ;
	if(num2>1&&fabs(num2-1)<0.01)
	{num2=1;}
	double theta3_5=+acos(  num2   );
	double theta3_6=-theta3_5;
	

	double theta2_5=atan2(-T1_4(2,3),-T1_4(0,3))-asin(-(*(a0+3))*sin(theta3_5)/sqrt(P1_4xzSquare));
	theta[1]=theta2_5,theta[2]=theta3_5;
	i=1;
	T1_2<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	i=2;
	T2_3<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	T3_4=(T1_2*T2_3).inverse()*T1_4;
	double theta4_5=atan2(T3_4(1,0),T3_4(0,0));

	double theta2_6=atan2(-T1_4(2,3),-T1_4(0,3))-asin(-(*(a0+3))*sin(theta3_6)/sqrt(P1_4xzSquare));
	theta[1]=theta2_6,theta[2]=theta3_6;
	i=1;
	T1_2<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	i=2;
	T2_3<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	T3_4=(T1_2*T2_3).inverse()*T1_4;
	double theta4_6=atan2(T3_4(1,0),T3_4(0,0));
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~theta3_7-theta3_8~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 i1=6;
	 theta[0]=resultTheta1[i1],theta[4]=resultTheta5[i1],theta[5]=resultTheta6[i1];
	  i=0;
	T0_1<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;

		i=4;
	
	T4_5<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	
		i=5;
	T5_6<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	
	T4_6=T4_5*T5_6;
	
	T1_4=(T0_1.inverse())*T0_6*(T4_6.inverse());
	printf("%lf %lf %lf",*(theta),*(theta0+4),*(theta0+5));
	std::cout << "T0_1\n" <<T0_1<< std::endl;
	std::cout << "T4_5\n" <<T4_5 << std::endl;
	std::cout << "T5_6\n" <<T5_6 << std::endl;
	std::cout << "T1_4\n" <<T1_4 << std::endl;
	P1_4xzSquare=T1_4(0,3)*T1_4(0,3)+T1_4(2,3)*T1_4(2,3);

	num2=(P1_4xzSquare-(*(a0+2))*(*(a0+2))-(*(a0+3))*(*(a0+3)))/(2*(*(a0+2))*(*(a0+3))) ;
	if(num2>1&&fabs(num2-1)<0.01)
	{num2=1;}
	double theta3_7=+acos(  num2   );
	double theta3_8=-theta3_7;
	
	double resultTheta3[8]={theta3_1,theta3_2, theta3_3, theta3_4,theta3_5,theta3_6,theta3_7,theta3_8};

	double theta2_7=atan2(-T1_4(2,3),-T1_4(0,3))-asin(-(*(a0+3))*sin(theta3_7)/sqrt(P1_4xzSquare));
	theta[1]=theta2_7,theta[2]=theta3_7;
	i=1;
	T1_2<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	i=2;
	T2_3<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	T3_4=(T1_2*T2_3).inverse()*T1_4;
	double theta4_7=atan2(T3_4(1,0),T3_4(0,0));

	double theta2_8=atan2(-T1_4(2,3),-T1_4(0,3))-asin(-(*(a0+3))*sin(theta3_8)/sqrt(P1_4xzSquare));
	theta[1]=theta2_8,theta[2]=theta3_8;
	i=1;
	T1_2<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	i=2;
	T2_3<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	T3_4=(T1_2*T2_3).inverse()*T1_4;
	double theta4_8=atan2(T3_4(1,0),T3_4(0,0));
	
	
	/////////////////////////////////////////////////////共8组解///////////////////////////////////////
	
	double thetaResult1[6]={theta1_1,theta2_1,theta3_1,theta4_1,theta5_1,theta6_1};
	double thetaResult2[6]={theta1_1,theta2_2,theta3_2,theta4_2,theta5_1,theta6_1};
	double thetaResult3[6]={theta1_1,theta2_3,theta3_3,theta4_3,theta5_2,theta6_2};
	double thetaResult4[6]={theta1_1,theta2_4,theta3_4,theta4_4,theta5_2,theta6_2};
	double thetaResult5[6]={theta1_2,theta2_5,theta3_5,theta4_5,theta5_3,theta6_3};
	double thetaResult6[6]={theta1_2,theta2_6,theta3_6,theta4_6,theta5_3,theta6_3};
	double thetaResult7[6]={theta1_2,theta2_7,theta3_7,theta4_7,theta5_4,theta6_4};
	double thetaResult8[6]={theta1_2,theta2_8,theta3_8,theta4_8,theta5_4,theta6_4};

	////////////////////////////////////////////////////////////////////////////////////////////
	
	printf("%lf %lf %lf %lf %lf %lf ",*(thetaResult1),*(thetaResult1+1),*(thetaResult1+2),*(thetaResult1+3),*(thetaResult1+4),*(thetaResult1+5));
	printf("\n ");
	printf("%lf %lf %lf %lf %lf %lf ",*(thetaResult2),*(thetaResult2+1),*(thetaResult2+2),*(thetaResult2+3),*(thetaResult2+4),*(thetaResult2+5));
	printf("\n ");
	printf("%lf %lf %lf %lf %lf %lf ",*(thetaResult3),*(thetaResult3+1),*(thetaResult3+2),*(thetaResult3+3),*(thetaResult3+4),*(thetaResult3+5));
	printf("\n ");
	printf("%lf %lf %lf %lf %lf %lf ",*(thetaResult4),*(thetaResult4+1),*(thetaResult4+2),*(thetaResult4+3),*(thetaResult4+4),*(thetaResult4+5));
	printf("\n ");
	printf("%lf %lf %lf %lf %lf %lf ",*(thetaResult5),*(thetaResult5+1),*(thetaResult5+2),*(thetaResult5+3),*(thetaResult5+4),*(thetaResult5+5));
	printf("\n ");
	printf("%lf %lf %lf %lf %lf %lf ",*(thetaResult6),*(thetaResult6+1),*(thetaResult6+2),*(thetaResult6+3),*(thetaResult6+4),*(thetaResult6+5));
	printf("\n ");
	printf("%lf %lf %lf %lf %lf %lf ",*(thetaResult7),*(thetaResult7+1),*(thetaResult7+2),*(thetaResult7+3),*(thetaResult7+4),*(thetaResult7+5));
	printf("\n ");
	printf("%lf %lf %lf %lf %lf %lf ",*(thetaResult8),*(thetaResult8+1),*(thetaResult8+2),*(thetaResult8+3),*(thetaResult8+4),*(thetaResult8+5));
	printf("\n ");
	return 0;	
}/**/


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~完全正确~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
double* ForwardKinematics( double theta[6])//DH参数
{	const double pi= 3.1415926;
	double a[6]={0,0, -425 ,-392.5 ,0 ,0}; 
	double alpha[6]={0, pi/2 ,0 ,0,pi/2,-pi/2};
	//double theta[6]={0 ,0, 0, 0 ,0, pi/2};
	double d[6]={89.2 , 0,0 ,109.3, 94.75,82.5};
 
	double *a0=a;
	double *alpha0=alpha;
	double *theta0=theta;
	double *d0=d;
		//printf("%f",*(p+2));
	//for(int i=0;i<6;i++)
	int i=0;
	Eigen::Matrix< double , 4 , 4> T1,T2,T3,T4,T5,T6,T1_4;   
	T1<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	 /* */  
	i=1;
	T2<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	i=2;
	T3<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	i=3;
	T4<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	i=4;
	T5<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	i=5;
	T6<<cos(*(theta0+i))					 ,-sin(*(theta0+i))					  , 0			     , *(a0+i)				    	,
		sin(*(theta0+i))*cos(*(alpha0+i))	 ,  cos(*(theta0+i))*cos(*(alpha0+i)) ,-sin(*(alpha0+i)) , -sin(*(alpha0+i))*(*(d0+i))	,
		sin(*(theta0+i))*sin(*(alpha0+i))    ,  cos(*(theta0+i))*sin(*(alpha0+i)) , cos(*(alpha0+i)) , cos(*(alpha0+i))*(*(d0+i))	,
		0									 ,  0								  , 0                , 1						    ;
	
		Eigen::Matrix4d T1_6 = Eigen::Matrix4d::Random();
		T1_6=T1*T2*T3*T4*T5*T6;
		std::cout << "Here is T1_6:\n" <<T1_6 << std::endl;
		double c[12]={T1_6(0,0), T1_6(0,1),T1_6(0,2),T1_6(0,3),
                T1_6(1,0), T1_6(1,1),T1_6(1,2),T1_6(1,3),
                T1_6(2,0), T1_6(2,1), T1_6(2,2),T1_6(2,3)};
		
	double* temp = c;

	return temp;	
}


 
  /*double* inverseKinematics( double roll[12])//DH参数，~~~~标准D_H法，摘自博客
{	const double pi= 3.1415926;
	double a[6]={0.0, -425 ,-392.5 ,0 ,0,0}; 
	double alpha[6]={pi/2 ,0 ,0,pi/2,-pi/2,0};
	
	//double d[6]={0 , -94.5,130.5 ,-120, 93,93};
	double d[6]={89.459 ,0,0,109.15, 94.65,82.3};
	double *a0=a;
	double *alpha0=alpha;
	
	double *d0=d;
		//printf("%f",*(p+2));
	//for(int i=0;i<6;i++)
	 
	 double nx=roll[0],ox=roll[1],ax=roll[2],px=roll[3];
	 double ny=roll[4],oy=roll[5],ay=roll[6],py=roll[7];
	 double nz=roll[8],oz=roll[9],az=roll[10],pz=roll[11];
	 
	Eigen::Matrix< double , 4 , 1> T1,P0_5;
	/////////////////////////////////////////////////////theta1求解///////////////////////////////////////
	double m=(*(d0+5))*ay-py;
	double n=ax*(*(d0+5))-px;
	double theta1_1=atan2(m,n)-atan2((*(d0+3)),+sqrt(m*m+n*n-(*(d0+3))*(*(d0+3))));
	double theta1_2=atan2(m,n)-atan2(   (*(d0+3)),  -sqrt(m*m+n*n-(*(d0+3))*(*(d0+3)))   );
	double theta1_9[2]={theta1_1,theta1_2};
	double*theta1_0=theta1_9;

	/////////////////////////////////////////////////////theta5求解//////////////////////////////////////////
	
	double theta5_1=acos(  ax*sin(theta1_1)-ay*cos(theta1_1)   );
	double theta5_2=-theta5_1;
	double theta5_3=acos(  ax*sin(theta1_2)-ay*cos(theta1_2)   );
	double theta5_4=-theta5_3;
	double theta5_9[4]={theta5_1,theta5_2,theta5_3,theta5_4};
	double*theta5_0=theta5_9;
	/////////////////////////////////////////////////////theta6求解//////////////////////////////////////////
	double m11=nx*sin(theta1_1)-ny*cos(theta1_1);
	double n11=ox*sin(theta1_1)-oy*cos(theta1_1);
	
	double theta6_1=atan2(m11,n11)-atan2(m11/sin(theta5_1),n11/sin(theta5_1));
	double theta6_2=atan2(m11,n11)-atan2(m11/sin(theta5_2),n11/sin(theta5_2));
	/////
	double m12=nx*sin(theta1_2)-ny*cos(theta1_2);
	double n12=ox*sin(theta1_2)-oy*cos(theta1_2);
	
	double theta6_3=atan2(m12,n12)-atan2(m12/sin(theta5_3),n12/sin(theta5_3));
	double theta6_4=atan2(m12,n12)-atan2(m12/sin(theta5_4),n12/sin(theta5_4));
	double theta6_9[4]={theta6_1,theta6_2,theta6_3,theta6_4};
	double*theta6_0=theta6_9;
	/////////////////////////////////////////////////////theta3,2,4求解///////////////////////////////////////
	int i1=0,i2=0;
	double	 m3=(*(d0+4))*(sin(*(theta6_0+i2))*(nx*cos(*(theta1_0+i1))+ny*sin(*(theta1_0+i1)))+cos(*(theta6_0+i2))*(ox*cos(*(theta1_0+i1))+oy*sin(*(theta1_0+i1))))
				-(*(d0+4))*(ax*cos(*(theta1_0+i1))+ay*sin(*(theta1_0+i1)))+px*cos(*(theta1_0+i1))+py*sin(*(theta1_0+i1));
	double n3=pz-(*d0)-az*(*(d0+5))+(*(d0+4))*(oz*cos(*(theta6_0+i2))+nz*sin(*(theta6_0+i2)));
	double theta3_1=+acos(   (m3*m3+n3*n3-(*(a0+1))*(*(a0+1))-(*(a0+2))*(*(a0+2)))/(2*(*(a0+1))*(*(a0+2)))   );
	double theta3_2=-theta3_1;
	
	double s2=(  ((*(a0+2))*cos(theta3_1)+(*(a0+1)))*n3- (*(a0+2))*sin(theta3_1)*m3 )/
					( (*(a0+1))*(*(a0+1)) +(*(a0+2)) *(*(a0+2))+2*(*(a0+1)) *(*(a0+2)) *cos(theta3_1)  );
	double c2=( m3+(*(a0+2))*sin(theta3_1)*s2 )/( (*(a0+2))*cos(theta3_1)+(*(a0+1)) );
	double theta2_1=atan2(s2,c2);
	double theta4_1=atan2(  -sin(theta6_1)*(nx*cos(theta1_1)+ny*sin(theta1_1))-cos(theta6_1)*(ox*cos(theta1_1)+oy*sin(theta1_1)),
							(oz*cos(theta6_1)+nz*sin(theta6_1))  )-theta2_1-theta3_1;

	 s2=(  ((*(a0+2))*cos(theta3_2)+(*(a0+1)))*n3- (*(a0+2))*sin(theta3_2)*m3 )/
					( (*(a0+1))*(*(a0+1)) +(*(a0+2)) *(*(a0+2))+2*(*(a0+1)) *(*(a0+2)) *cos(theta3_2)  );
	 c2=( m3+(*(a0+2))*sin(theta3_2)*s2 )/( (*(a0+2))*cos(theta3_2)+(*(a0+1)) );
	double theta2_2=atan2(s2,c2);
	double theta4_2=atan2(  -sin(theta6_1)*(nx*cos(theta1_1)+ny*sin(theta1_1))-cos(theta6_1)*(ox*cos(theta1_1)+oy*sin(theta1_1)),
							(oz*cos(theta6_1)+nz*sin(theta6_1))  )-theta2_2-theta3_2;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	i1=0,i2=1;
		 m3=(*(d0+4))*(sin(*(theta6_0+i2))*(nx*cos(*(theta1_0+i1))+ny*sin(*(theta1_0+i1)))+cos(*(theta6_0+i2))*(ox*cos(*(theta1_0+i1))+oy*sin(*(theta1_0+i1))))
				-(*(d0+4))*(ax*cos(*(theta1_0+i1))+ay*sin(*(theta1_0+i1)))+px*cos(*(theta1_0+i1))+py*sin(*(theta1_0+i1));
	 n3=pz-(*d0)-az*(*(d0+5))+(*(d0+4))*(oz*cos(*(theta6_0+i2))+nz*sin(*(theta6_0+i2)));
	double theta3_3=+acos(   (m3*m3+n3*n3-(*(a0+1))*(*(a0+1))-(*(a0+2))*(*(a0+2)))/(2*(*(a0+1))*(*(a0+2)))   );
	double theta3_4=-theta3_3;

	 s2=(  ((*(a0+2))*cos(theta3_3)+(*(a0+1)))*n3- (*(a0+2))*sin(theta3_3)*m3 )/
					( (*(a0+1))*(*(a0+1)) +(*(a0+2)) *(*(a0+2))+2*(*(a0+1)) *(*(a0+2)) *cos(theta3_3)  );
	 c2=( m3+(*(a0+2))*sin(theta3_3)*s2 )/( (*(a0+2))*cos(theta3_3)+(*(a0+1)) );
	double theta2_3=atan2(s2,c2);
	double theta4_3=atan2(  -sin(theta6_2)*(nx*cos(theta1_1)+ny*sin(theta1_1))-cos(theta6_2)*(ox*cos(theta1_1)+oy*sin(theta1_1)),
							(oz*cos(theta6_2)+nz*sin(theta6_2))  )-theta2_3-theta3_3;

	s2=(  ((*(a0+2))*cos(theta3_4)+(*(a0+1)))*n3- (*(a0+2))*sin(theta3_4)*m3 )/
					( (*(a0+1))*(*(a0+1)) +(*(a0+2)) *(*(a0+2))+2*(*(a0+1)) *(*(a0+2)) *cos(theta3_4)  );
	 c2=( m3+(*(a0+2))*sin(theta3_4)*s2 )/( (*(a0+2))*cos(theta3_4)+(*(a0+1)) );
	double theta2_4=atan2(s2,c2);
	double theta4_4=atan2(  -sin(theta6_2)*(nx*cos(theta1_1)+ny*sin(theta1_1))-cos(theta6_2)*(ox*cos(theta1_1)+oy*sin(theta1_1)),
							(oz*cos(theta6_2)+nz*sin(theta6_2))  )-theta2_4-theta3_4;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	i1=1,i2=2;
		 m3=(*(d0+4))*(sin(*(theta6_0+i2))*(nx*cos(*(theta1_0+i1))+ny*sin(*(theta1_0+i1)))+cos(*(theta6_0+i2))*(ox*cos(*(theta1_0+i1))+oy*sin(*(theta1_0+i1))))
				-(*(d0+4))*(ax*cos(*(theta1_0+i1))+ay*sin(*(theta1_0+i1)))+px*cos(*(theta1_0+i1))+py*sin(*(theta1_0+i1));
	 n3=pz-(*d0)-az*(*(d0+5))+(*(d0+4))*(oz*cos(*(theta6_0+i2))+nz*sin(*(theta6_0+i2)));
	double theta3_5=+acos(   (m3*m3+n3*n3-(*(a0+1))*(*(a0+1))-(*(a0+2))*(*(a0+2)))/(2*(*(a0+1))*(*(a0+2)))   );
	double theta3_6=-theta3_5;

	s2=(  ((*(a0+2))*cos(theta3_5)+(*(a0+1)))*n3- (*(a0+2))*sin(theta3_5)*m3 )/
					( (*(a0+1))*(*(a0+1)) +(*(a0+2)) *(*(a0+2))+2*(*(a0+1)) *(*(a0+2)) *cos(theta3_5)  );
	 c2=( m3+(*(a0+2))*sin(theta3_5)*s2 )/( (*(a0+2))*cos(theta3_5)+(*(a0+1)) );
	double theta2_5=atan2(s2,c2);
	double theta4_5=atan2(  -sin(theta6_3)*(nx*cos(theta1_2)+ny*sin(theta1_2))-cos(theta6_3)*(ox*cos(theta1_2)+oy*sin(theta1_2)),
							(oz*cos(theta6_3)+nz*sin(theta6_3))  )-theta2_5-theta3_5;

	s2=(  ((*(a0+2))*cos(theta3_6)+(*(a0+1)))*n3- (*(a0+2))*sin(theta3_6)*m3 )/
					( (*(a0+1))*(*(a0+1)) +(*(a0+2)) *(*(a0+2))+2*(*(a0+1)) *(*(a0+2)) *cos(theta3_6)  );
	 c2=( m3+(*(a0+2))*sin(theta3_6)*s2 )/( (*(a0+2))*cos(theta3_6)+(*(a0+1)) );
	double theta2_6=atan2(s2,c2);
	double theta4_6=atan2(  -sin(theta6_3)*(nx*cos(theta1_2)+ny*sin(theta1_2))-cos(theta6_3)*(ox*cos(theta1_2)+oy*sin(theta1_2)),
							(oz*cos(theta6_3)+nz*sin(theta6_3))  )-theta2_6-theta3_6;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	i1=1,i2=3;
		 m3=(*(d0+4))*(sin(*(theta6_0+i2))*(nx*cos(*(theta1_0+i1))+ny*sin(*(theta1_0+i1)))+cos(*(theta6_0+i2))*(ox*cos(*(theta1_0+i1))+oy*sin(*(theta1_0+i1))))
				-(*(d0+4))*(ax*cos(*(theta1_0+i1))+ay*sin(*(theta1_0+i1)))+px*cos(*(theta1_0+i1))+py*sin(*(theta1_0+i1));
	 n3=pz-(*d0)-az*(*(d0+5))+(*(d0+4))*(oz*cos(*(theta6_0+i2))+nz*sin(*(theta6_0+i2)));
	double theta3_7=+acos(   (m3*m3+n3*n3-(*(a0+1))*(*(a0+1))-(*(a0+2))*(*(a0+2)))/(2*(*(a0+1))*(*(a0+2)))   );
	double theta3_8=-theta3_7;
	double theta3_9[8]={theta3_1,theta3_2,theta3_3,theta3_4,theta3_5,theta3_6,theta3_7,theta3_8};
	double*theta3_0=theta3_9;

	s2=(  ((*(a0+2))*cos(theta3_7)+(*(a0+1)))*n3- (*(a0+2))*sin(theta3_7)*m3 )/
					( (*(a0+1))*(*(a0+1)) +(*(a0+2)) *(*(a0+2))+2*(*(a0+1)) *(*(a0+2)) *cos(theta3_7)  );
	 c2=( m3+(*(a0+2))*sin(theta3_7)*s2 )/( (*(a0+2))*cos(theta3_7)+(*(a0+1)) );
	double theta2_7=atan2(s2,c2);
	double theta4_7=atan2(  -sin(theta6_4)*(nx*cos(theta1_2)+ny*sin(theta1_2))-cos(theta6_4)*(ox*cos(theta1_2)+oy*sin(theta1_2)),
							(oz*cos(theta6_4)+nz*sin(theta6_4))  )-theta2_7-theta3_7;

	s2=(  ((*(a0+2))*cos(theta3_8)+(*(a0+1)))*n3- (*(a0+2))*sin(theta3_8)*m3 )/
					( (*(a0+1))*(*(a0+1)) +(*(a0+2)) *(*(a0+2))+2*(*(a0+1)) *(*(a0+2)) *cos(theta3_8)  );
	 c2=( m3+(*(a0+2))*sin(theta3_8)*s2 )/( (*(a0+2))*cos(theta3_8)+(*(a0+1)) );
	double theta2_8=atan2(s2,c2);
	double theta4_8=atan2(  -sin(theta6_4)*(nx*cos(theta1_2)+ny*sin(theta1_2))-cos(theta6_4)*(ox*cos(theta1_2)+oy*sin(theta1_2)),
							(oz*cos(theta6_4)+nz*sin(theta6_4))  )-theta2_8-theta3_8;
	
	double thetaResult1[6]={theta1_1,theta2_1,theta3_1,theta4_1,theta5_1,theta6_1};
	double thetaResult2[6]={theta1_1,theta2_2,theta3_2,theta4_2,theta5_1,theta6_1};
	double thetaResult3[6]={theta1_1,theta2_3,theta3_3,theta4_3,theta5_2,theta6_2};
	double thetaResult4[6]={theta1_1,theta2_4,theta3_4,theta4_4,theta5_2,theta6_2};
	double thetaResult5[6]={theta1_2,theta2_5,theta3_5,theta4_5,theta5_3,theta6_3};
	double thetaResult6[6]={theta1_2,theta2_6,theta3_6,theta4_6,theta5_3,theta6_3};
	double thetaResult7[6]={theta1_2,theta2_7,theta3_7,theta4_7,theta5_4,theta6_4};
	double thetaResult8[6]={theta1_2,theta2_8,theta3_8,theta4_8,theta5_4,theta6_4};

	for(int i=0;i<=5;i++)
	{	printf("%lf  ",thetaResult1[i]);
	
	}
	printf("\n");
	for(int i=0;i<=5;i++)
	{	printf("%lf  ",thetaResult2[i]);
	
	}
	printf("\n");
	for(int i=0;i<=5;i++)
	{	printf("%lf  ",thetaResult3[i]);
	
	}
	printf("\n");
	for(int i=0;i<=5;i++)
	{	printf("%lf  ",thetaResult4[i]);
	
	}
	printf("\n");
	for(int i=0;i<=5;i++)
	{	printf("%lf  ",thetaResult5[i]);
	
	}
	printf("\n");
	for(int i=0;i<=5;i++)
	{	printf("%lf  ",thetaResult6[i]);
	
	}
	printf("\n");
	for(int i=0;i<=5;i++)
	{	printf("%lf  ",thetaResult7[i]);
	
	}
	printf("\n");
	for(int i=0;i<=5;i++)
	{	printf("%lf  ",thetaResult8[i]);
	
	}
	printf("\n");
	
	


	return 0;	
}*/