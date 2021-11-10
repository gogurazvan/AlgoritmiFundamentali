#include<bits/stdc++.h>

using namespace std;
ifstream in("alg.in");
ofstream out("alg.out");
const int nmax = 100001;

class Graf{
    int n, m;
    vector<int> vecin[nmax];


    void recursieDFS(int nod, bool vizitat[]){
        vizitat[nod]=true;
        for(int i=0; i<vecin[nod].size(); ++i)
            if(vizitat[vecin[nod][i]]==false)
                recursieDFS(vecin[nod][i], vizitat);

    }



    void recursieCTC(int nod, int vizitat[], int varf[], stack <int> &s, bool scheck[], int &cont,int &nr,int sol[],int &k){

        vizitat[nod]=cont;
        scheck[nod]=true;
        varf[nod]=cont;
        s.push(nod);
        for(int i=0; i<vecin[nod].size(); ++i){
            if(vizitat[vecin[nod][i]]==-1){
                cont=cont+1;
                recursieCTC(vecin[nod][i], vizitat,varf,s,scheck,cont,nr,sol,k);
                varf[nod]=min( varf[nod], varf[ vecin[nod][i] ] );
            }else if(scheck[vecin[nod][i]]==true) varf[nod]=min( varf[nod], vizitat[ vecin[nod][i] ] );
        }
        if( varf[nod] == vizitat[nod] ){
            nr++;
            while(s.top()!=nod){
                sol[k]=s.top(); k++;
                scheck[s.top()]=false;
                s.pop();
            }
            sol[k]=s.top(); k++;
            sol[k]=-1; k++;
            scheck[s.top()]=false;
            s.pop();
        }

    }

    void recursieBC(int nod, int vizitat[], int varf[], stack <int> &s, int &cont, int &nr,int sol[],int &k){

        vizitat[nod]=cont;
        varf[nod]=cont;
        cont++;
        s.push(nod);
        for(int i=0; i<vecin[nod].size(); ++i){
            if(vizitat[vecin[nod][i]]==-1){

                s.push(nod*(-1));
                recursieBC(vecin[nod][i], vizitat,varf,s,cont,nr,sol,k);

                if( varf[ vecin[nod][i] ] >= vizitat[nod] ){
                    nr++;
                    while(s.top()!=nod && s.top()!=nod*(-1)){
                        if(s.top()>0){ sol[k]=s.top(); k++;}
                        s.pop();
                    }
                    sol[k]=nod; k++;
                    sol[k]=-1; k++;
                }else{
                    varf[nod]=min( varf[nod], varf[ vecin[nod][i] ] );
                }

            }else varf[nod]=min( varf[nod], vizitat[ vecin[nod][i] ] );
        }


    }

public:

    Graf(int n, int m){
        this->n=n;
        this->m=m;
    }

    void citireM( bool orient ){
        for( int i=0; i<m; ++i ){

            int a, b;  in >> a >> b;
            vecin[a].push_back(b);
            if( orient == false ) vecin[b].push_back(a);
        }
    }

    void DFS(){

        int nr_comp_conex=0;
        bool vizitat[n];

        for(int i=1;i<=n;++i)
            vizitat[i]=false;

        for(int i=1;i<=n;++i){
            if(vizitat[i]==false){
                recursieDFS(i,vizitat);
                nr_comp_conex++;
            }
        }
        out<<nr_comp_conex;

    }

    void BFS(int s){

        int drum[n+1];

        for(int i=1;i<=n;++i)
            drum[i]=-1;

        drum[s]=0;
        queue <int> poz;
        poz.push(s);

        while(!poz.empty()){
            int nod=poz.front();
            for(int i=0; i<vecin[nod].size(); ++i){
                if( drum[vecin[nod][i]] == -1 ){
                    poz.push( vecin[nod][i] );
                    drum[vecin[nod][i]]=drum[nod]+1;
                }
            }
            poz.pop();
        }

        for(int i=1;i<=n;++i)
            out<<drum[i]<<" ";

    }

    void CTC(){
        int desc[n+1], varf[n+1];
        stack <int> s;
        bool scheck[n+1];
        for(int i=1;i<=n;++i){
            desc[i]=-1;
            varf[i]=-1;
            scheck[i]=false;
        }
        int cont=0,nr=0,solutie[2*n+5], k=0;
        for(int i=1;i<=n;++i){
            if(desc[i]==-1)
                recursieCTC(i,desc,varf,s,scheck,cont,nr,solutie,k);

        }
        out<<nr<<"\n";
        int j=k-2;
        while(j>=0){
            while(solutie[j]!=-1 && j>=0){
                out<<solutie[j]<<" ";
                --j;
            }
            nr--; j--; out<<"\n";
        }

    }

    void BC(){
        int desc[n+1], varf[n+1];
        stack <int> s;
        for(int i=1;i<=n;++i){
            desc[i]=-1;
            varf[i]=-1;
        }
        int cont=0,nr=0,solutie[2*n+5], k=0;
        for(int i=1;i<=n;++i){
            if(desc[i]==-1){
                if(vecin[i].empty()==true){

                    nr++;
                    solutie[k]=i; k++;
                    solutie[k]=-1; k++;
                }else{

                    recursieBC(i,desc,varf,s,cont,nr,solutie,k);
                }
                while(s.empty()!=true) s.pop();
            }

        }
        out<<nr<<"\n";
        int j=k-2;
        while(j>=0){
            while(solutie[j]!=-1 && j>=0){
                out<<solutie[j]<<" ";
                --j;
            }
            nr--; j--; out<<"\n";
        }




    }


    string HH(){
        int grad[n+3];
        for(int i=1; i<=n;++i)
            in>>grad[i];
        sort(grad+1,grad+n+1,greater<int>());

        int nr0=0;
        for(int i=1; i<=n; ++i){
            if(grad[i]==0) return "este graf";
            if(grad[i]>n-i) return "nu este graf";
            if(grad[i]==1){
                if((n-i-nr0+1)%2==0) return "este graf";
                else return "nu este graf";
            }
            for(int j=i+1; j<=i+grad[i]; ++j){

                grad[j]--;
                if(grad[j]<0) return "nu este graf";
                if(grad[j]==0) nr0++;
            }
            sort(grad+i+1,grad+n+1,greater<int>());
        }
        return "este graf";
    }

};




int main()
{
    int n,m;
    in>>n>>m;

    Graf g(n,m);
    g.citireM(false);
    g.BC();


    return 0;
}
