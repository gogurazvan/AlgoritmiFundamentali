#include<bits/stdc++.h>

using namespace std;
ifstream in("alg.in");
ofstream out("alg.out");
const int nmax = 100001;

struct muchie_prop{
    int cost;
};

class Graf{
    int n, m;
    vector<int> vecin[nmax];
    map<pair<int, int>, muchie_prop> prop;


    void recursieDFS(int nod, bool vizitat[]){
        vizitat[nod]=true;
        for(int i=0; i<vecin[nod].size(); ++i)
            if(vizitat[vecin[nod][i]]==false)
                recursieDFS(vecin[nod][i], vizitat);

    }
    void recursieDFSdistante(int nod, int vizitat[],int k){
        vizitat[nod]=k;
        for(int i=0; i<vecin[nod].size(); ++i)
            if(vizitat[vecin[nod][i]]==-1)
                recursieDFSdistante(vecin[nod][i], vizitat,k+1);

    }

    void recursieDFS_sort(int nod, bool vizitat[],stack <int> &s){
        vizitat[nod]=true;
        for(int i=0; i<vecin[nod].size(); ++i)
            if(vizitat[vecin[nod][i]]==false)
                recursieDFS_sort(vecin[nod][i], vizitat,s);

        s.push(nod);

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

    void citireM( bool orient , int nivel_costuri=0){

        for( int i=0; i<m; ++i ){

            int a, b, c;
            in >> a >> b;
            vecin[a].push_back(b);

            if( nivel_costuri>=1 ){
                in>>c;
                prop[{a,b}].cost=c;
            }

            if( orient == false ){
                vecin[b].push_back(a);
                if( nivel_costuri>=1 ){
                    prop[{b,a}].cost=c;
                }
            }
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


    bool HH(){
        int grad[n+3];
        for(int i=1; i<=n;++i)
            in>>grad[i];
        sort(grad+1,grad+n+1,greater<int>());

        int nr0=0;
        for(int i=1; i<=n; ++i){
            if(grad[i]==0) return true;
            if(grad[i]>n-i) return false;
            if(grad[i]==1){
                if((n-i-nr0+1)%2==0) return true;
                else return false;
            }
            for(int j=i+1; j<=i+grad[i]; ++j){

                grad[j]--;
                if(grad[j]<0) return false;
                if(grad[j]==0) nr0++;
            }
            sort(grad+i+1,grad+n+1,greater<int>());
        }
        return true;
    }

    void ST(){
        bool vizitat[n+1];
        stack <int> st;

        for(int i=1; i<=n;++i)
            vizitat[i]=false;
        for(int i=1; i<=n;++i)
            if(vizitat[i]==false)
                recursieDFS_sort(i,vizitat,st);

        while(st.empty()!=true){
            out<<st.top()<<" ";
            st.pop();
        }
    }
    int Darb(){
        int distante[100001];
        for(int i=1;i<=n;++i)
            distante[i]=-1;
        recursieDFSdistante(1,distante,0);
        int nod_mac=1,mac=0;
        for(int i=1;i<=n;++i){
            if(distante[i]>mac)
            {
                mac=distante[i];
                nod_mac=i;
            }
            distante[i]=-1;
        }
        recursieDFSdistante(nod_mac,distante,0);
        for(int i=1;i<=n;++i)
            if(distante[i]>mac)
                mac=distante[i];

        return mac+1;



    }

    void Roy_Floyd(int M[101][101]){
        for(int k=1;k<=n;++k)
            for(int i=1;i<=n;++i)
                for(int j=1;j<=n;++j){
                    if( (M[i][j] > M[i][k]+M[k][j] || ( M[i][j]==0 && i!=j )) && M[i][k]!=0 && M[k][j]!=0)
                        M[i][j]=M[i][k]+M[k][j];
                }
    }

    void Dijkstra(int start, int dist[]){
        bool viz[n+1];
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>> > dist_min;
        for( int i=1; i<=n; ++i ){
            viz[i]=false;
            dist[i]=-1;
        }
        int p = start;
        dist[start]=0;

        for( int i=1; i<=n; ++i ){
            viz[p]=true;
            for( int j=0; j< vecin[p].size(); ++j ){
                int nvec = vecin[p][j];
                if( !viz[nvec] && ( dist[nvec] == -1  ||  dist[nvec] > dist[p]+ prop[{p,nvec}].cost ) ){
                    dist[nvec]= dist[p]+ prop[{p,nvec}].cost;
                    dist_min.push( make_pair(dist[nvec], nvec) );
                }

            }

            while( viz[ dist_min.top().second ] && !dist_min.empty() )
                dist_min.pop();
            if(!dist_min.empty()){
                p=dist_min.top().second;
                dist_min.pop();
            }





        }

    }



};




int main()
{




    return 0;
}
