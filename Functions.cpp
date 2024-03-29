#include<bits/stdc++.h>
using namespace std;
#define MOD 1000000007
#define print_vector(v)     for (auto &it: v) { cout<<it<<" "; } cout<<endl;
typedef long long ll;
typedef unsigned long long ull;

ll nCr(ll n,ll r)
{
    if(r>n)
    return 0;
    if(r==n || r==0)
    return 1;
    else
    {
        ll k=1;
        for(ll i=r+1;i<=n;i++)
        k=k*i;
        for(ll i=1;i<=(n-r);i++)
        k/=i;
        return k;
    }
}
ll binarySearch(vector<ll> &v,ll n,ll k)
{
    ll lb=0;
    ll ub=n-1;
    while(ub-lb>1)
    {
        ll mid=lb+(ub-lb)/2;
        if(v[mid]>k)
        {
            ub=mid-1;
        }
        else
        {
            lb=mid;
        }
    }
    if(v[lb]==k)
    return lb;
    else if(v[ub]==k)
    return ub;
    else
    return -1;
}
ull power(ull x,ll y,ll p)
{
    ull res=1;
    x=x%MOD;
    while(y>0)
    {
        if(y&1)
        {
            res=(res*x)%MOD;
        }
    y=y>>1;
    x=(x*x)%p;
    }
    return res;
}
ull modInverse(ull n,ll p)
{
    return power(n,p-2,p);
}
ull nCrFermat(ull n,ll r,ll p)
{
    if(n<r)
    return 0;
    if(r==0)
    return 1;
    ull fac[n+1];
    fac[0]=1;
    for(ll i=1;i<=n;i++)
    fac[i]=(i*fac[i-1])%p;
    return(fac[n]*modInverse(fac[r],p)%p*modInverse(fac[n-r],p)%p)%p;
}
ll binExp(ll x,ll y)
{
    if(y==0)
    return 1;
    else if(y%2==1)
    {
        return x*binExp(x,y/2)*binExp(x,y/2);
    }
    else
    {
        return binExp(x,y/2)*binExp(x,y/2);
    }
}
map<ll,ll> primeFactorization(ll n)
{
    map<ll,ll> m;
    while(n%2==0)
    {
        m[2]++;
        n/=2;
    }
    for(ll i=3;i*i<=n;i+=2)
    {
        while(n%i==0)
        {
            m[i]++;
            n/=i;
        }
    }
    if(n>2)
    m[n]++;
    return m;
}

//Segment Tree
// TO ACCESS HIGHER AMOUNT OF MEMORY, DECLARE SEGMENT TREES AS GLOBAL
void buildSegTree(vector<ll> &v,vector<ll> &seg,ll n,ll l,ll r)
{
    //sizeof(seg) = 4*sizeof(v)
    if(l==r)
    {
        seg[n]=v[l];
        return;
    }
    ll mid=(l+r)/2;
    buildSegTree(v,seg,2*n+1,l,mid);
    buildSegTree(v,seg,2*n+2,mid+1,r);
    seg[n]=seg[2*n+1]+seg[2*n+2];
}
ll query(vector<ll> &seg,ll n,ll low,ll high,ll l, ll r)
{
    //low,high -seg tree ranges
    //l,r - requested ranges
    //n = 0 for root
    //Requested Range must engulf node range

    // Change Returning statement values for queries regarding MAX,MIN (return INT_MAX or INT_MIN)
    if(low>=l && high<=r)
    return seg[n];
    else if(high<l || low>r)
    return 0;
    int mid=(low+high)/2;
    return query(seg,2*n+1,low,mid,l,r)+query(seg,2*n+2,mid+1,high,l,r);
}
void update(vector<ll> &seg,vector<ll> &v,ll n,ll l,ll r,ll idx,ll val)
{
    //POINT UPDATE
    //idx- index of array to update
    //val- value (TO BE ADDED,NOT SUBSTITUTED)
    if(l==r)
    {
        v[idx]+=val;
        seg[n]+=val;
        return;
    }
    ll mid=(l+r)/2;
    if(idx<=mid)
    update(seg,v,2*n+1,l,mid,idx,val);
    else
    update(seg,v,2*n+2,mid+1,r,idx,val);
    seg[n]=seg[2*n+1]+seg[2*n+2];
}
void rangeUpdate(vector<ll> &seg,vector<ll> &lazy,vector<ll> &v,ll n,ll low,ll high,ll l,ll r,ll val)
{
    //RANGE UPDATE
    //low,high -seg tree ranges
    //l,r - requested ranges
    //n = 0 for root
    //Requested Range must engulf node range
    if(lazy[n]!=0)
    {
        seg[n]+=(high-low+1)*lazy[n];
        if(low!=high)
        {
            lazy[2*n+1]+=lazy[n];
            lazy[2*n+2]+=lazy[n];
        }
        lazy[n]=0;
    }

    if(low>r || high<l || low>high)
    return;

    if(low>=l && high<=r)
    {
        seg[n]+=(high-low+1)*val;
        if(low!=high)
        {
            // lazy[2*n+1]+=lazy[n];
            // lazy[2*n+2]+=lazy[n];
            lazy[2*n+1]+=val;
            lazy[2*n+2]+=val;
        }
        return;
    }
    ll mid=(low+high)/2;
    rangeUpdate(seg,lazy,v,2*n+1,low,mid,l,r,val);
    rangeUpdate(seg,lazy,v,2*n+2,mid+1,high,l,r,val);
    seg[n]=seg[2*n+1]+seg[2*n+2];
}

ll rangeQueryLazy(vector<ll> &seg,vector<ll> &lazy,vector<ll> &v,ll n,ll low,ll high,ll l,ll r)
{
    if(lazy[n]!=0)
    {
        seg[n]+=(high-low+1)*lazy[n];
        if(low!=high)
        {
            lazy[2*n+1]+=lazy[n];
            lazy[2*n+2]+=lazy[n];
        }
        lazy[n]=0;
    }

    if(low>r || high<l)
    return 0;
    if(low>=l && high<=r)
    {
        if(lazy[n]!=0)
        {
            seg[n]+=(high-low+1)*lazy[n];
            if(low!=high)
            {
                lazy[2*n+1]+=lazy[n];
                lazy[2*n+2]+=lazy[n];
            }
            lazy[n]=0;
        }
        return seg[n];
    }
    ll mid=(low+high)/2;
    return rangeQueryLazy(seg,lazy,v,2*n+1,low,mid,l,r)+rangeQueryLazy(seg,lazy,v,2*n+2,mid+1,high,l,r);

}

//DSU
void unite(ll a,ll b,vector<ll> &par,vector<ll> &sz)
{
    a=getParent(a,par);
    b=getParent(b,par);
    if(a==b)
    return;
    if(sz[a]<sz[b])
    swap(a,b);
    par[b]=a;
    sz[a]+=sz[b];
}
ll getParent(ll n,vector<ll> &par)
{
    if(par[n]==n)
    return n;
    return par[n]=getParent(par[n],par);
}

//Dijkstra's Algorithm
void dijkstra(vector<vector<pair<ll,ll>>> &adj,ll src,ll n)
{
    //Finds shortest distance to all connected nodes from src
    priority_queue<pair<ll,ll>,vector<pair<ll,ll>>,greater<pair<ll,ll>>> q;
    q.push({0,src});
    ll d=0,MIN=INT_MAX;
    vector<bool> hasVisited(n+1,false);
    vector<ll> shortestDist(n+1,INT_MAX);
    shortestDist[src]=0;
    while (!q.empty())
    {
        ll x=q.top().second;
        hasVisited[x]=true;
        // cout<<x<<endl;
        d=q.top().first;
        for(auto i:adj[x])
        {
            if(hasVisited[i.first]==false)
            {
                shortestDist[i.first]=min(shortestDist[i.first],shortestDist[x]+i.second);
                q.push({shortestDist[i.first],i.first});
            }
        }
        q.pop();
    }
    print_vector(shortestDist);
}

//LCA using binary lifting and In-out time
void DFS(vector<vector<ll>> &adj,ll n,ll parent,vector<ll> &in,vector<ll> &out,vector<vector<ll>> &up,ll &timer,int L)
{
    in[n]=timer++;
    up[n][0]=parent;
    //Set L=log(num) where num is number of nodes in graph
    for(ll i=1;i<=L;i++)
    {
        up[n][i]=up[up[n][i-1]][i-1];
    }
    for(auto &i:adj[n])
    {
        if(i==parent)
        continue;
        DFS(adj,i,n,in,out,up,timer,L);
    }
    out[n]=timer++;
}
bool isAncestor(ll u,ll v,vector<ll> &in,vector<ll> &out)
{
    if(in[u]<=in[v] && out[u]>=out[v])
    return 1;
    return 0;
}
ll LCA(vector<vector<ll>> &up,ll u,ll v,vector<ll> &in,vector<ll> &out,ll L)
{
    if(isAncestor(u,v,in,out))
    return u;
    if(isAncestor(v,u,in,out))
    return v;
    for(ll i=L;i>=0;i--)
    {
        if(!isAncestor(up[u][i],v,in,out))
        u=up[u][i];
    }
    return up[u][0];
}