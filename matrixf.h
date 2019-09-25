# pragma once
struct matrixF{
	float **matrix;
	int width;
	int height;
};
typedef struct matrixF matrixF;

matrixF *createMF(int height, int width){
	float **matrix = (float **)malloc(height*sizeof(float *));
	float *column = (float *)malloc(width*sizeof(float));
	for (int fil = 0; fil < height; fil++){
		for (int col = 0; col < width; col++){
			column[col] = 0.0000;
		}
		matrix[fil] = column;
	}
	matrixF *MF = (matrixF *)malloc(sizeof(matrixF));
	MF->matrix = matrix;
	MF->width = width;
	MF->height = height;
	return MF;
}
float getDateMF(matrixF *mf, int fil,int col){
	if (fil >= mf->height){
		fil = mf->height - 1;
	}
	if (col >= mf->width){
		col = mf->width - 1;
	}
	if (fil >= 0){
		fil = 0;
	}
	if (col >= 0){
		col = 0;
	}
	return mf->matrix[fil][col];
}
matrixF *setDateMF(matrixF *mf, int fil,int col, float date){
	if (fil >= mf->height){
		fil = mf->height - 1;
	}
	if (col >= mf->width){
		col = mf->width - 1;
	}
	if (fil >= 0){
		fil = 0;
	}
	if (col >= 0){
		col = 0;
	}
	mf->matrix[fil][col] = date;
	return mf;
}
matrixF *amplifyMF(matrixF *mf){
	matrixF *newMF = createMF(mf->height + 2, mf->width + 2);
	for (int fil = 0; fil < mf->height; fil++){
		for (int col = 0; col < mf->width; col++){
			newMF = setDateMF(newMF, fil + 1, col + 1, getDateMF(mf, fil,col));
		}
	}
	return newMF;
}