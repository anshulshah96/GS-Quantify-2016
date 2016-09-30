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
	jd(ll t,ll i,string o,string ins,ll imp,ll dur)
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
	std::vector< jd > addj;
	// std::vector< jd > remj;
	std::vector< string > remj;
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
// struct cpucompare{
// 	bool operator()(const jd& l, const jd& r)
// 	{
// 		bool ans;

// 		ans = l.cpustart + l.duration < r.cpustart+r.duration;

// 		return !ans;
// 	}
// };

std::vector< snap > snap_vec;
string temp;

priority_queue< jd, vector<jd>, qcompare > aspq;
priority_queue< ll, vector<ll>, greater<ll> > cpupq;

void add_job(jd job){
	if(snap_vec.empty() || snap_vec.back().ctime<job.timestamp)
	{
		snap sn;
		sn.ctime = job.timestamp;
		sn.addj.pb(job);
		snap_vec.pb(sn);
	}
	else{
		snap_vec.back().addj.pb(job);
	}
}
void rem_job(jd job){
	if(snap_vec.empty() || snap_vec.back().ctime<job.timestamp)
	{
		snap sn;
		sn.ctime = job.timestamp;
		sn.remj.pb(job.orig);
		snap_vec.pb(sn);
	}
	else{
		snap_vec.back().remj.pb(job.orig);
	}
}
void print_job(jd job){
	cout<<"job "<<job.timestamp<<" "<<job.id<<" "<<job.orig<<" "<<job.instr<<" "<<job.importance<<" "<<job.duration<<endl;
}
void pre_time(ll times)
{
	if(cpupq.empty()) return;
	while(cpupq.top() <= times){
		cpu_free++;
		cpupq.pop();
	}
}

void see_snaps(ll times)
{
	cout<<"------------------";
	cout<<endl<<" FOR "<<times<<endl;
	cout<<"------------------";
	for(int i = 0;i<snap_vec.size();i++)
	{
		snap sn = snap_vec[i];
		cout<<endl<<"snap "<<sn.ctime<<endl;
		for(int  j = 0;j<sn.addj.size();j++)
			print_job(sn.addj[j]);
		for(int  j=  0;j<sn.remj.size();j++)
			cout<<sn.remj[j]<<endl;
	}
}
int main(){
	freopen("input.txt","r",stdin);
	cin>>temp;
	cin>>cpu_tot;
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
			// pre_time(times);	// do remaining tasks upto this timestamp
			jd job = jd(times,id,orig,instr,imp,dur);
			aspq.push(job);
			add_job(job);

			see_snaps(times);
		}
		else if(s[0] == 'a')
		{
			ll times,k;
			cin>>times>>k;
			pre_time(times);   // do remaining tasks upto this timestamp
			k = min(cpu_free,k);
			while(!aspq.empty() && k)
			{
				k--;
				jd job = aspq.top();
				aspq.pop();
				cpupq.push(job.duration+times);
				print_job(job);
				rem_job(job);
			}
			see_snaps(times);
		}
	}
}