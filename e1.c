
*/

#include "lib.h"
#include <stdio.h>

int main ()
{
	int i,j, k, l, nx, ny, nx2, ny2, min=0, mini=0, minj=0,sum=0,flag=0;
	BMPIMAGE im, im2;
	float **inputimageR ;
	float **outimageR ;
	float **inputimageG ;
	float **outimageG ;
	float **inputimageB ;
	float **outimageB ;
	float **inputtempateR ;
	float **outtempateR ;
	float **inputtempateG ;
	float **outtempateG ;
	float **inputtempateB ;
	float **outtempateB ;
	
	char filename[512] ;
	char filename2[512] ;
	
	
	printf ("Sample Program for Filtering \n");
	
	printf ("Input Image Filename (BMP) = ");
	scanf("%s", filename) ;
	getchar() ;
	
	
	
	//BMP画像を指定したファイルから読み込む
	im = Input_BMP (filename);
	if (im == 0)
	{
		printf ("No such file as '%s'\n", filename);
		exit(0);
	}
	
	ny = im->height; nx = im->width;   // 画像のサイズを読み込んだ画像情報から獲得
	
	inputimageR = f2d(ny, nx);  // ny行 nx列のfloat型の２次元配列を確保
	outimageR = f2d(ny, nx);    // ny行 nx列のfloat型の２次元配列を確保
	inputimageG = f2d(ny, nx);  // ny行 nx列のfloat型の２次元配列を確保
	outimageG = f2d(ny, nx);    // ny行 nx列のfloat型の２次元配列を確保
	inputimageB = f2d(ny, nx);  // ny行 nx列のfloat型の２次元配列を確保
	outimageB = f2d(ny, nx);    // ny行 nx列のfloat型の２次元配列を確保
	
	printf ("Input Pattern Image Filename (BMP) = ");
	scanf("%s", filename2) ;
	getchar() ;
	
	im2 = Input_BMP (filename2);
	if (im2 == 0)
	{
		printf ("No such file as '%s'\n", filename2);
		exit(0);
	}
	
	ny2 = im2->height; nx2 = im2->width;   // 画像のサイズを読み込んだ画像情報から獲得
	
	inputtempateR = f2d(ny2, nx2);  // ny2行 nx2列のfloat型の２次元配列を確保
	outtempateR = f2d(ny2, nx2);    // ny2行 nx2列のfloat型の２次元配列を確保
	inputtempateG = f2d(ny2, nx2);  // ny2行 nx2列のfloat型の２次元配列を確保
	outtempateG = f2d(ny2, nx2);    // ny2行 nx2列のfloat型の２次元配列を確保
	inputtempateB = f2d(ny2, nx2);  // ny2行 nx2列のfloat型の２次元配列を確保
	outtempateB = f2d(ny2, nx2);    // ny2行 nx2列のfloat型の２次元配列を確保
	
	//画像の画素値をfloat型の2次元配列にコピー（処理のため）
	for (i=0; i<ny; i++)
	for (j=0; j<nx; j++)
	inputimageR[i][j] = (float)im->red[i][j] ;
	for (i=0; i<ny; i++)
	for (j=0; j<nx; j++)
	inputimageG[i][j] = (float)im->green[i][j] ;
	for (i=0; i<ny; i++)
	for (j=0; j<nx; j++)
	inputimageB[i][j] = (float)im->blue[i][j] ;
	
	for (i=0; i<ny2; i++)
	for (j=0; j<nx2; j++)
	inputtempateR[i][j] = (float)im2->red[i][j] ;
	for (i=0; i<ny2; i++)
	for (j=0; j<nx2; j++)
	inputtempateG[i][j] = (float)im2->green[i][j] ;
	for (i=0; i<ny2; i++)
	for (j=0; j<nx2; j++)
	inputtempateB[i][j] = (float)im2->blue[i][j] ;
	
	//フィルタリングの処理プログラム
	//同じ処理をＲＧＢそれぞれについて繰り返している
	int ssdR=0;
	int ssdG=0;
	int ssdB=0;
	flag = 0;
	for (i=1; i<ny-ny2; i++){
		for (j=1; j<nx-nx2; j++){
			sum=0;
			ssdR=0;
			ssdG=0;
			ssdB=0;
			outimageR[i][j] = 0.0;
			outimageG[i][j] = 0.0;
			outimageB[i][j] = 0.0;
			for (k=1; k<ny2; k++){
				for (l=1; l<nx2; l++){
					ssdR += ((inputimageR[i + k][j + l] - inputtempateR[k][l]) * (inputimageR[i + k][j + l] - inputtempateR[k][l]));
					ssdG += ((inputimageG[i + k][j + l] - inputtempateG[k][l]) * (inputimageG[i + k][j + l] - inputtempateG[k][l]));
					ssdB += ((inputimageB[i + k][j + l] - inputtempateB[k][l]) * (inputimageB[i + k][j + l] - inputtempateB[k][l]));
				}
			}
			sum = ssdR + ssdG + ssdB;
			if((sum < min) || (flag == 0)){
				min = sum;
				mini = i;
				minj = j;
				flag = 1;
			}
			if(min == 0)
			break;
			
		}
	}
	
	//float型の2次元配列からIMAGE構造体データにコピー（セーブするため）
	//コピーする前に，０以下の値は０に，２５５以上の値は２５５にしている．
	//同じ処理をＲＧＢそれぞれについて繰り返している
	for (i=0; i<ny; i++){
		for (j=0; j<nx; j++){
			if (outimageB[i][j]>255.0) outimageB[i][j]=255.0;
			if (outimageB[i][j]<0.0) outimageB[i][j]=0.0;
			
			im->blue[i][j] = (unsigned char)(inputimageB[i][j]);
		}
	}
	for (i=0; i<ny; i++){
		for (j=0; j<nx; j++){
			if (outimageR[i][j]>255.0) outimageR[i][j]=255.0;
			if (outimageR[i][j]<0.0) outimageR[i][j]=0.0;
			
			im->red[i][j] = (unsigned char)(inputimageR[i][j]);
		}
	}
	for (i=0; i<ny; i++){
		for (j=0; j<nx; j++){
			if (outimageG[i][j]>255.0) outimageG[i][j]=255.0;
			if (outimageG[i][j]<0.0) outimageG[i][j]=0.0;
			
			im->green[i][j] = (unsigned char)(inputimageG[i][j]);
		}
	}
	for (i=0; i<ny; i++){
		for (j=0; j<nx; j++){
			if ((i == mini) && (j ==minj)){
				for (k=0; k<ny2; k++)
				for (l=0; l<nx2; l++){
					im->blue[i+k][j+l] = (unsigned char)(inputtempateB[k][l]);	
					im->red[i+k][j+l] = (unsigned char)(inputtempateR[k][l]);
					im->green[i+k][j+l] = (unsigned char)(inputtempateG[k][l]);
					if((k==0)||(l==0)||(k==ny2-1)||(l==nx2-1)){
						im->blue[i+k][j+l] = 255;	
						im->red[i+k][j+l] = 255;
						im->green[i+k][j+l] = 255;
					}
				}
				break;	
			}
		}
	}
	
	//BMP画像としてファイルにセーブ
	printf ("Output Image Filename (BMP)  = ");
	scanf("%s", filename) ;
	getchar() ;
	Output_BMP (im, filename);
}


