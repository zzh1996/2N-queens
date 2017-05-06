#include <cstdio>
#include <cstdlib>

int main(int argc, char **argv){
    if(argc<2)
        return -1;
    int n;
    FILE *in=fopen("input.txt","r");
    fscanf(in,"%d",&n);
    fclose(in);
    in=fopen(argv[1],"r");
    int *q1=new int[n];
    int *q2=new int[n];
    int *c1=new int[n]();
    int *c2=new int[n]();
    int *lc1=new int[2*n-1]();
    int *rc1=new int[2*n-1]();
    int *lc2=new int[2*n-1]();
    int *rc2=new int[2*n-1]();
    for(int i=0;i<n;i++)
        fscanf(in,"%d",&q1[i]);
    for(int i=0;i<n;i++)
        fscanf(in,"%d",&q2[i]);
    fclose(in);
    for(int i=0;i<n;i++){
        if(q1[i]<1||q1[i]>n||q2[i]<1||q2[i]>n)
            printf("out of range\n");
        if(q1[i]==q2[i])
            printf("overlap\n");
        q1[i]--;
        q2[i]--;
        c1[q1[i]]++;
        c2[q2[i]]++;
        lc1[i+q1[i]]++;
        rc1[i+n-1-q1[i]]++;
        lc2[i+q2[i]]++;
        rc2[i+n-1-q2[i]]++;
    }
    for(int i=0;i<n;i++){
        if(c1[i]!=1||c2[i]!=1)
            printf("column conflict\n");
    }
    for(int i=0;i<2*n-1;i++){
        if(lc1[i]>1||rc1[i]>1||lc2[i]>1||rc2[i]>1)
            printf("cross conflict\n");
    }
    return 0;
}