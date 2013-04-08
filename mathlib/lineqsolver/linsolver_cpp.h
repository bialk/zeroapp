#ifndef linsolver_cpp_h
#define linsolver_cpp_h



class indx{
public:
  int i,j;
  indx(): i(0),j(0){}
  indx(int ii, int jj):i(ii),j(jj){}
  bool operator < (const indx &p) const{
    return i<p.i || (i==p.i && j<p.j);
  }
};


class SparceSolver{
public:

  const char *aux_file_prefix;   // file name for aux files 
  char aux_delete;               // 0/1 - (delete/keep) aux files after processing
  int nice;                      // prioryty of the task (19-0);

  int sizem;

  class KMtrx: public std::map<indx, float>{
  public:
    float& operator()(int i, int j){
      iterator it =  insert(value_type(indx(i,j),0)).first;
      return it->second;
    }
  } mtrk;

  class BMtrx: public std::map<int, float>{
  public:
    float& operator()(int i){
      iterator it =  insert(value_type(i,0)).first;
      return it->second;
    }
  } mtrb;

  SparceSolver():aux_delete(1),nice(19),aux_file_prefix("ssolver"){}

  int init(){
    mtrk.clear();
    mtrb.clear();
  }

  int solve(){
    int ix,iy;
    //auxilary files
    char data_mtrx_A[200]; sprintf(data_mtrx_A,"%s_mtrxa.txt",aux_file_prefix);
    char data_mtrx_B[200]; sprintf(data_mtrx_B,"%s_mtrxb.txt",aux_file_prefix);
    char data_mtrx_X[200]; sprintf(data_mtrx_X,"%s_mtrxx.txt",aux_file_prefix);


    FILE* f=fopen(data_mtrx_A,"w");
    if(!f){ printf("can't create auxilary file '%s'\n",data_mtrx_A); return 0; }    KMtrx::iterator it;
    fprintf(f,"%i %i %i\n", sizem, sizem, mtrk.size());
    for(it=mtrk.begin();it!=mtrk.end();it++){
      fprintf(f,"%i %i %e\n",it->first.i,it->first.j,it->second);
    }
    fclose(f);


    f=fopen(data_mtrx_B,"w");
    if(!f){ printf("can't create auxilary file '%s'\n",data_mtrx_B); return 0; }
    BMtrx::iterator itb;
    
    
    int nozero=0;
    for(itb=mtrb.begin();itb!=mtrb.end();itb++){
      if(itb->second!=0) nozero++;
    }
    fprintf(f,"%i 1\n", nozero);
    for(itb=mtrb.begin();itb!=mtrb.end();itb++){
      if(itb->second!=0) 
	fprintf(f,"%i %e\n",itb->first,itb->second);
    }
    
    /* -- old code for right side
    fprintf(f,"%i 1\n", mtrb.size());
    for(itb=mtrb.begin();itb!=mtrb.end();itb++){      
      fprintf(f,"%i %e\n",itb->first,itb->second);
    }    
    fprintf(f,"%i %e\n",itb->first,itb->second);
    */
    fclose(f);

    init();
    printf("\nRunning solver of linear equations...\n"
           "=====================================\n");
    char cmdstr[200];
    sprintf(cmdstr, "nice -0 solv_lineq 0 %s_test.log 1 2 1 %s %s 6666666  %s",
            aux_file_prefix, data_mtrx_A , data_mtrx_B, data_mtrx_X);
    printf("%s\n",cmdstr);
    system(cmdstr);

    f=fopen(data_mtrx_X,"r");
    for(ix=0;ix<sizem;ix++){
      float a;
      fscanf(f, "%e\n", &a);
      vectorx(ix,a);
    }
    fclose(f);

    if(aux_delete){
      printf("\nDeleting auxilary dataset...\n");
      sprintf(cmdstr, "rm -f %s*", aux_file_prefix);
      printf("%s\n",cmdstr);
      system(cmdstr);
    }
  }

  void test(){
    sizem=50000;
    aux_file_prefix="../data_aux/solv";
    aux_delete=0;
    int i;
    for(i=0;i<sizem;i++){
      if(i>0) mtrk(i,i-1)=-1;
      mtrk(i,i)=2;
      if(i<sizem-1) mtrk(i,i+1)=-1;
    }
    mtrb(sizem-1)=sizem+1;
    solve();
  }
  virtual void vectorx(int i, float a){
    // put here the code for accepting solution
  }
};

#endif
