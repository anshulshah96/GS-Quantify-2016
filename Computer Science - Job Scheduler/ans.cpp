#include<bits/stdc++.h>
using namespace std;

// #define ll long long
#define inf 0x7fffffff
#define SCD(t) scanf("%d",&t)
#define SCLD(t) scanf("%ld",&t)
#define SCLLD(t) scanf("%lld",&t)
#define SCC(t) scanf("%c",&t)
#define SCS(t) scanf("%s",t)
#define SCF(t) scanf("%f",&t)
#define SCLF(t) scanf("%lf",&t)
#define pr pair<int,int>
#define mp(a,b) make_pair(a,b)
#define pb push_back
#define eb emplace_back
#define fr first
#define sc second
#define mset(arr,val) memset(arr,val,sizeof(arr));

typedef long long ll;

const int MAX = 1000000;
ll cpu_tot, cpu_free; 
ll tm_asg = 0 ,tm_qt = 0, tm_qo = 0;

struct jd
{
	ll id;
	ll timestamp;
	string orig;
	string instr;
	ll importance;
	ll duration;
	ll cpustart;
	jd(ll i,ll t,string o,string ins, ll imp,ll dur)
	{
		id = i;
		timestamp = t;
		orig = o;
		instr = ins;
		importance = imp;
		duration = dur;
	}
};
struct snap
{
	ll ctime;
	std::vector< jd > adds;
	std::vector< jd > rems;
};

struct qcompare{
	bool operator()(const jd& l, const jd& r)
	{
		bool ans;
		if(l.importance != r.importance)
			ans = l.importance > r.importance;
		else if (l.timestamp != r.timestamp)
			ans = l.timestamp < r.timestamp;
		else 
			ans = l.duration < r.duration;
		return !ans;
	}
};
struct cpucompare{
	bool operator()(const jd& l, const jd& r)
	{
		bool ans;

		ans = l.cpustart + l.duration < r.cpustart+r.duration;

		return !ans;
	}
};

std::vector< snap > snap_vec;
string temp;

priority_queue< jd, vector<jd>, qcompare > aspq;
priority_queue< jd, vector<jd>, cpucompare > cpupq;

void add_job(){
	
}

void pre_time(ll times)
{
	if(cpupq.empty()) return;
	jd top = cpupq
	while()
}
int main(){
	freopen("input.txt","r",stdin);
	cin>>temp;
	cin>>cpus_tot;
	cpu_free = cpu_tot;

	while(cin)
	{
		string s;
		cin>>s;
		if(s == "") continue;
		else if(s[0]=='j')
		{
			ll times,id,imp,dur;
			cin>>times>>id;
			string orig,instr;
			cin>>orig>>instr;
			cin>>imp>>dur;
			pre_time(times);	// do remaining tasks upto this timestamp
			aspq.push(jd(times,id,orig,instr,imp,dur));
		}
	}
}