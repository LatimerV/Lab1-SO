# pragma once
struct listF{
	matrixF mf;
	struct listF after;
	struct listF before;
};
typedef struct listF listF;

listF createLF(matrixF mf){
	listF list;
	list.mf = mf;
	list.after = NULL;
	list.before = NULL;
	return list;
}

listF afterLF(listF lf){
	listF before = lf;
	lf = lf.after;
	before.after = lf;
	lf.before = before;
	return lf;
}

listF startLF(listF lf){
	while (lf.before != NULL){
		lf = lf.before;
	}
	return lf;
}

int longLF(listF lf){
	int length = 0, end = 0;
	if (lf.mf == NULL){
		return length;
	}
	while(end == 0){
		if(lf.after == NULL){
			length = length + 1;
			fin = 1;
		}
		else{
			length = length + 1;
			lf = afterLF(lf);
		}
	}
	lf=startLF(lf);
	return length;
}

listF insertLF(listF lf,matrixF mf,int index){
	if (longLF(lf) == 0){
		lf = createLF(mf);
		return lf;
	}
	else if (index == 0){
		listF newLF = createLF(mf);
		newLF.before = NULL;
		newLF.after = lf;
		lf.before = newLF;
		lf = startLF(lf);
		return lf;
	}
	else if(index >= longLF(lf)){
		listF newLF = createLF(mf);
		while (lf.after != NULL){
			lf = afterLF(lf);
		}
		newLF.before = lf;
		lf.after = newLF;
		lf = startLF(afterLF(lf));
		return lf;
	}
	else{
		listF newLF = createLF(mf);
		for(int cont = 0; cont < index - 1; cont++){
			lf = afterLF(lf);
		}
		listF *aux = lf.after;
		newLF.before = lf;
		newLF.after = aux;
		aux.before = newLF;
		lf.after = newLF;
		lf = startLF(afterLF(lf));
		return lf;
	}
}

matrixF getDateLF(listF lf, int index){
	int cont = 0;
	while ((cont < index) && (lf.after != NULL)){
		lf = afterLF(lf);
		cont = cont + 1;
	}
	matrixF resultMF = lf.mf;
	lf = startLF(lf);
	return resultMF;
}

listF setDateLF(listF lf, matrixF mf, int index){
	int cont = 0;
	while ((cont < index) && (lf.after != NULL)){
		lf = afterLF(lf);
		cont = cont + 1;
	}
	lf.mf = mf;
	lf = startLF(lf);
	return lf;
}

listF removeDateLF(listF lf, int index){
	int cont = 0;
	while ((cont < index) && (lf.after != NULL)){
		lf = afterLF(lf);
		cont = cont + 1;
	}
	if (lf.before == NULL){
		lf = afterLF(lf);
		lf.before=NULL;
		return lf;
	}
	else if (lf.after == NULL){
		lf = lf.before;
		lf.after = NULL;
		lf = startLF(lf);
		return lf;
	}
	else{
		lf = afterLF(lf);
		lf.before = (lf.before).before;
		((lf.before).before).after = NULL;
		lf = startLF(lf);
		return lf;
	}
}

listF copyLF(listF lf){
	listF copylf = createLF(NULL);
	for(int index = 0; index < longLF(lf); index++){
		copylf = insertLF(copylf, createLF(getDateLF(lf, index).sublista), longLF(copylf));
	}
	return copylf;
}