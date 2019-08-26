#include "ankan_after_reach.hpp"

//チェックメソッド
bool Ankan_After_Reach::check(Hai_Array& tehai, int tumohai) {
	tehai[tumohai]++; // http://cmj3.web.fc2.com/から持ってきたアルゴリズムだが、元のアルゴリズムはtehaiにtumohaiが入っていることが前提のため。

	//字牌なら可
	if(tumohai>30)return true;

	//送りカン禁止
	if(tehai[tumohai]!=4) return false;

	tumo_syurui=tumohai%10;
	
	//周りに牌が無いなら可
	if(tumo_syurui==1){
		if(!tehai[tumohai+1]&!tehai[tumohai+2])
			return true;
	}
	else if(tumo_syurui==2){
		if(!tehai[tumohai-1]&!tehai[tumohai+1]&!tehai[tumohai+2])
			return true;
	}
	else if(tumo_syurui>=3 &&tumo_syurui<=7){
		if(!tehai[tumohai-1]&!tehai[tumohai+1]&!tehai[tumohai-2]&!tehai[tumohai+2])
			return true;
	}
	else if(tumo_syurui==8){
		if(!tehai[tumohai+1]&!tehai[tumohai-1]&!tehai[tumohai-2])
			return true;
	}
	else if(tumo_syurui==9){
		if(!tehai[tumohai-1]&!tehai[tumohai-2])
			return true;
	}

	//構成不変なら可
	//初期化
	int i,j=0,work=0,after=0,befor=0;
	
	//作業用にコピー
    for(i=0;i<38;i++){
		kouseichk[j++]=tehai[i];
	}
	
    kouseichk[tumohai]--;

	//カンしないでの構成
	taatu=jyantou=0;
	chk_kousei(kouseichk,1,&work,&befor);

    work=1+tumohai;
    kouseichk[tumohai]-=3;

	//カンしての構成
	taatu=jyantou=0;
	chk_kousei(kouseichk,1,&work,&after);

	//同じなら
	if(after==befor){
		if(riichi_tyuuren_kinsi(tumohai))//九連リーチ後のアンカン禁止
			return true;
	}
	return false;
}

//九連リーチ後のアンカン禁止 trueでカンできない
bool Ankan_After_Reach::riichi_tyuuren_kinsi(int tumohai)
{
	//カン牌/10が1でも9でも無い
	if(tumo_syurui!=1 && tumo_syurui!=9)
		return true;

	//牌姿から、ありえないなら
	int j=0,k=0,l=0;
	for(int i=1;i<10;i++){
		if(kouseichk[i]==0) {j++;k=i;}
		if(kouseichk[i]==2) {l=i;}
		if(i==9&&j==0) return true;		
		else if(j>=2) return true;
	}

	//牌姿が九連でないなら
	kouseichk[k]++;
	kouseichk[l]--;
	kouseichk[10]=-1;
	int chk1[11]={0,3,1,1,1,1,1,1,1,3,-1};
	if(memcmp(kouseichk,chk1,sizeof(chk1))==0)
		return false;

	return true;
}

//構成不変なら可
void Ankan_After_Reach::chk_kousei(int kouseichk[],int i,int *work,int *arg)
{
	for(;!kouseichk[i];i++);
    if(i>=38){
		for(int j=1;j<38;j++)
			if(kouseichk[j]!=0) return;
		*arg+=*work;
		return;
	}
	//コーツ
	if(kouseichk[i]>=3)
	{
		*work+=1+i;
		kouseichk[i]-=3;
		chk_kousei(kouseichk,i,work,arg);
		kouseichk[i]+=3;
		*work-=1+i;
	}
	//シュンツ
	if(kouseichk[i+1]&&kouseichk[i+2]&&i<30&&i%10<=7)
	{
		*work+=2+i;
		kouseichk[i]--,kouseichk[i+1]--,kouseichk[i+2]--;
		chk_kousei(kouseichk,i,work,arg);
		kouseichk[i]++,kouseichk[i+1]++,kouseichk[i+2]++;
		*work-=2+i;
	}

	//テンパイしているため頭orターツは一つずつしかとれない
	//トイツ
	if(!jyantou&&kouseichk[i]>=2)
	{
		jyantou=1;
		*work+=3+i;
		kouseichk[i]-=2;
		chk_kousei(kouseichk,i,work,arg);
		kouseichk[i]+=2;
		*work-=3+i;
		jyantou=0;
	}
	//ペンチャンorリャンメン
	if(!taatu&&kouseichk[i+1]&&i<30&&i%10<=8)
	{
		taatu=1;
		*work+=4+i;
		kouseichk[i]--,kouseichk[i+1]--;
		chk_kousei(kouseichk,i,work,arg);
		kouseichk[i]++,kouseichk[i+1]++;
		*work-=4+i;
		taatu=0;
	}	             
	//カンチャン
	if(!taatu&&kouseichk[i+2]&&i<30&&i%10<=7)
	{
		taatu=1;
		*work+=5+i;
		kouseichk[i]--,kouseichk[i+2]--;
		chk_kousei(kouseichk,i,work,arg);
		kouseichk[i]++,kouseichk[i+2]++;
		*work-=5+i;
		taatu=0;
	}
	chk_kousei(kouseichk,i+1,work,arg);
}