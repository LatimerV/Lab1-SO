# pragma once
struct matrixF{
	float digit;
	struct matrixF *up;
	struct matrixF *down;
	struct matrixF *left;
	struct matrixF *right;
};
typedef struct matrixF matrixF;

matrixF *rightMF(matrixF *mf){
	matrixF *leftmf = mf;
	mf = mf->right;
	leftmf->right = mf;
	mf->left = leftmf;
	return mf;
}

matrixF *downMF(matrixF *mf){
	matrixF *upmf = mf;
	mf = mf->down;
	upmf->down = mf;
	mf->up = upmf;
	return mf;
}

matrixF *startLeftMF(matrixF *mf){
	while (mf->left != NULL){
		mf = mf->left;
	}
	return mf;
}

matrixF *startUpMF(matrixF *mf){
	while (mf->up != NULL){
		mf = mf->up;
	}
	return mf;
}

matrixF *createMF(int height, int width){
	matrixF *mf = (matrixF *)malloc(sizeof(matrixF));
	mf->digit = 0.0000;
	mf->up = NULL;
	mf->down = NULL;
	mf->left = NULL;
	mf->right = NULL;
	for (int cont = 0; cont < width - 1; cont++){
		matrixF *auxmf = (matrixF *)malloc(sizeof(matrixF));
		auxmf->up = NULL;
		auxmf->down = NULL;
		auxmf->left = NULL;
		auxmf->right = NULL;
		auxmf->digit = 0.0000;
		mf->right = auxmf;
		auxmf->left = mf;
		mf = rightMF(mf);
	}
	mf = startLeftMF(mf);
	for (int fil = 0; fil < height - 1; fil++){
		matrixF *newmf = (matrixF *)malloc(sizeof(matrixF));
		newmf->up = NULL;
		newmf->down = NULL;
		newmf->left = NULL;
		newmf->right = NULL;
		newmf->digit = 0.0000;
		for (int col = 0; col < width - 1; col++){
			matrixF *auxmf = (matrixF *)malloc(sizeof(matrixF));
			auxmf->up = NULL;
			auxmf->down = NULL;
			auxmf->left = NULL;
			auxmf->right = NULL;
			auxmf->digit = 0.0000;
			newmf->right = auxmf;
			auxmf->left = newmf;
			newmf = rightMF(newmf);
		}
		newmf = startLeftMF(newmf);
		mf->down = newmf;
		newmf->up = mf;
		mf = downMF(mf);
	}
	mf = startLeftMF(mf);
	mf = startUpMF(mf);
	return mf;
}

matrixF *setDateMF(matrixF *mf, int fil,int col, float date){
	if (fil < 0){
		fil = 0;
	}
	if (col < 0){
		col = 0;
	}
	int x = 0, y = 0;
	while ((y < fil) && (mf->down != NULL)){
		mf = downMF(mf);
		y = y + 1;
	}
	while ((x < col) && (mf->right != NULL)){
		mf = rightMF(mf);
		x = x + 1;
	}
	mf->digit = date;
	mf = startLeftMF(mf);
	mf = startUpMF(mf);
	return mf;
}

float getDateMF(matrixF *mf, int fil,int col){
	if (fil < 0){
		fil = 0;
	}
	if (col < 0){
		col = 0;
	}
	int x = 0, y = 0;
	while ((y < fil) && (mf->down != NULL)){
		mf = downMF(mf);
		y = y + 1;
	}
	while ((x < col) && (mf->right != NULL)){
		mf = rightMF(mf);
		x = x + 1;
	}
	float date = mf->digit;
	mf = startLeftMF(mf);
	mf = startUpMF(mf);
	return date;
}

/*struct matrixF{
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
	if (fil >= (mf->height)){
		fil = (mf->height) - 1;
	}
	if (col >= (mf->width)){
		col = (mf->width) - 1;
	}
	if (fil < 0){
		fil = 0;
	}
	if (col < 0){
		col = 0;
	}
	return mf->matrix[fil][col];
}
matrixF *setDateMF(matrixF *mf, int fil,int col, float date){
	if (fil >= (mf->height)){
		fil = (mf->height) - 1;
	}
	if (col >= (mf->width)){
		col = (mf->width) - 1;
	}
	if (fil < 0){
		fil = 0;
	}
	if (col < 0){
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
}*/