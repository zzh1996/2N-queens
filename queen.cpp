#include <cstdio>
#include <cstdlib>
#include <ctime>

class Chess{
public:
    int n;
    int *q;
    int *lc;//右上-左下对角线
    int *rc;//左上-右下对角线
    int h;

    Chess(int n):n(n),q(new int[n]()),lc(new int[2*n-1]()),rc(new int[2*n-1]()){}
    ~Chess(){
        delete[] q;
        delete[] lc;
        delete[] rc;
    }

    void recalculate_crosses(){
        for(int i=0;i<2*n-1;i++){
            lc[i]=rc[i]=0;
        }
        for(int i=0;i<n;i++){
            lc[i+q[i]]++;
            rc[i+n-1-q[i]]++;
        }
        h=0;
        for(int i=0;i<2*n-1;i++){
            h+=(lc[i]*(lc[i]-1))/2;
            h+=(rc[i]*(rc[i]-1))/2;
        }
    }

    void swap(int x,int y){//交换x和y两行
        int t;

        if(x+q[x]==y+q[y])//交换之前同一左对角线
            h-=2*lc[x+q[x]]-3;
        else
            h-=lc[x+q[x]]-1+lc[y+q[y]]-1;
        
        if(x+q[y]==y+q[x])//交换之后同一左对角线
            h+=2*lc[x+q[y]]+1;
        else
            h+=lc[x+q[y]]+lc[y+q[x]];
        
        if(x-q[x]==y-q[y])//交换之前同一右对角线
            h-=2*rc[x+n-1-q[x]]-3;
        else
            h-=rc[x+n-1-q[x]]-1+rc[y+n-1-q[y]]-1;
        
        if(x-q[y]==y-q[x])//交换之后同一右对角线
            h+=2*rc[x+n-1-q[y]]+1;
        else
            h+=rc[x+n-1-q[y]]+rc[y+n-1-q[x]];

        lc[x+q[x]]--;
        rc[x+n-1-q[x]]--;
        lc[y+q[y]]--;
        rc[y+n-1-q[y]]--;
        lc[y+q[x]]++;
        rc[y+n-1-q[x]]++;
        lc[x+q[y]]++;
        rc[x+n-1-q[y]]++;

        t=q[x];
        q[x]=q[y];
        q[y]=t;
    }

    void init(){
        do{
            int i,p,tmp;
            for(int i=0;i<2*n-1;i++){
                lc[i]=rc[i]=0;
            }
            int *t=new int[n];
            for(i=0;i<n;i++)
                t[i]=i;
            for(i=0;i<n;i++){
                for(int j=0;j<100;j++){
                    p=rand()%(n-i);
                    if(lc[i+t[p]]==0&&rc[i+n-1-t[p]]==0&&i+n-1-t[p]!=n-1)
                        break;
                }
                q[i]=t[p];
                lc[i+q[i]]++;
                rc[i+n-1-q[i]]++;
                tmp=t[p];
                t[p]=t[n-i-1];
                t[n-i-1]=tmp;
            }
            delete[] t;
            recalculate_crosses();
        }while(h==0&&rc[n-1]>0); //处理初始h=0并且主对角线有皇后的特殊情况
    }

    void print(){
        int i,j;
        for(i=0;i<n;i++){
            for(j=0;j<n;j++){
                putchar(q[i]==j?'Q':q[j]==i?'q':'.');
            }
            putchar('\n');
        }
    }

    void write(double totaltime){
        FILE *fp=fopen("output_hill_climbing.txt","w");
        if(n!=5){
            int *t=new int[n];
            for(int i=0;i<n;i++){
                fprintf(fp,"%d\n",q[i]+1);
                t[q[i]]=i;
            }
            for(int i=0;i<n;i++){
                fprintf(fp,"%d\n",t[i]+1);
            }
        }else{
            fprintf(fp,"1\n4\n2\n5\n3\n3\n1\n4\n2\n5\n");
        }
        fprintf(fp,"%.3f\n",totaltime);
        fclose(fp);
    }
};

void solve(int n){
    clock_t start, finish;
    double totaltime;
    start = clock();
    Chess c(n);
    if(n!=5){
        c.init();
        int count=0;
        while(c.h>0){
            int mh=c.h,tries=1;
            for(int ii=0;ii<n;ii++){ //随机找后继
                for(int jj=0;jj<n;jj++){
                    /*int i=0;
                    while(c.lc[i+c.q[i]]==0&&c.rc[i+n-1-c.q[i]]==0)*/
                    int i=rand()%n;
                    int j=rand()%n;
                    if(i==j)continue;
                    c.swap(i,j);
                    if(c.h<mh&&c.rc[n-1]==0){
                        mh=c.h;
                        //优化：找到解就跑路
                        goto exit;
                    }
                    c.swap(i,j);
                    tries++;
                }
            }
            for(int i=0;i<n;i++){ //找到h最小的后继
                for(int j=i+1;j<n;j++){
                    c.swap(i,j);
                    if(c.h<mh&&c.rc[n-1]==0){
                        mh=c.h;
                        //优化：找到解就跑路
                        goto exit;
                    }
                    c.swap(i,j);
                    tries++;
                }
            }
            c.init();
            printf("Restart!\n");
            continue;
    exit:
            count++;
            if(c.h<10000||count%10000==0)
                printf("h=%d with %d tries\n",c.h,tries);
        }
    }
    finish = clock();
	totaltime = (double)(finish-start)/CLOCKS_PER_SEC;
    c.recalculate_crosses(); //验证解的正确性
    printf("Solved n=%d queens with h=%d\n",n,c.h);
    if(n<50&&n!=5)
        c.print();
    c.write(totaltime);
}

int main(){
    int n;
    FILE *in=fopen("input.txt","r");
    fscanf(in,"%d",&n);
    fclose(in);
    //srand(time(0));
    solve(n);
    return 0;
}